(define print-hello
  (lambda (a b)
    (string-concatenate
     (list
      "the sum of "
      (number->string a)
      " and "
      (number->string b)
      " is "
      (number->string (+ a b))))))
