var x = <><a><b>hello</b></a><c><b>goodbye</b></c></>
var s = x.(b=="hello"||b=="goodbye")
print(s)