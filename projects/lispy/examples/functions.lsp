/* 
Define the variable `func` as a function that receives two arguments: args, body. 
*/

(def {func} {lambda {args body} {def (head args) (lambda (tail args) body)})

/* When using `func`, the first value on `args` is the function name. */
(def {add x y} {+ x y})

