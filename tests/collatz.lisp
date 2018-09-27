#!/usr/local/bin/ispl
(progn
  (set 'collatz (lambda '(n) '(if (== n 1) '(1) (cons n (recurse (if (== (mod n 2) 0) (/ n 2) (+ (* 3 n) 1)))))))
  (set 'n (read))
  (if (not (eq (typeof n) 'INTEGER)) (set 'n 670617279))
  (set 'c (collatz n))
  (print c \n (length c) \n)
  )
