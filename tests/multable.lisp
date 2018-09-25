#!/usr/local/bin/ispl
(progn
  (source lib)
  (set 'max (if (> (length argv) 2) (nth 2 argv) 12))
  (set 'multiplier (lambda '(n) '(lambda '(m) `(* ,n m))))
  (set 'tablerow (lambda '(n) '(mapcar (multiplier n) (range 1 1 max))))
  (foreach 'row (mapcar tablerow (range 1 1 max))
	   '(progn (foreach 'n row '(print n \t)) (terpri)))
  )
