 var v:Vector.<int> = new Vector.<int>();
v[0] = 15;
v[1] = 54;
v[2] = 77;
v[3] = 8;
v[4] = 55;
v[5] = 89;
v[6] = 1;
v[7] = 43;
v[8] = 3;
v[9] = 0x998899;

var result:Vector.<int> = v.slice(3, 7);
for (var i:uint = 0; i < result.length; i++)
{
  //trace("result[" + i + "] =", result[i]);
}
