(define stop-names (list "8" "4" "lute"))

;; stops are initialized to off
(define stops (make-vector (length stop-names) #f))

;; return a list; 1 means on, 0 off
(define stops-status
  (lambda ()
    (map (lambda (e) (if e 1 0)) (vector->list stops))))

(define flip-stop
  (lambda (stop-number)
    (let ((i (inexact->exact stop-number)))
      (vector-set! stops i (not (vector-ref stops i))))
    (stops-status)))
