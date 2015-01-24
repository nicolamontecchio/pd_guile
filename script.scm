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

(define printsomething
  (lambda ()
    (display "hello")))

(define thiscrashes
  (lambda (a b)
    (+ a b)
    ;; asd
    ;; (display "asd\n")
    ))

(define any-to-string
  (lambda (s)
    (if (symbol? s) (symbol->string s) s)))

(define outlist
  (lambda ()
    '(1 2 3)))

(define catch-thiscrashes
  (lambda ()
    (catch #t
      (lambda () (thiscrashes 3 4))
      (lambda (key . parameters)
	(display (string-append "ERROR: " (symbol->string key) "\n"))
	;; (display (string-append
	;; 	  (string-concatenate (map any-to-string parameters))
	;; 	  "\n"))
	))))

;; (catch-thiscrashes)
