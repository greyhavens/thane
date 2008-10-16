LabelTest( { p1:"hi,", p2:" norris" }, "hi, norris", " norrishi," );

function LabelTest( object, expect1, expect2 ) {
result = "";
yoohoo:
{ for ( property in object )
{ result += object[property]; }; break; yoohoo;


};
}