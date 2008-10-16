package {
 CONFIG const str1="value1"
 CONFIG const str2=""
 CONFIG const str3="value2"
 CONFIG const str4="value1"

 CONFIG const plus1=CONFIG::str1+" and "+CONFIG::str2+CONFIG::str3
 CONFIG const plus2=CONFIG::str2+CONFIG::str2+""
 CONFIG const ne1=CONFIG::str2!=CONFIG::str1
 CONFIG const ne2=CONFIG::str1!=CONFIG::str4
 CONFIG const eq1=CONFIG::str2==CONFIG::str1
 CONFIG const eq2=CONFIG::str1==CONFIG::str4

 print("plus1="+CONFIG::plus1)
 print("plus2="+CONFIG::plus2)
 print("ne1="+CONFIG::ne1)
 print("ne2="+CONFIG::ne2)
 print("eq1="+CONFIG::eq1)
 print("eq2="+CONFIG::eq2)
}