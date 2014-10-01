(define stop-names (list "8" "4" "lute"))

;; stops are initialized to off
(define stops (make-vector (length stop-names) #f))

(define stops-status
  (lambda ()
    (map (lambda (e) (if e 1 0)) (vector->list stops))))

(define flip-stop
  (lambda (stop-number)
    (vector-set!
     stops
     stop-number
     (not (vector-ref stops stop-number)))
    ;; stops
    ;; 3
    ;; 4
    (stops-status)))
