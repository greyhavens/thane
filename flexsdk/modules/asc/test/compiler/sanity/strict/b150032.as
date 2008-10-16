var a:Boolean = true
var a1:Boolean = false
var result1 = false;
var result2 = false;

outer: {
inner:{
if (a==true){
break inner;
}
result1=true;
if (a1==true){
break outer;
}

}
result2=true
}
print(result1);
print(result2);






