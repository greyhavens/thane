/* Generate a mega-switch for keyword recognition */

var keywords = [// always reserved
                [ "break", "Token::Break"],
                [ "case", "Token::Case"],
                [ "catch", "Token::Catch"],
                [ "class", "Token::Class"],
                [ "continue", "Token::Continue"],
                [ "default", "Token::Default"],
                [ "delete", "Token::Delete"],
                [ "do", "Token::Do"],
                [ "else", "Token::Else"],
                [ "enum", "Token::Enum"],
                [ "extends", "Token::Extends"],
                [ "false", "Token::False"],
                [ "finally", "Token::Finally"],
                [ "for", "Token::For"],
                [ "function", "Token::Function"],
                [ "if", "Token::If"],
                [ "in", "Token::In"],
                [ "instanceof", "Token::InstanceOf"],
                [ "new", "Token::New"],
                [ "null", "Token::Null"],
                [ "return", "Token::Return"],
                [ "super", "Token::Super"],
                [ "switch", "Token::Switch"],
                [ "this", "Token::This"],
                [ "throw", "Token::Throw"],
                [ "true", "Token::True"],
                [ "try", "Token::Try"],
                [ "typeof", "Token::TypeOf"],
                [ "var", "Token::Var"],
                [ "void", "Token::Void"],
                [ "while", "Token::While"],
                [ "with", "Token::With"],
                // contextually reserved
                [ "call", "Token::Call"],
                [ "cast", "Token::Cast"],
                [ "const", "Token::Const"],
                [ "decimal", "Token::Decimal"],
                [ "double", "Token::Double"],
                [ "dynamic", "Token::Dynamic"],
                [ "each", "Token::Each"],
                [ "eval", "Token::Eval"],
                [ "final", "Token::Final"],
                [ "get", "Token::Get"],
                [ "has", "Token::Has"],
                [ "implements", "Token::Implements"],
                [ "import", "Token::Import"],
                [ "int", "Token::Int"],
                [ "interface", "Token::Interface"],
                [ "internal", "Token::Internal"],
                [ "intrinsic", "Token::Intrinsic"],
                [ "is", "Token::Is"],
                [ "let", "Token::Let"],
                [ "namespace", "Token::Namespace"],
                [ "native", "Token::Native"],
                [ "override", "Token::Override"],
                [ "package", "Token::Package"],
                [ "precision", "Token::Precision"],
                [ "private", "Token::Private"],
                [ "protected", "Token::Protected"],
                [ "prototype", "Token::Prototype"],
                [ "public", "Token::Public"],
                [ "rounding", "Token::Rounding"],
                [ "standard", "Token::Standard"],
                [ "strict", "Token::Strict"],
                [ "to", "Token::To"],
                [ "set", "Token::Set"],
                [ "static", "Token::Static"],
                [ "to", "Token::To"],
                [ "type", "Token::Type"],
                [ "uint", "Token::UInt"],
                [ "undefined", "Token::Undefined"],
                [ "use", "Token::Use"],
                [ "unit", "Token::Unit"],
                [ "xml", "Token::Xml"],
                [ "yield", "Token::Yield"]];

//keywords = [["case", "CASE"], ["call","CALL"],["ca","CA"]];

keywords.sort(function (a, b) {  return a[0] < b[0] ? -1 : a[0] > b[0] ? 1 : 0 });
print(generate( keywords, 0 ));

// notPartOfIdent is an Array that maps ASCII codes and Unicode space
// values that definitely terminate any identifier to "true", and
// every other character code to "undefined".  If the code for a
// character following a keyword maps to true in this table then we
// can safely decide that we've seen a keyword, and if it doesn't map
// to true then we're not going to have to look for a keyword in
// identifier().  The latter is the case only because the program will
// cause a syntax error to be signalled if there is a
// non-identifier-constituent Unicode character terminating a keyword.

function generate(tbl, pos) {
    var a = "a".charCodeAt(0);
    var buckets = new Array(26);
    var newDefault = false;
    var count = 0;

    for ( var i=0 ; i < 26 ; i++ )
        buckets[i] = new Array();

    var tblItem; // used below if there's only one
    for ( var i=0 ; i < tbl.length ; i++ ) {
        tblItem = tbl[i];
        var item = tblItem[0];
        if (item.length > pos) {
            var x = item.charCodeAt(pos) - a;
            buckets[x].push(tblItem);
            count++;
        }
        if (item.length == pos)
            newDefault = tblItem;
    }

    var s = "";
    if (count == 1) {
        var name = tblItem[0];
        s += "if (";
        for ( var i=pos ; i < name.length ; i++ )
            s += "src.charPosAt(curIndex+" + (i-1) + ") == " + name.charCodeAt(i) + " /* Char::" + name.charAt(i) + " */ &&\n";
        s += "notPartOfIdent[src.charPosAt(curIndex+" + (name.length-1) + ")]) {\n"
        s += "curIndex += " + (name.length-1) + ";\n";
        s += "return " + tblItem[1] + ";\n";
        s += "}\n";
    }
    if (count > 1) {
        if (pos > 0)
            s += "switch(str.charCodeAt(curIndex+" + (pos - 1) + ")) {\n";
    
        for ( var i=0 ; i < 26 ; i++ ) {
            if (buckets[i].length > 0) {
                s += ("case " + (a + i) + ": /* Char::" + String.fromCharCode(a + i) + " */\n" + 
                      generate(buckets[i], pos+1));
            }
        }
        s += "default:\n";
    }

    if (newDefault) {
        s += "if (!(notPartOfIdent[src.charPosAt(curIndex+" + (newDefault[0].length - 1)+ ")])) \n";
        s += "break bigswitch;\n";
        s += "curIndex += " + (newDefault[0].length - 1) + ";\n";
        s += "return " + newDefault[1] + ";\n";
    }
    else
        s += "break bigswitch;\n";

    if (count > 1 && pos > 0)
        s += "}\n";
        
    return s;
}
