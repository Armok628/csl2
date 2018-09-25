#!/usr/local/bin/ispl
(progn
  (source lib)
  (set 'max (if (> (length argv) 2) (nth 2 argv) 12))
  (foreach 'row (mapcar (lambda '(n) '(mapcar (lambda '(m) `(* ,n m)) (range 1 1 max))) (range 1 1 max)) '(progn (foreach 'n row '(print n \t)) (terpri)))
  )
