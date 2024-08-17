/* 
Define the variable `func` as a function that receives two arguments: args, body. 
*/

(def {func} {lambda {args body} {def (head args) (lambda (tail args) body)})

/* When using `func`, the first value on `args` is the function name. */
(def {add x y} {+ x y})

/* You can use these functions like: */
add 10 20 // should return 30
