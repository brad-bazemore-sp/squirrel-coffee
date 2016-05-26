# Squirrel Coffee
![alt text] (https://github.com/sonyccd/Squirrel-Coffee/blob/master/sq_coffee.png "Logo")  


##Squirrel Coffee language
This langue is very similar to java but I will pretend it is not and start from the beginning.  

### Data types
* Integer
* Float

Only two primitive data types: int and float which can be used as 
types of variables, parameters, and method return types.

###Expressions

####Binary arithmetic operators
* +
* −
* *
* /

The additive operators + and − have a lower precedence than multiplicative operators * and /.  
In addition, + and − may be used as unary operators which have the highest precedence.  
  
As in Java, the parenthesis may be used to alter the default precedence of operators.  
Values represented by expressions have types.  Given int arguments, each arithmetic operation returns a result of type int.  
If at least one of the arguments is of type float, the result is always float.  
In case of mixed type arguments, the int argument is automatically converted to float before the operation is performed.  
A method invocation represents a value consistent with the return type of the method.  
A void method invocation may not be used in an expression.  
Explicit casts may be used to convert the type from int to float and from float to int, if needed.

####Binary relational operators:
* <
* <=
* ==
* !=
* >=
* >

Types are handled as above. The equality operators (== and !=) have lower precedence than the other relational operators.  
Relational operators can only be applied to numeric operands (expressions of type int or float). Each relational operator returns a boolean result.  
All of the operators (arithmetic and relational) can only be applied to numeric arguments.  
For example, it is not legal to compare two relational expressions for equality, even though Squirrel Coffee syntax allows it.  
Please note that according the Squirrel Coffee’s syntax, an expression may be composed of a StringLiteral.  
In this case, the type of the expression is String.  However, a String expression cannot be used as an operand to any of the arithmetic or relational operators.  
Squirrel Coffee does not allow method overloading.  Consequently, for every method invocation, there should be a single method declaration matching it.  
A method invocation matches a declaration if the invocation has the same number of arguments as the number of formal parameters in the corresponding method declaration.  
Furthermore, the type of the actual argument expression and the corresponding formal parameter must be the same or convertible.  
If the actual argument expression of type int is provided in place of a formal parameter of type float, an automatic conversion of the actual argument expression to (wider) type float is performed before the call.  
An explicit type cast must be used to pass a float actual argument expression in place of a formal parameter of type int.  
Parameters of primitive types (int and float) are passed by value.  String type parameters are passed by reference (see the SimpleIO class below).

###Statements

####Statements:
* assignment statement
* return statement
* while statement
* if statement
* method invocation statement
* block statement
* empty statement 

The semantics of each statement is the same as in Java.  
The type of a variable, parameter, or field used on the left hand side of an assignment statement must be the same as that of the right hand side expression.  
In addition, if the left hand side is of type float and the expression of type int, an automatic conversion of the expression to float is performed.  
An explicit type cast must be used if a float expression is assigned to an int left hand side.  
The expression used as part of the return statement must be of the same type as the return type of the method in which the return statement is used.  
However, if the method return type is float and the expression is of type int, an automatic conversion of the expression to float is performed.  
An explicit cast must be used if a float expression is returned from a method with the int return type.  
The expression used in the if and while statements must be of type boolean.  That is, in Squirrel Coffee, it must be a relational expression.  
The method invocation statement may invoke a void or a non-void method.  
Unlike in Java, the block statement in Squirrel Coffee may not contain local declarations (only executable statements are allowed).

### Classes

A Squirrel Coffee program must be composed of only a single public class.  All fields and methods declared in the class must be static and have package visibility (private, protected, and public access qualifiers are not allowed).  In addition, all field declarations must include their initializations.  Furthermore, there must be a single public void main method, which must have an array of Strings as the only argument.    
Squirrel Coffee follows the scoping rules of Java.  However, package declarations and class imports are not permitted.  Consequently, the only Squirrel Coffee class is placed in the default package.
A Squirrel Coffee method may include local variable declarations, which must be placed ahead of any executable statements.  All local variable declarations must include their initializations.  Methods may be recursive, i.e. a method may invoke itself.  Every method must have a single return statement which must be the last statement in the method body (this requirement is already captured by the Squirrel Coffee grammar above).
The Squirrel Coffee system includes a class library including just one class, called SimpleIO, described below.
### SimpleIO
The SimpleIO class offers a number of static methods to conveniently perform simple reading and writing of Squirrel Coffee data.   Reading is from the standard input and writing is to standard output.  The meaning of the SimpleIO’s methods is self explanatory.  The read methods read and return a single value of the requested type.  If a requested int or float value cannot be read (the data does not exist, or it cannot be interpreted as an int or a float), an error message is printed on the error stream and 0 or 0.0f is returned, respectively.  The print methods write a single value on the standard output.  The println method accepts no arguments and writes the end of line.  Note that printString may write a string literal with a newline (\n) in it, effectively printing the end of line, as well.
