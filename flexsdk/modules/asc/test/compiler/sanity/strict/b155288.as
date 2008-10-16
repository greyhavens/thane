// [31Aug07:rickr] lowered original test N value from 10000 to 100 since qvm was taking too long 
var N:int = 100
var a = []
for(var i:int=0; i<N; i++) a.push(i+"salt")
a.sort(Array.CASEINSENSITIVE)

// now allocate some non Strings
for(var i:int=0; i<N; i++) new QName("asdf", i)

//touch each string
for(var i:int=0; i<N; i++) a[i].toUpperCase()
