(progn
  (set 'merge (lambda '(a b f) '(cond ((null a) b)
				      ((null b) a)
				      ((f (car a) (car b)) (cons (car a) (recurse (cdr a) b f)))
				      (t (cons (car b) (recurse a (cdr b) f))))))
  (set 'nthcdr (lambda '(n l) '(if (> n 0) (recurse (- n 1) (cdr l)) l)))
  (set 'nsplit (lambda '(l) '(progn
			      (set 'n (- (/ (length l) 2) 1))
			      (set 'p (nthcdr n l))
			      (set 'c (cdr p))
			      (rplacd p nil)
			      (cons l c))))
  (set 'sort (lambda '(l f) '(if (< (length l) 2) l
			       (progn (set 'l (nsplit l))
				 (merge (sort (car l) f) (sort (cdr l) f) f)))))
  )
