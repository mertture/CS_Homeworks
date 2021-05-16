(define (checkOperatorsTwo expr)
	(if (null? expr)
	#f
		(if (number? expr)
		#f
			(if (and (number? (car expr)) (null? (cdr expr)))
			#t
			(if (and (pair? (car expr)) (null? (cdr expr)))
				(checkOperators (car expr))
				(if (and (number? (car expr)) (and (not (eqv? '- (cadr expr))) (not (eqv? '+ (cadr expr))) (not (eqv? '/ (cadr expr))) (not (eqv? '* (cadr expr)))))
				(checkOperators (cddr expr))
					(if (and (pair? (car expr))   (and (not (eqv? '- (cadr expr))) (not (eqv? '+ (cadr expr))) (not (eqv? '/ (cadr expr))) (not (eqv? '* (cadr expr)))))
					(and (checkOperators (car expr)) (checkOperators (cddr expr)))
					#f)))))))

(define (twoOperatorCalculator expr)
		(if (null? expr)
		0
			(if (and (pair? (cdr expr)) (and (number? (car expr)) (and (not (eqv? '+ (cadr expr))) (not (eqv? '- (cadr expr))))))
			(display "")
				(if (eqv? '- (car expr))
				(twoOperatorCalculator (cons (-(cadr expr)) (cddr expr)))
					(if (eqv? '+ (car expr))
					(twoOperatorCalculator (cdr expr))
						(if (and (pair? (cdr expr)) (and (number? (car expr)) (number? (cadr expr))))
						(display "")
						(+ (car expr) (twoOperatorCalculator (cdr expr)))))))))


(define (fourOperatorCalculator expr)
	(if (null? (cdr expr))
	expr
		(if (and (pair? (cdr expr)) (and (number? (car expr)) (and (not (eqv? '+ (cadr expr))) (not (eqv? '- (cadr expr))) (not (eqv? '/ (cadr expr))) (not (eqv? '* (cadr expr))))))
		(display "")
			(if (eqv? '* (cadr expr))
			(fourOperatorCalculator (cons (* (car expr) (caddr expr)) (cdddr expr)))
				(if (eqv? '/ (cadr expr))
				(fourOperatorCalculator (cons (/ (car expr) (caddr expr)) (cdddr expr)))
				(cons (car expr) (fourOperatorCalculator (cdr expr))))))))


(define NestedLists (lambda (x)
	(if (pair? x)
	(twoOperatorCalculator
		(fourOperatorCalculator 
			(calculatorNested x))) 
		x)))

(define calculatorNested (lambda (expr) (map NestedLists expr)))



(define (checkOperators expr)
	(if (null? expr)
	#f
		(if (number? expr)
		#f
			(if (and (number? (car expr)) (null? (cdr expr)))
			#t
			(if (and (pair? (car expr)) (null? (cdr expr)))
				(checkOperators (car expr))
				(if (and (number? (car expr)) (or (eqv? '- (cadr expr)) (eqv? '+ (cadr expr)) (eqv? '/ (cadr expr)) (eqv? '* (cadr expr))))
				(checkOperators (cddr expr))
					(if (and (pair? (car expr))   (or (eqv? '- (cadr expr)) (eqv? '+ (cadr expr)) (eqv? '/ (cadr expr)) (eqv? '* (cadr expr))))
					(and (checkOperators (car expr)) (checkOperators (cddr expr)))
					#f)))))))


(define (calculator expr)
	(if (checkoperators expr)
	(twoOperatorCalculator (fourOperatorCalculator (calculatorNested expr)))
	#f))
