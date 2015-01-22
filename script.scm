(define print-hello
  (lambda (a b)
    ;; (string-concatenate
    ;;  (list
    ;;   "the product of "
    ;;   (number->string a)
    ;;   " and "
    ;;   (number->string b)
    ;;   " is "
    ;;   (number->string (* a b))))
    (/ (+ a b) 2)))


(define flululu
  (lambda (a b)
    (string-concatenate
     (list a b))))

(define sayhi
  (lambda ()
    (* 10 20)))

(define thiscrashes
  (lambda (a b)
    (display (string-append (number->string (+ a b)) "\n"))
    asdlfksjscdhf))

(define catch-thiscrashes
  (lambda ()
    (catch #t
      (lambda () (thiscrashes 3 4))
      (lambda (key . parameters)
	(display "noooo!!\n")))))

(catch-thiscrashes)
