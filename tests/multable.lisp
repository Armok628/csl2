#!/usr/local/bin/ispl
(progn
  (source lib)
  (set 'multiplier (lambda '(n) '(lambda '(m) `(* ,n m))))
  (set 'table_row (lambda '(n) '(mapcar (multiplier n) (range 1 1 10))))
  (foreach 'row (mapcar table_row (range 1 1 10)) '(progn (foreach 'n row '(print n \t)) (terpri)))
  )
