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

(define float
  (lambda (f)
    f * f))

(define sayhi
  (lambda ()
    (* 10 20)))

(define printsomething
  (lambda ()
    (display "hello")))

(define thiscrashes
  (lambda (a b)
    (post "this is a debug message that should appear in the PD window")
    (+ a b)
    (display "asd")
    sad))

(define cwd
  (lambda ()
    (post (getcwd))))

(define any-to-string
  (lambda (s)
    (if (symbol? s) (symbol->string s) s)))

(define outlist
  (lambda ()
    '(1 2 3)))

(define badoutput
  (lambda ()
    '('(1 2 3) '(3 5))))


(define catch-thiscrashes
  (lambda ()
    (catch #t
      (lambda () (thiscrashes 3 4))
      (lambda (key . parameters)
	(display
	 (string-append "ERROR: " (symbol->string key) "\n"))))))

;; (catch-thiscrashes)
