#!/usr/local/bin/ispl
(progn
  (source lib)
  (foreach 'row (mapcar (lambda '(n) '(mapcar (lambda '(m) `(* ,n m)) (range 1 1 10))) (range 1 1 10)) '(progn (foreach 'n row '(print n \t)) (terpri)))
  )
