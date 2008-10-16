package {
  config namespace NS1
  class C {
    public namespace NS1
    NS1 var v1=10;
    var v1=5;
    function getv():int { return NS::v1 }
  }
}