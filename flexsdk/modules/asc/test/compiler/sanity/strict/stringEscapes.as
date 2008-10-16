var s1:String = "this is a 'continued \
 line' the newline in the source is removed during parsing"
print(s1);

/* This is a comment with a ' in it, as well as a " and a \ to test that special
    characters are ignored during comment parsing */
var s2:String = 'this is not a "continued \n line" the embedded newline is added during parsing'

print(s2);

var s3:String = "un\kn\own escape chars such as '\\q' should result in the character 'q', rather than '\\q'";
print(s3);