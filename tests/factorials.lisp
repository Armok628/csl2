#!/usr/local/bin/ispl
(progn
  (print argv \n)
  (source lib)
  (set '! (lambda '(n) '(if (> n 1) (* n (recurse (- n 1))) 1)))
  (set 'n (if (> (length argv) 2) (car (cdr (cdr argv))) 1000))
  (tick)
  (for '(set 'i 0) '(< i n) '(+= 'i 1)
       '(print (array (mapcar ! (range 1 1 20))) \n))
  (print (/ (tock) n) '" seconds per iteration" \n)
  )
