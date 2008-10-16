thisError = "no error";
thisError1="no error";
try{
throw new ReferenceError();

}catch(eo:ReferenceError){
thisError1 = "This is outer reference error:"+" "+eo.toString();

}finally{

	trace("finally begin");
try {
throw new TypeError();

}catch(ei5:TypeError){
thisError="This is inner Security Error!!!"+ei5.toString();

}
	trace("finally end");
	
}
