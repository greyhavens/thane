package {
  var a=CONFIG::a
  var b:Array=[1,2,CONFIG::a,3,4,5]
  var c:Array=[1,2,CONFIG::b 3,4,5,6]
  var d={a:1,b:CONFIG::a}
  var e={a:1,CONFIG::b b:2}
  print(a)
  print(b)
  print(c)
  print(d['b'])
  print(e['b'])
}
