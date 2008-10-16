while (true){
try{
throw new TypeError();
break;
}catch(e:TypeError){
thisError=e.toString();
print("Type Error:"+thisError);
break;

}
}
