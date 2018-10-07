#!/usr/local/bin/ispl
(progn
  (set 'printer (lambda '(s) '(if s `(,(car s) \t @(recurse (cdr s))) `(\t ,(inside dictionary 'expr) \n))))
  (set 'loopbody (lambda '(s o) '(if s `(foreach (quote ,(car s)) '(nil t) (quote ,(recurse (cdr s) o))) o)))
  (print '"Variables: ") (set 'vars (read))
  (print '"Expression: ") (set 'expr (read))
  (foreach 'var vars '(print var \t))
  (terpri)
  (eval (loopbody vars (cons 'print (printer vars))))
  )
