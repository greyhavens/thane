/**
 *	
 *  	Warning Message: The order in which properties of an object are iterated over within a
 *			 '(for x in target)' statement is random in AS3. If you experience odd
 *			 ordering behavior, inspect these loops to determine if this change in
 *			 ordering may affect your code
 */

package Warnings {

	var myObject:Object = {name:"Tara", age:27, city:"San Francisco"};
	
	var name:String;
	
	for (name in myObject) {}

		
}

