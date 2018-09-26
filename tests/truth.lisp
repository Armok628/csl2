#!/usr/local/bin/ispl
(progn
  (set 'expr (read))
  (foreach 'a '(nil t)
 '(foreach 'b '(nil t)
 '(foreach 'c '(nil t)
 '(foreach 'd '(nil t)
	   '(print a \t b \t c \t d \t \t (eval expr) \n))))))
