x=10
y=20
a=10
b=20
c=30
switch(x) {
	case a:
	switch(y) {
		case b:
		default:
		print("inner switch");
		break;
	}
	while(true) {
		print("while");
		break;
	}
	print("outer switch");
	true;
	break;
	case b:
	'b';
	break;
	default:
	'default';
	break;
	case c:
	'c';
	break;
}

function f(x) {
var c='test';
var r;

switch(x)
{
case 1:
    r='case one';
    break;
case "foo":
    r='case foo';
    break;
case c:
    r='case '+c;
    break;
default:
    r='default';
    break;
}

print(r);
}
f(1)
f('foo')
f('test')
f(0)
