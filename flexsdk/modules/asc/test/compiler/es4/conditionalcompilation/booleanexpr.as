package {
  CONFIG const int1=10
  CONFIG const int0=0
  CONFIG const bool1=true
  CONFIG const bool2=false
  CONFIG const str1=""
  CONFIG const str2="notempty"

  CONFIG const not1=!CONFIG::bool2
  CONFIG const not2=!true
  CONFIG const not3=!CONFIG::int0
  CONFIG const not4=!CONFIG::int1
  CONFIG const not5=!CONFIG::str1
  CONFIG const not6=!CONFIG::str2

  CONFIG const orcond1=CONFIG::bool1 || CONFIG::bool2
  CONFIG const orcond2=false || CONFIG::bool2
  CONFIG const orcond3=0 || CONFIG::int1
  CONFIG const orcond4=CONFIG::int0 || 0
  CONFIG const orcond5=CONFIG::str1 || CONFIG::str2
  CONFIG const orcond6=CONFIG::str1 || CONFIG::str1

  CONFIG const and1=CONFIG::bool1 && CONFIG::bool1
  CONFIG const and2=CONFIG::bool1 && CONFIG::bool2
  CONFIG const and3=CONFIG::int1 && -111
  CONFIG const and4=CONFIG::int0 && 0
  CONFIG const and5=CONFIG::str2 && CONFIG::str2
  CONFIG const and6=CONFIG::str2 && CONFIG::str1

  CONFIG const ne1=CONFIG::bool1 != true
  CONFIG const ne2=CONFIG::bool2 != true
  CONFIG const ne3=CONFIG::int1 != 10
  CONFIG const ne4=CONFIG::int1 != 9
  CONFIG const ne5=CONFIG::str1 != ""
  CONFIG const ne6=CONFIG::str2 != ""

  CONFIG const eq1=CONFIG::bool1 == true
  CONFIG const eq2=CONFIG::bool2 == true
  CONFIG const eq3=CONFIG::int1 == 10
  CONFIG const eq4=CONFIG::int1 == 9
  CONFIG const eq5=CONFIG::str1 == ""
  CONFIG const eq6=CONFIG::str2 == ""

  CONFIG const expr1= ( true == CONFIG::bool1 && 5 == false )|| true;

  print("not1="+CONFIG::not1)
  print("not2="+CONFIG::not2)
  print("not3="+CONFIG::not3)
  print("not4="+CONFIG::not4)
  print("not5="+CONFIG::not5)
  print("not6="+CONFIG::not6)
  print("orcond1="+CONFIG::orcond1)
  print("orcond2="+CONFIG::orcond2)
  print("orcond3="+CONFIG::orcond3)
  print("orcond4="+CONFIG::orcond4)
  print("orcond5="+CONFIG::orcond5)
  print("orcond6="+CONFIG::orcond6)
  print("and1="+CONFIG::and1)
  print("and2="+CONFIG::and2)
  print("and3="+CONFIG::and3)
  print("and4="+CONFIG::and4)
  print("and5="+CONFIG::and5)
  print("and6="+CONFIG::and6)
  print("ne1="+CONFIG::ne1)
  print("ne2="+CONFIG::ne2)
  print("ne3="+CONFIG::ne3)
  print("ne4="+CONFIG::ne4)
  print("ne5="+CONFIG::ne5)
  print("ne6="+CONFIG::ne6)
  print("eq1="+CONFIG::eq1)
  print("eq2="+CONFIG::eq2)
  print("eq3="+CONFIG::eq3)
  print("eq4="+CONFIG::eq4)
  print("eq5="+CONFIG::eq5)
  print("eq6="+CONFIG::eq6)
  print("expr1="+CONFIG::expr1)
}