print(this==this)
function a(o) { print(this==o) }
a(this)					//[object global]
function b(o) { var x:Function = a; print(this==o); x(o); }
b(this)					//[object global][object global], when this==null, so this==global
function c(o) { function d(o) { print(this==o); } d(o) }
c(this)					//[object global]
var o = new Object
o.e=b
o.e(o)				//[object Object][object global]
o.f=a
o.f(o)				//[object Object]
