  print("print CONFIG::flag="+CONFIG::flag)
  CONFIG::flag {
    print("statement block: CONFIG::flag==true")
  }
  if (CONFIG::flag) {
    print("conditional: CONFIG::flag==true")
  } else {
    print("conditional: CONFIG::flag==false")
  }
