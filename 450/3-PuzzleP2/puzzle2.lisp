;(1 2 3 8 e 4 7 6 5)
;(L (goal-state))

(defun goal-state (boardState)
  (equal boardState '(1 2 3 8 e 4 7 6 5))
)

(defun get-direction (move)
  (car move))

(defun get-state (move)
(car (cdr move)))

(defun same-state (move1 move2)
  (equal (get-state move1) (get-state move2)))

;(same-state '(u (1 2 3 8 e 4 7 6 5)) '(d (1 2 3 8 e 4 7 6 5)))

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

(defun search-bfs (frontier)
  (cond 
    ;if options are empty, we OUT!
    ((null frontier) NIL)
    ;get state of first move of first path in frontier -> equal to goal state?
    ;then return the moveList of the first path of the frontier
    ((goal-state (get-state (first (first frontier))))  (path (first frontier)))
    ;otherwise
    (t (search-bfs (append (cdr frontier) (extend-path (first frontier )))))
  )
)

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
    (t (search-dfs-fd (append (extend-path (first frontier )) (cdr frontier)) depthbound))
  )
)

;curdepth will start at 1
(defun search-id (frontier &optional (curdepth 1))
  (cond 
    ((null frontier) NIL)
    ;if search-dfs did not find goal path, try again at higher depth limit
    ((null (search-dfs-fd frontier curdepth)) (search-id frontier (+ curdepth 1)))
    ;else it was found, so return it 
    (t (search-dfs-fd frontier curdepth))
  )
)

(defun sss (state &key (type 'BFS) (depth 7))
  (cond
    ((goal-state state) NIL)
    ((equal type 'BFS) (search-bfs (make-open-init state)))
    ((equal type 'DFS) (search-dfs-fd (make-open-init state) depth))
    ((equal type 'ID)  (search-id (make-open-init state)))
    (t NIL)
  )
)

(defun make-open-init (state)
  (list (list (cons NIL (cons state NIL))))
)

(defun extend-path (path) 
  (mapcar #'(lambda (x) (cons x path)) (remove-redundant (moves (get-state (first path))) path))
)

;(extend-path (first (make-open-init '(2 8 3 1 6 4 7 e 5))))
;(extend-path '((NIL (2 8 3 1 6 4 7 e 5)) (U (2 8 3 1 E 4 7 6 5))))
;(extend-path '((U (2 8 3 1 e 4 7 6 5)) (NIL (2 8 3 1 6 4 7 e 5))))

;(search-bfs (make-open-init '(2 8 3 1 6 4 7 e 5)))
;(search-dfs-fd (make-open-init '(2 8 3 1 6 4 7 e 5)) 7)
;(search-id (make-open-init '(2 8 3 1 6 4 7 e 5)))
;(search-id (make-open-init '(2 8 1 e 6 3 7 5 4)))
;(search-id (make-open-init '(3 1 2 e 8 4 7 6 5)))
;(search-id (make-open-init '(7 1 3 e 8 4 6 5 2)))