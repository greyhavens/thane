/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/**
   File Name:    nested.as
   Description:  tests nested Vectors: Vector.<Vector.<int>>
   Author:       dschaffe@adobe.com 5-Nov-2007
   *
   */
var SECTION="";
var VERSION = "ECMA_1";

startTest();

writeHeaderToLog( " nested Vector");

var v1:Vector.<Vector.<int>>=new Vector.<Vector.<int>>();
var v2:Vector.<int>;
v2=new Vector.<int>;v2[0]=0;v2[1]=1;
v1.push(v2);
v2=new Vector.<int>;v2[0]=2;v2[1]=3;
v1.push(v2);
v2=new Vector.<int>;v2[0]=4;v2[1]=5;
v1.push(v2);
AddTestCase("push nested vector.<int>",
            "0,1,2,3,4,5",
            v1.toString());

var v3:Vector.<Vector.<String>>=new Vector.<Vector.<String>>();
var v4:Vector.<String>;
v4=new Vector.<String>;v4[0]='one';v4[1]='two';
v3.push(v4);
v4=new Vector.<String>;v4[0]='three';v4[1]='four';
v3.push(v4);
v4=new Vector.<String>;v4[0]='five';v4[1]='six';
v3.push(v4);
AddTestCase("push nested vector.<String>",
            "one,two,three,four,five,six",
            v3.toString());

var v5:Vector.<Vector.<Vector.<int>>>=new Vector.<Vector.<Vector.<int>>>();
var v6:Vector.<Vector.<int>>;
var v7:Vector.<int>;
v7=new Vector.<int>();v7[0]=0;v7[1]=1;
v6=new Vector.<Vector.<int>>();
v6.push(v7);
v5.push(v6);
v7=new Vector.<int>();v7[0]=2;v7[1]=3;
v6=new Vector.<Vector.<int>>();
v6.push(v7);
v7=new Vector.<int>();v7[0]=4;v7[1]=5;
v6.push(v7);
v5.push(v6);

AddTestCase("push nested vector.<vector.<int>>",
            "0,1,2,3,4,5",
            v5.toString());

class tree {
  var value:String;
  var left:tree;
  var right:tree;
  function tree(value:String,left:tree,right:tree){ 
    this.value=value;
    this.left=left;
    this.right=right;
  }
  static function depthfirst(t:tree):String {
    var out="";
    out+=t.value;
    if (t.left!=undefined) out+=","+depthfirst(t.left);
    if (t.right!=undefined) out+=","+depthfirst(t.right);
    return out;
  }
  static function collect(t:tree):Vector.<tree> {
    var out:Vector.<tree>=new Vector.<tree>();
    out.push(t);
    if (t.left!=undefined) out=out.concat(collect(t.left));
    if (t.right!=undefined) out=out.concat(collect(t.right));
    return out;
  }
  static function printlist(l:Vector.<tree>):String {
    var ret:String="";
    for (var i:int=0;i<l.length;i++) {
      ret+=l[i].value;
      if (i<l.length-1) ret+=",";
    }
    return ret;
  }
} 
var two=new tree("two",undefined,undefined);
var three=new tree("three",undefined,undefined);
var one=new tree("one",two,three);
var six=new tree("six",undefined,undefined);
var seven=new tree("seven",undefined,undefined);
var five=new tree("five",six,seven);
var four=new tree("four",five,undefined);
var root=new tree("root",one,four);

AddTestCase("test vector of custom classes",
           tree.printlist(tree.collect(root)).toString(),
           "root,one,two,three,four,five,six,seven");
test();
