
// cn: This tests RegExp handling of double byte whitespace characters (testSpace())
//      as well as matching the location between double byte characters (testsplit())
//     The unicode whitespace chars are:
//
// #define UNICODE_NOBREAK_SPACE 0x00A0				// - no-break space
// #define UNICODE_ZEROWIDTH_SPACE 0x200B			// - zero width space
// #define UNICODE_WORDJOINER_SPACE 0x2060			// - word joiner  NOTE: [CN] this char is not a recognized character under jdk 1.4.  
													//    If jdk 1.5 is used to compile source, then this whitespace char will be recognized.
													//    If jdk 1.4 is used, '\u2060' will be treated as 'u2060' rather than an escpaced whitespace char.  
													//    Avoid using it in this sanity test.
// #define UNICODE_IDEOGRAPHIC_SPACE 0x3000			// - ideographic space
// #define UNICODE_ZEROWIDTH_NOBREAK_SPACE 0xFEFF	// - zero width no-break space

 
function testSpace(input)
{
	var pattern = /([^\s]*)\s(\S*)\s([^\s]*)\s(\S*)\s(.*)/; // i.e. 5 blocks of non-whitespace seperated by single whitespace chars
	var result = pattern(input);
	print("space test result length: " + result.length);
	print("match: " + result);
}

function testsplit(input)
{
    var pattern2  = new RegExp(); 
    var result2  = input.split(pattern2); // when the empty RegExp is used, we match the position before the first char.
    print("result length: " + result2.length); // this should cause the string to be broken up into an array of individual chars.
    for (var i=0; i<result2.length; i++) 
    {
        print(result2[i]);
    }
}

testsplit("\u0180\u0181\u0182\u0083");
testsplit("AAA\u00A0\u0181\u200bBCD\u3000EFG\ufeffHJI");
testSpace("we\u00A0\u0281have\u0282\u200bfound\u3000each\ufeffmatch");
