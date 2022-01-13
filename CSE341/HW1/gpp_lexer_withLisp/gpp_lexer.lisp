#|
	CSE341 - PROGRAMMING LANGUAGES
	ASSIGNMENT1 - G++ LEXER
	AUTHOR: GÖKBEY GAZİ KESKİN
	1901042631
|#


;TOKEN CONSTANTS
(defconstant op_plus "+")
(defconstant op_minus "-")
(defconstant op_mult "*")
(defconstant op_dblmult "**")
(defconstant op_div "/")
(defconstant op_oparan "(")
(defconstant op_cparan ")")
(defconstant op_comma ",")
(defconstant op_comment ";")
(defconstant op_str "\"")
(defconstant kw_true "true")
(defconstant kw_false "false")
(defconstant kw_and "and")
(defconstant kw_or "or")
(defconstant kw_not "not")
(defconstant kw_equal "equal")
(defconstant kw_append "append")
(defconstant kw_less "less")
(defconstant kw_nil "nil")
(defconstant kw_list "list")
(defconstant kw_concat "concat")
(defconstant kw_set "set")
(defconstant kw_deffun "deffun")
(defconstant kw_for "for")
(defconstant kw_while "while")
(defconstant kw_if "if")
(defconstant kw_exit "exit")
(defconstant kw_load "load")
(defconstant kw_disp "disp")

(defvar inputFile "input.gpp"); name of the input file
(defvar valueFlag 0);1 if the current char is an unsigned int or float
(defvar opFlag 0); 1 if the current char is an operator
(defvar identifierFlag 0); 1 if the next char is an identifier
(defvar commentFlag 0) ;1 if the current char is ; (comment token) until the newline
(defvar stringFlag 0) ;  one when currChar is " for the first time. 0 for the second time 
(defvar temp ""); temporary token
(defvar opTemp ""); temporary token for operators
(defvar tokenList ""); All the tokens

;;;MAIN FUNCTION
(defun gppinterpreter (filename)
  (with-open-file (stream filename) ;read the file char by char and call the specifiy function with currentChar
     (loop :for currChar := (read-char stream nil) :while currChar :collect 
      	(specifiy currChar) 
     )
  		
  )	
  	
	tokenList ;return value	
)

#|
Specifies the current char
if it is an operator ("+", "-", "/", ")",...) directly calls the addToken function
if it is the * operator waits until the next char to specifiy if it is * or ** and then calls addToken function
if it is a identifier, keyword, or value reads until one of the next operator, space, newline or tab then calls addToken function 
|#
(defun specifiy (currChar)
	(cond
		;;;COMMENT EVALUATION
		((string-equal currChar op_comment) ;if the current char is ; lexer shouldn't tokenize until the newline.
			(setf commentFlag 1 ))
		((char= currChar #\Newline) ;token is done. add to tokenList. Also, comment is done too.
			(setf commentFlag 0)
			(addToken temp))

				;;;" " STRING EVALUATION
		((and (eq commentFlag 0) (eq stringFlag 0)(string-equal currChar op_str)) ;if the current char is " put everything in temp until the next ".
			(setf stringFlag 1 )
			(setf temp (concatenate 'string temp (list currChar))))
		((and (eq commentFlag 0) (eq stringFlag 1) (not(string-equal currChar op_str))) ;add everything to temp until the ending "
			(setf temp (concatenate 'string temp (list currChar))))
		((and (eq commentFlag 0) (eq stringFlag 1) (string-equal currChar op_str)) ;add ending " then add token to tokenList.
			(setf temp (concatenate 'string temp (list currChar)))
			(addToken temp))

		;;;ESCAPE CHAR EVALUATION (add temp token to token list when current char is an escape char )
		((and (eq commentFlag 0)(or (char= currChar #\Space ) (char= currChar #\Tab))) ;token is done. add to tokenList
			(addToken temp))



		#|for the special cases with operators without space (example: 5*5, func(a) a*7)|#
		
		;if there is a token in temp and current char is * operator, then add temp to token list and keep the operator in optemp.
		((and (eq commentFlag 0) (> (length temp) 0)(string-equal currChar op_mult)) 
			(addToken temp)
			(setf opTemp (concatenate 'string opTemp (list currChar))))
		;if opTemp is "*" and current char is not "*" then add them both to tokenList but seperately.
		((and (eq commentFlag 0) (string-equal opTemp op_mult) (not(string-equal currChar op_mult)))
			(setf opFlag 1)
			(addToken opTemp)
			(setf opTemp (concatenate 'string opTemp (list currChar)))
			(setf opFlag 1)
			(addToken opTemp))
		;if opTemp is "*" and current char is also "*" add them to tokenList as one token
		((and (eq commentFlag 0)(string-equal op_mult currChar)(string-equal op_mult opTemp))
			(setf opTemp (concatenate 'string opTemp (list currChar)))
			(setf opFlag 1)
			(setf identifierFlag 0)
			(addToken opTemp))
		;if there is a token in temp and current char is an operator other than "*"
		((and (eq commentFlag 0) (> (length temp) 0)(eq  (isOp currChar) 1)) 
			(addToken temp)
			(setf opTemp (concatenate 'string opTemp (list currChar)))
			(setf opFlag 1)
			(setf identifierFlag 0)
			(addToken opTemp))
			
		;for operator evaluation (except * and **)
		((and (eq commentFlag 0)(eq (isOp currChar) 1))
			(setf opTemp (concatenate 'string opTemp (list currChar)))
			(setf opFlag 1)
			(setf identifierFlag 0)
			(addToken opTemp))
		;if the current char is a character appends it to temp
 		((and (eq commentFlag 0)(alpha-char-p currChar))
			(setf temp (concatenate 'string temp (list currChar)))
			(setf identifierFlag 1)
 			(setf valueFlag 0))
		;if the current character is an int appends it to temp
	 	((and (eq commentFlag 0)(eq (isValue currChar) 1))
			(setf temp (concatenate 'string temp (list currChar)))
			(setf valueFlag 1))

	)
)

; adds given tokens to tokenList according to flags
(defun addToken (token) 
	(cond
		;;ERRORS
		((and (eq valueFlag 1) (digit-char-p (char temp 0)) (= identifierFlag 1)) ;if an identifier starts with digit
			(setf tokenList (concatenate 'string tokenList "TOKENIZATION ERROR (LEADING DIGIT)->" temp " | "))
			(setf temp "")
			(setf identifierFlag 0)
			(setf valueFlag 0))
		((and (eq valueFlag 1) (char-equal (char temp 0) (char "0" 0))) ;if a value starts with "0"
			(setf tokenList (concatenate 'string tokenList "TOKENIZATION ERROR (FIRST INDEX 0)->" temp " | "))
			(setf temp "")
			(setf valueFlag 0))
		((and (eq valueFlag 1) (char-equal (char temp 0) (char "." 0))) ;if a value starts with ".""
			(setf tokenList (concatenate 'string tokenList "TOKENIZATION ERROR (LEADING .)->" temp " | "))
			(setf temp "")
			(setf valueFlag 0))
		;;operators
		((and (= (length opTemp) 2) (string-equal opTemp op_dblmult)) ;**
			(setf tokenList (concatenate 'string tokenList "Operator->" opTemp " | "))
			(setf opTemp "")
			(setf opFlag 0))
		((eq opFlag 1) ;other operators
	 		(setf tokenList (concatenate 'string tokenList "Operator->" opTemp " | "))
	 		(setf opTemp "")
	 		(setf opFlag 0))
		;;keywords
		((string-equal token kw_true)
			(setf tokenList (concatenate 'string tokenList "keyword->true | "))
			(setf temp ""))

		((string-equal token kw_false)
			(setf tokenList (concatenate 'string tokenList "keyword->false | "))
			(setf temp ""))

		((string-equal token kw_and)
			(setf tokenList (concatenate 'string tokenList "keyword->and | "))
			(setf temp ""))
	
	
	 	((string-equal token kw_or)
			(setf tokenList (concatenate 'string tokenList "keyword->or | "))
			(setf temp ""))
	 	
	 	((string-equal token kw_not)
			(setf tokenList (concatenate 'string tokenList "keyword->not | "))
			(setf temp ""))

	 	((string-equal token kw_equal)
			(setf tokenList (concatenate 'string tokenList "keyword->equal | "))
			(setf temp ""))

	 	((string-equal token kw_append)
			(setf tokenList (concatenate 'string tokenList "keyword->append | "))
			(setf temp ""))
	 	((string-equal token kw_nil)
			(setf tokenList (concatenate 'string tokenList "keyword->nil | "))
			(setf temp ""))
	 	((string-equal token kw_list)
			(setf tokenList (concatenate 'string tokenList "keyword->list | "))
			(setf temp ""))
	 	((string-equal token kw_less)
			(setf tokenList (concatenate 'string tokenList "keyword->less | "))
			(setf temp ""))

	 	((string-equal token kw_concat)
			(setf tokenList (concatenate 'string tokenList "keyword->concat | "))
			(setf temp ""))

	 	((string-equal token kw_set)
			(setf tokenList (concatenate 'string tokenList "keyword->set | "))
			(setf temp ""))

	 	((string-equal token kw_deffun)
			(setf tokenList (concatenate 'string tokenList "keyword->deffun | "))
			(setf temp ""))

	 	((string-equal token kw_for)
			(setf tokenList (concatenate 'string tokenList "keyword->for | "))
			(setf temp ""))

	 	((string-equal token kw_while)
			(setf tokenList (concatenate 'string tokenList "keyword->while | "))
			(setf temp ""))

	 	((string-equal token kw_if)
			(setf tokenList (concatenate 'string tokenList "keyword->if | "))
			(setf temp ""))

	 	((string-equal token kw_load)
			(setf tokenList (concatenate 'string tokenList "keyword->load | "))
			(setf temp ""))

	 	((string-equal token kw_disp)
			(setf tokenList (concatenate 'string tokenList "keyword->disp | "))
			(setf temp ""))

	 	((string-equal token kw_exit)
			(setf tokenList (concatenate 'string tokenList "keyword->exit | "))
			(setf temp ""))
	 	;;IDENTIFIER
	 	((eq identifierFlag 1)
	 		(setf tokenList (concatenate 'string tokenList "identifier->" temp " | "))
	 		(setf temp "")
	 		(setf identifierFlag 0)
	 		(setf valueFlag 0))
	 	;;VALUES
	 	((and (eq valueFlag 1)(includeDot temp))
	 		(setf tokenList (concatenate 'string tokenList "value(unsignedReal)->" temp " | "))
	 		(setf temp "")
	 		(setf valueFlag 0))
	 	((eq valueFlag 1)
	 		(setf tokenList (concatenate 'string tokenList "value(unsignedInt)->" temp " | "))
	 		(setf temp "")
	 		(setf valueFlag 0))
	 	;;STRING
	 	((eq stringFlag 1)
	 		(setf tokenList (concatenate 'string tokenList "String->" temp " | "))
	 		(setf temp "")
	 		(setf stringFlag 0))
	 	(t  (setf temp "") 
	 		(setf opTemp ""))
	)
)

;returns 1 if the given string is an operator
(defun isOp (var)
	(cond
		( (string-equal var "(")1)
		( (string-equal var ")")1)
		( (string-equal var "+")1)
		( (string-equal var "-")1)
		( (string-equal var "*")1)
		( (string-equal var "**")1)
		( (string-equal var "/")1)
		( (string-equal var ";")1)
		( (string-equal var "\"")1)
		( (string-equal var ",")1)
		(t 0)
	)
)
;returns 1 if the given digit is part of a value (unsigned int or float)
(defun isValue (var)
	(cond 
		( (string-equal var "0")1)
		( (string-equal var "1")1)
		( (string-equal var "2")1)
		( (string-equal var "3")1)
		( (string-equal var "4")1)
		( (string-equal var "5")1)
		( (string-equal var "6")1)
		( (string-equal var "7")1)
		( (string-equal var "8")1)
		( (string-equal var "9")1)
		( (string-equal var ".")1)
		(t 0)

	)
)

;recursive function to determine if the number is a value(unsigned int or float)
(defun includeDot (var)
  (cond
   ((=(length var) 0) nil ) ;base case. if the var's size is zero stop recursing
   ((string= "." (subseq var 0 1)) ".") ; var starts with "."
   (t (includeDot (subseq var 1))))) ; otherwise shorten var by 1 and start over


(format t "~a" (gppinterpreter inputFile)) ;;call the lexer with input file and print the return value (tokenList)
