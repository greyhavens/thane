var elemChildren:XMLList = elem.localized;
var langArray:Array = new Array();
for each ( var child:XML in elemChildren )
{
langArray.push( child.@lang.toString() );
}

// Retrieve the name from the first element in the array
// or the first child of elem
if ( langArray.length > 0 )
{
return elemChildren.(@lang == langArray[0])[0].toString();
} 
