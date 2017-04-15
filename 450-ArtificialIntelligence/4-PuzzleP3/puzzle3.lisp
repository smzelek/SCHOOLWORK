;(1 2 3 8 e 4 7 6 5)
;(L (goal-state))

(defun goal-state (board-state)
  (equal board-state '(1 2 3 8 e 4 7 6 5)))

(defun get-direction (move)
  (car move))

;(get-state '(u (1 2 3 8 e 4 7 6 5)))

(defun get-state (move)
  (cadr move))

(defun same-state (move1 move2)
  (equal (get-state move1) (get-state move2)))

;(same-state '(u (1 2 3 8 e 4 7 6 5)) '(d (1 2 3 8 e 4 7 6 5)))

(defun path (moveList)
  (if (null (cdr (cdr moveList)))
    (list(get-direction (car moveList)))
    (append (path (cdr moveList)) (list(get-direction (car moveList))))))

(defun remove-redundant (moveList1 moveList2)
  (cond ((null moveList1) nil)
    ((dup-state (car moveList1) moveList2) (remove-redundant (cdr moveList1) moveList2))
    (t (cons (car moveList1) (remove-redundant (cdr moveList1) moveList2)))))

(defun dup-state (move moveList)
  (cond ((same-state move (car moveList)) t)
    ((null moveList) NIL)
    (t (dup-state move (cdr moveList)))))

(defun moves (state)
  (append (get-up-move state) (get-down-move state) (get-left-move state) (get-right-move state)))

(defun get-up-move (state)
  (if (in-top-row state)
    NIL
    (list (cons 'U (list (transpose (swap-left (transpose state))))))))

(defun get-down-move (state)
  (if (in-bottom-row state)
    NIL
    (list (cons 'D (list (transpose (swap-right (transpose state))))))))

(defun get-left-move (state)
  (if (in-left-col state)
    NIL
    (list (cons 'L (list (swap-left state))))))

(defun get-right-move (state)
  (if (in-right-col state)
    NIL
    (list (cons 'R (list (swap-right state))))))

(defun transpose (state)
  (cons (nth 0 state) (cons (nth 3 state) (cons (nth 6 state)
  (cons (nth 1 state) (cons (nth 4 state) (cons (nth 7 state)
  (cons (nth 2 state) (cons (nth 5 state) (cons (nth 8 state) NIL))))))))))

(defun swap-right (state)
  (cond ((null state) NIL)
    ;if current element is E, add next element, then E, then rest
    ((equal (car state) 'e) (cons (cadr state) (cons 'e (cddr state))))
    (t (cons (car state) (swap-right (cdr state))))))

(defun swap-left (state)
  (cond ((null state) NIL)
    ;if next element is e, add e, THEN current, then add the rest of the list
    ((equal (cadr state) 'e) (cons 'e (cons (car state) (cddr state))))
    (t (cons (car state) (swap-left (cdr state))))))

(defun in-top-row (state)
  (< (empty-idx state) 3))

(defun in-left-col (state)
  (or (eq (empty-idx state) 0) (eq (empty-idx state) 3) (eq (empty-idx state) 6)))

(defun in-right-col (state)
  (or (eq (empty-idx state) 2) (eq (empty-idx state) 5) (eq (empty-idx state) 8)))

(defun in-bottom-row (state)
  (> (empty-idx state) 5))

(defun empty-idx (state)
  (if (equal (car state) 'e)
    0
    (+ 1 (empty-idx (cdr state)))))

(defun search-bfs (frontier)
  (cond
    ;if options are empty, we OUT!
    ((null frontier) NIL)
    ;get state of first move of first path in frontier -> equal to goal state?
    ;then return the moveList of the first path of the frontier
    ((goal-state (get-state (first (first frontier))))  (path (first frontier)))
    ;otherwise
    (t (search-bfs (append (cdr frontier) (extend-path (first frontier )))))))

(defun search-dfs-fd (frontier depthbound)
  (cond
    ;if options are empty, we OUT!
    ((null frontier) NIL)
    ;if length of path greater than depth bound... skip it
    ((> (- (length (first frontier)) depthbound) 1)  (search-dfs-fd (cdr frontier) depthbound))
    ;get state of first move of first path in frontier -> equal to goal state?
    ;then return the moveList of the first path of the frontier
    ((goal-state (get-state (first (first frontier))))  (path (first frontier)))
    ;otherwise
    (t (search-dfs-fd (append (extend-path (first frontier )) (cdr frontier)) depthbound))))

;(search-id (make-open-init '(2 8 3 1 6 4 7 e 5)))

;curdepth will start at 1
(defun search-id (frontier &optional (curdepth 1))
  (cond
    ((null frontier) NIL)
    ;if search-dfs did not find goal path, try again at higher depth limit
    (t
      (let ((result (search-dfs-fd frontier curdepth)))
        (if (null result)
          (search-id frontier (+ curdepth 1))
          result)))))

(defun make-open-init (state)
  (list (list (cons NIL (cons state NIL)))))

;(extend-path (first (make-open-init '(2 8 3 1 6 4 7 e 5))))
;(extend-path '((NIL (2 8 3 1 6 4 7 e 5)) (U (2 8 3 1 E 4 7 6 5))))
;(extend-path '((U (2 8 3 1 e 4 7 6 5)) (NIL (2 8 3 1 6 4 7 e 5))))

(defun extend-path (path)
  (mapcar #'(lambda (x) (cons x path)) (remove-redundant (moves (get-state (first path))) path)))

;(out-of-place '(2 8 3 1 6 4 7 e 5))
;(out-of-place '(1 2 3 8 e 4 7 6 5))

(defun out-of-place (state &optional (gstate '(1 2 3 8 e 4 7 6 5)))
  (cond
    ((null state) 0)
    ((equal (car state) 'e) (+ 0 (out-of-place (cdr state) (cdr gstate))))
    ((equal (car state) (car gstate)) (+ 0 (out-of-place (cdr state) (cdr gstate))))
    (t (+ 1 (out-of-place (cdr state) (cdr gstate))))))

;(out-of-place-f '((D (1 2 3 e 8 4 7 6 5)) (R (e 2 3 1 8 4 7 6 5)) (U (1 2 3 4 8 7 e 6 5)) (nil (1 2 3 e 8 7 4 6 5))))

(defun out-of-place-f (path)
  (+ (out-of-place (get-state (car path))) (- (length path) 1)))

(defun get-goal-col (num)
  (cond
    ((eq num 1) 0)
    ((eq num 2) 1)
    ((eq num 3) 2)
    ((eq num 8) 0)
    ((eq num 4) 2)
    ((eq num 7) 0)
    ((eq num 6) 1)
    ((eq num 5) 2)))

(defun get-goal-row (num)
  (cond
    ((eq num 1) 0)
    ((eq num 2) 0)
    ((eq num 3) 0)
    ((eq num 8) 1)
    ((eq num 4) 1)
    ((eq num 7) 2)
    ((eq num 6) 2)
    ((eq num 5) 2)))

;(manhattan '(2 1 4 8 6 5 3 e 7))
;(manhattan '(1 2 3 8 e 4 7 6 5))

(defun manhattan (state &optional (row 0) (col 0))
  (cond
    ((null state) 0)
    ((eq col 2) ;row=row+1 col=0
      (+ (get-total-difference (car state) row col) (manhattan (cdr state) (+ row 1) 0)))
    (t ;row=row, col=col+1
      (+ (get-total-difference (car state) row col) (manhattan (cdr state) row (+ col 1))))))

(defun get-total-difference (num row col)
  (if (eq num 'e)
    0
    (+ (difference (get-goal-row num) row) (difference (get-goal-col num) col))))

(defun difference (num1 num2)
  (if (< (- num1 num2) 0)
    (- num2 num1)
    (- num1 num2)))

;(manhattan-f '((D (1 2 3 e 8 4 7 6 5)) (R (e 2 3 1 8 4 7 6 5)) (U (1 2 3 4 8 7 e 6 5)) (nil (1 2 3 e 8 7 4 6 5))))

(defun manhattan-f (path)
  (+ (manhattan (get-state (car path)) 0 0) (- (length path) 1)))

;(funcall (better #'out-of-place-f) p1 p2)
;(funcall (better #'out-of-place-f) p2 p1)
;(funcall (better #'out-of-place-f) p3 p2)

;return T if path1 is better than or equal to path2
(defun better (eval-f)
  (lambda (path1 path2)
    (<= (funcall eval-f path1) (funcall eval-f path2))))

;(merge 'list (cdr frontier) (sort (extend-path (first frontier)) (better #'out-of-place-f)) (better #'out-of-place-f))
;(search-a* (make-open-init s1) #'out-of-place-f)

;assume paths in frontier already sorted by heuristic
;search a* operates on frontier by
;expanding the first path in frontier (best one)
;sorting expand-path result by heuristic
;merge sorted expansion into frontier
;pass frontier and heuristic into a*

(defun search-a* (frontier heuristic-f)
  (cond
    ((null frontier) NIL)
    ((goal-state (get-state (first (first frontier)))) (path (first frontier)))
    (t (search-a* (merge 'list (cdr frontier) (sort (extend-path (first frontier)) (better heuristic-f)) (better heuristic-f)) heuristic-f))))

(defun sss (state &key (type 'BFS) (depth 7) (f #'out-of-place-f))
  (cond
    ((goal-state state) NIL)
    ((equal type 'BFS) (search-bfs (make-open-init state)))
    ((equal type 'DFS) (search-dfs-fd (make-open-init state) depth))
    ((equal type 'ID)  (search-id (make-open-init state)))
    ((equal type 'A*)  (search-a* (make-open-init state) f))
    (t NIL)))

;(make-open-init '(2 8 3 1 6 4 7 e 5))
;(sort (extend-path (first (make-open-init '(2 8 3 1 6 4 7 e 5)))) (better #'out-of-place-f))
;(search-bfs (make-open-init '(2 8 3 1 6 4 7 e 5)))
;(search-dfs-fd (make-open-init '(2 8 3 1 6 4 7 e 5)) 7)
;(search-id (make-open-init '(2 8 3 1 6 4 7 e 5)))
;(search-id (make-open-init '(2 8 1 e 6 3 7 5 4)))
;(search-id (make-open-init '(3 1 2 e 8 4 7 6 5)))
;(search-id (make-open-init '(7 1 3 e 8 4 6 5 2)))
