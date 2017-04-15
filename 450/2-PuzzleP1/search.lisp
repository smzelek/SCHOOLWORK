
(defun goal-state (boardState)
  (equal boardState '(1 2 3 8 e 4 7 6 5))
)

(defun get-direction (move)
  (car move))

(defun get-state (move)
(car (cdr move)))

(defun same-state (move1 move2)
  (equal (get-state move1) (get-state move2)))

(defun path (moveList)
  (if (null (cdr (cdr moveList))) (list(get-direction (car moveList)))
  (append (path (cdr moveList)) (list(get-direction (car moveList)))))
)

(defun remove-redundant (moveList1 moveList2)
  (cond ((null moveList1) nil)
        ((dup-state (car moveList1) moveList2) (remove-redundant (cdr moveList1) moveList2))
        (t (cons (car moveList1) (remove-redundant (cdr moveList1) moveList2)))
  )
)

(defun dup-state (move moveList)
  (cond ((same-state move (car moveList)) t)
        ((null moveList) NIL)
        (t (dup-state move (cdr moveList))))
)


(defun moves (state)
  (append (get-up-move state) (get-down-move state) (get-left-move state) (get-right-move state))
)

(defun get-up-move (state)
  (if (in-top-row state) NIL
  (list (cons 'U (list (transpose (swap-left (transpose state)))))))
)

(defun get-down-move (state)
  (if (in-bottom-row state) NIL
  (list (cons 'D (list (transpose (swap-right (transpose state)))))))
)

(defun get-left-move (state)
  (if (in-left-col state) NIL
  (list (cons 'L (list (swap-left state)))))
)

(defun get-right-move (state)
  (if (in-right-col state) NIL
  (list (cons 'R (list (swap-right state)))))
)

(defun transpose (state)
  (cons (nth 0 state) (cons (nth 3 state) (cons (nth 6 state)
  (cons (nth 1 state) (cons (nth 4 state) (cons (nth 7 state)
  (cons (nth 2 state) (cons (nth 5 state) (cons (nth 8 state) NIL)))))))))
)

(defun swap-right (state)
  (cond ((null state) NIL)
    ;if current element is E, add next element, then E, then rest
    ((equal (car state) 'e) (cons (cadr state) (cons 'e (cddr state))))
    (t (cons (car state) (swap-right (cdr state))))
  )
)

(defun swap-left (state)
  (cond ((null state) NIL)
    ;if next element is e, add e, THEN current, then add the rest of the list
    ((equal (cadr state) 'e) (cons 'e (cons (car state) (cddr state))))
    (t (cons (car state) (swap-left (cdr state))))
  )
)

(defun in-top-row (state)
  (< (empty-idx state) 3)
)

(defun in-left-col (state)
  (or (eq (empty-idx state) 0) (eq (empty-idx state) 3) (eq (empty-idx state) 6))
)

(defun in-right-col (state)
  (or (eq (empty-idx state) 2) (eq (empty-idx state) 5) (eq (empty-idx state) 8))
)

(defun in-bottom-row (state)
  (> (empty-idx state) 5)
)

(defun empty-idx (state)
  (if (equal (car state) 'e) 0
    (+ 1 (empty-idx (cdr state)))
  )
)
