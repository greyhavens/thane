function f() {
print('f');
return true;
}
var x = 4;
var p = "x";
print(this[p]);
this[p] = 5;
print(this[p]);
this['x'] = 6;
print(this[p]);
this['f']();