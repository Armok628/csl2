# ispl
A small garbage-collected LISP dialect based on compilation to postfix notation

Written from scratch in pure C using only the standard library.

## ISPL Front-End

Compile with `make`.
The executable produced runs a typical REPL when given no arguments.
Results of REPL evaluations are prepended by a newline and "=> " when printed.
When given an argument, the executable interprets the argument as a script to be run.

Built-in functions can be listed with `cat src/*.c | grep INIT`.
Quotations and `PROGN`s are never implicit with these functions.

Functions do not contain their own namespaces, but namespaces can be created as objects.
Namespaces are described in more detail below.

All functions are created with `LAMBDA`, in which `PROGN` is still never implicit.
So, instead of `(defun name (args) ...)`, use `(set 'name (lambda '(args) '(progn ...)))`
Although longer, this allows for interesting code generation possibilities without the need for macros.

Speaking of code generation, a Common Lisp style backquote-unquote syntax is available,
with one minor difference: write `,@` as `@`.
This feature is implemented as a special set of parser rules.
In fact, ``` (quote `(,a @b c @d)) ``` yields `(CONS a (APPEND b (CONS (QUOTE c) d)))`.

Special forms include `LIST`, `QUOTE`, `COND`, `IF`, `PROGN`, and `PRINT`,
where by "special form" I mean a quasi-function implemented as a special translation rule,
and interpreted differently where encountered.

This is done to circumvent the limitation that stack-based notations do not group their arguments.
Therefore, this is done as a compromise to introduce otherwise-impossible variadic "functions."

All of the special forms work the same as their equivalent in other dialects,
except for `PRINT`, which is not typically a special form.

`PRINT` simply prints out all of its arguments without any added spaces or newlines, and returns `NIL`.
For convenience, variables `\t` and `\n` have been added,
which are bound to a symbol containing only their escape characters from C.

Another such standard variable, `ERROR`, is bound to the error object,
which is normally returned when a nonfatal error has occurred.

#### Inspiration from Tcl
Some features of this language are heavily inspired by Tcl.

Functions which act similarly to Tcl operations include
`+=`, `SOURCE`, `FOR`, `FOREACH`, `INSIDE` (`namespace eval`), `SET`, `UNSET`, `UPLEVEL`, `ASSIGN` (`lassign`), `ASET` (`lset`), and `AGET` (`lindex`).

In fact, like Tcl, `FOR` and `FOREACH` compile their script arguments only once, but interpret them many times.

Even the backquote-unquote syntax can be translated to/from Tcl in the context of substitution:

| ISPL | Tcl |
| :-: | :-: |
| `,...` | `$...` |
| `,(...)` | `[...]` |
| `@...` | `{*}...` |

Though the `SEE` function is actually inspired by Forth, Tcl has the comparable `info body`.
(However, the `SEE` function will actually show a function's arguments and *compiled* body.
Perhaps a better comparison would be to `tcl::unsupported::disassemble proc`.)

Also like Tcl, ISPL links identical symbols together by the same memory object to avoid redundancy.
In ISPL, these symbols (and their strings) are also garbage-collected.

The heavily annotated built-in example in example.c implements a function called `SOURCE`,
which does the same thing as Tcl's source command.

#### Namespace Management

Namespaces in ISPL work similarly to those in Tcl in that functions, like Tcl procedures,
can not, on their own, contain a static namespace.

Again similar to Tcl, namespaces are mostly explicit.
However, because functions are created as first-class objects,
they are not inherently bound to any namespace.

As a compromise, two namespaces are checked for a valid binding:
the global scope, in which built-in functions are defined,
and the local scope, created only when a lambda function is called.
Built-in functions, which are linked to C functions, execute in the namespace of their caller.

Just as in Tcl, namespaces can be manually created with the `NAMESPACE` function, which takes no arguments.
They can be manipulated using the `INSIDE` function, comparable to Tcl's `namespace eval`.

In fact, the global scope itself is bound to the symbol `DICTIONARY` inside itself,
so it can be manipulated with `INSIDE` as any other namespace.

Namespaces higher in the call stack can be manipulated using the `UPLEVEL` function.

## C Back-End

The header files contain short summaries of the purpose of their contents.

Implementation details and type definitions can be found in object.h.
Its source file, object.c, contains C functions only for direct use *inside the back-end.*
As a result, they generally avoid returning object pointers in favor of ordinary C types.
Such functions include those for memory management and type checking.

Built-in functions (such as those in core.c and arith.c)
are intended only for direct use *inside the language environment.*
As such, I have made a point of not declaring core functions in any headers.
They are made available to the language environment by their function pointers.

Side note: This project makes use of my own hash table implementation,
called [semstable](../../../semstable).

#### Notes on Built-in Functions

The source code for the core functions in core.c may seem odd.
A fair amount of work is done in functions generated by ~~quasi-recursive macros~~ preprocessor abuse,
used to convert C functions into a stack-based equivalent.

Simply put, the `STACK` macro generates an ISPL function
by popping arguments, pushing results, and decrementing references,
as well as creating a function object to represent it to the language.

The `INIT` macro binds the object representation to the function's name.

Although complex, this system greatly reduces the amount of work required to add new built-in functions.

##### To create source files for new built-in functions:

1. Make the header (with include guards) and `#include "core.h"` to gain access to the necessary macros.
3. Make an init function at the bottom of the source file, and declare it in the header.
2. Include your header in namespace.h and add a call to the new init function in `init_dict`.

See example.h, arith.h, and lmath.h as examples. See core.h for info about the included macros.

##### To create a new built-in function:

1. Write the C function. It must take only object pointers (or void) and return an object pointer.
   * The C function must follow the memory management guidelines below to ensure memory safety.
   * The C function is expected to `return error` if a nonfatal error has occurred,
   or to report the error to `stderr` and `exit(1)` if the error is necessarily fatal.
2. Use the `STACK(cname,argc)` macro after the function definition to generate a stack-based function.
3. Use the `INIT(lispname,cname)` macro in the init function to make the function available.

See example.c for a heavily annotated example.
See core.c for more complex examples.
See arith.c for even more examples (and a ton of lazy macros).
See lmath.c for bindings to C's standard math library.

##### Memory management guidelines for new built-in functions:

* Use `incr_refs` and `decr_refs` when storing or removing a reference, respectively.
* Destroy objects as soon as they become unneeded to avoid hiding memory management bugs.

Whether a C variable should count as a reference depends on the intended use of the object:

* If the object is generated for temporary use by the same function, increment its references.
  * This must be done so that its references can be decremented later.
  * If the object comes from `pop`, its references were already incremented by `push`.
* If the object will be generated as a return value, leave its references alone.
  * Memory management of returned objects is to be done by the caller.
  * If the object comes from the stack, use `dpop()` instead of `pop()`.
