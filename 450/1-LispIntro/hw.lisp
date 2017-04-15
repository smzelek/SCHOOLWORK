(defun my-rotate (elems)
  (append (cdr elems) (list (car elems))))

(defun my-rotate-n (n elems)
  (if (<= n 0)
    elems
    (my-rotate-n (- n 1) (my-rotate elems))))

(defun first-sat (xs ys foo)
  (cond ((and xs ys) ;if non-empty lists
          (if (funcall foo (car xs) (car ys)) ;satisfies function?
            (list (car xs) (car ys)) ;if true, return pair
            (first-sat (cdr xs) (cdr ys) foo))) ;else try next pair
        (t NIL))) ;if one or more list is empty

(defun my-remove (R xs)
(cond ((null xs) ())
      ((listp (car xs)) (cons (my-remove R (car xs)) (my-remove R (cdr xs))))
      ((eq R (car xs)) (my-remove R (cdr xs)))
      (t (cons (car xs) (my-remove R (cdr xs))))
))

(defun palindromep (xs)
  (palindromehelper xs (floor (length xs) 2) 0)
)

(defun palindromehelper (xs numToCheck numChecked)
  (if (eq numToCheck 0)
    t
    (and (equal (nthItemFromEnd xs numChecked) (car xs)) (palindromehelper (cdr xs) (- numToCheck 1) (+ numChecked 1)))
  )
)

(defun nthItemFromEnd (xs n)
  (if (eq (- (length xs) 1) n)
    (car xs)
    (nthItemFromEnd (cdr xs) n)))
