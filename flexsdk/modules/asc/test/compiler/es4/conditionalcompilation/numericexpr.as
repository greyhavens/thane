package {
  CONFIG const val1=1
  CONFIG const val2=0
  CONFIG const val3=-50
  CONFIG const val4=111
  CONFIG const val5=10

  CONFIG const plus1=CONFIG::val1+CONFIG::val2
  CONFIG const plus2=CONFIG::val3+CONFIG::val4+CONFIG::val1+CONFIG::val2

  CONFIG const minus1=CONFIG::val1-CONFIG::val2
  CONFIG const minus2=CONFIG::val3-CONFIG::val4-CONFIG::val1-CONFIG::val2

  CONFIG const times1=CONFIG::val3*CONFIG::val4
  CONFIG const times2=CONFIG::val2*CONFIG::val1

  CONFIG const div1=CONFIG::val1/CONFIG::val2
  CONFIG const div2=CONFIG::val4/CONFIG::val3
 
  CONFIG const mod1=CONFIG::val4%CONFIG::val5

  CONFIG const lt1=CONFIG::val4<CONFIG::val3
  CONFIG const lt2=CONFIG::val3<CONFIG::val4

  CONFIG const le1=CONFIG::val4<=CONFIG::val3
  CONFIG const le2=CONFIG::val4<=CONFIG::val4

  CONFIG const gt1=CONFIG::val4>CONFIG::val3
  CONFIG const gt2=CONFIG::val3>CONFIG::val4

  CONFIG const ge1=CONFIG::val4>=CONFIG::val3
  CONFIG const ge2=CONFIG::val4>=CONFIG::val4

  CONFIG const ne1=CONFIG::val4!=CONFIG::val3
  CONFIG const ne2=CONFIG::val4!=CONFIG::val4

  CONFIG const eq1=CONFIG::val4==CONFIG::val3
  CONFIG const eq2=CONFIG::val4==CONFIG::val4

  CONFIG const shl1=15 << 1
  CONFIG const shr1=15 >> 1
  CONFIG const shrl1=15 >>> 1

  CONFIG const bitand1=15 & 7
  CONFIG const bitor1=15 | 0xFF
  CONFIG const bitxor1=15 ^ 0xFF

  print("plus1="+CONFIG::plus1)
  print("plus2="+CONFIG::plus2)
  print("minus1="+CONFIG::minus1)
  print("minus2="+CONFIG::minus2)
  print("times1="+CONFIG::times1)
  print("times2="+CONFIG::times2)
  print("div1="+CONFIG::div1)
  print("div2="+CONFIG::div2)
  print("mod1="+CONFIG::mod1)
  print("lt1="+CONFIG::lt1)
  print("lt2="+CONFIG::lt2)
  print("le1="+CONFIG::le1)
  print("le2="+CONFIG::le2)
  print("gt1="+CONFIG::gt1)
  print("gt2="+CONFIG::gt2)
  print("ge1="+CONFIG::ge1)
  print("ge2="+CONFIG::ge2)
  print("eq1="+CONFIG::eq1)
  print("eq2="+CONFIG::eq2)
  print("ne1="+CONFIG::ne1)
  print("ne2="+CONFIG::ne2)
  print("shl1="+CONFIG::shl1)
  print("actual:"+(15<<1))
  print("shr1="+CONFIG::shr1)
  print("actual:"+(15>>1))
  print("shrl1="+CONFIG::shrl1)
  print("actual:"+(15>>>1))
  print("bitand1="+CONFIG::bitand1);
  print("bitor1="+CONFIG::bitor1);
  print("bitxor1="+CONFIG::bitxor1);
}