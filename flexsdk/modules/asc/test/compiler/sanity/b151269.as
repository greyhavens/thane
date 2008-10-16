var classes =
[Error,
DefinitionError,
EvalError,
RangeError,
ReferenceError,
SecurityError,
SyntaxError,
TypeError,
URIError,
VerifyError,
UninitializedError,
 ArgumentError];

for (var name in classes)
{
	var c = classes[name];
	print (new c("created with new"));
	print (c("created with call"));
}
