/* -*- mode: java; tab-width: 4 -*- */
/* Generate a mega-switch for keyword recognition.
 * Approximately right for AS3 (this is a prototype)
 *
 * Run this script, capture the output, remove the last two lines of
 * the captured output (which should be "default:" and "goto bigswitch_end;").
 * Then replace the code in eval_lex.cpp between the lines "Begin
 * generated code" and "End generated code" by the output.  Then
 * reindent (Emacs and Xcode are your friends).
 */

var keywords = [[ "as", "T_As", "!compiler->es3_keywords"],
				[ "break", "T_Break"],
                [ "case", "T_Case"],
                [ "catch", "T_Catch"],
                [ "class", "T_Class", "!compiler->es3_keywords"],
                [ "const", "T_Const", "!compiler->es3_keywords"],
                [ "continue", "T_Continue"],
                [ "default", "T_Default"],
                [ "delete", "T_Delete"],
                [ "do", "T_Do"],
                [ "else", "T_Else"],
				[ "extends", "T_Extends", "!compiler->es3_keywords"],
                [ "false", "T_False"],
                [ "finally", "T_Finally"],
                [ "for", "T_For"],
                [ "function", "T_Function"],
                [ "if", "T_If"],
				[ "implements", "T_Implements", "!compiler->es3_keywords"],
                [ "import", "T_Import", "!compiler->es3_keywords"],
                [ "in", "T_In"],
                [ "instanceof", "T_InstanceOf"],
                [ "interface", "T_Interface", "!compiler->es3_keywords"],
                [ "internal", "T_Internal", "!compiler->es3_keywords"],
                [ "is", "T_Is", "!compiler->es3_keywords"],
                [ "native", "T_Native", "!compiler->es3_keywords"],
                [ "new", "T_New"],
                [ "null", "T_Null"],
				[ "package", "T_Package", "!compiler->es3_keywords"],
				[ "private", "T_Private", "!compiler->es3_keywords"],
				[ "protected", "T_Protected", "!compiler->es3_keywords"],
				[ "public", "T_Public", "!compiler->es3_keywords"],
                [ "return", "T_Return"],
                [ "super", "T_Super", "!compiler->es3_keywords"],
                [ "switch", "T_Switch"],
                [ "this", "T_This"],
                [ "throw", "T_Throw"],
                [ "true", "T_True"],
                [ "try", "T_Try"],
                [ "typeof", "T_TypeOf"],
				[ "use", "T_Use", "!compiler->es3_keywords"],
                [ "var", "T_Var"],
                [ "void", "T_Void"],
                [ "while", "T_While"],
                [ "with", "T_With"]];

keywords.sort(function (a, b) { return a[0] < b[0] ? -1 : a[0] > b[0] ? 1 : 0 });
print(generate(keywords, 0));

// notPartOfIdent is a function that maps ASCII codes and Unicode space
// values that definitely terminate any identifier to true, and
// every other character code to false.  If the code for a
// character following a keyword maps to true then we
// can safely decide that we've seen a keyword, and if it doesn't map
// to true then we're not going to have to look for a keyword in
// identifier().  The latter is the case only because the program will
// cause a syntax error to be signalled if there is a
// non-identifier-constituent Unicode character terminating a keyword.

function generate(tbl, pos) {
    var a = "_".charCodeAt(0);
    var n = "z".charCodeAt(0) - "_".charCodeAt(0) + 1;
    var buckets = new Array(n);
    var newDefault = false;
    var count = 0;

    for ( var i=0 ; i < n ; i++ )
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
            s += "idx[" + (i-1) + "] == '" + name.charAt(i) + "' &&\n";
        if (tblItem[2])
            s += tblItem[2] + " &&\n";
        s += "notPartOfIdent(idx[" + (name.length-1) + "])) {\n"
        s += "idx += " + (name.length-1) + ";\n";
        s += "return " + tblItem[1] + ";\n";
        s += "}\n";
    }
    if (count > 1) {
        if (pos > 0)
            s += "switch(idx[" + (pos - 1) + "]) {\n";
    
        for ( var i=0 ; i < n ; i++ ) {
            if (buckets[i].length > 0) {
                s += ("case '" + String.fromCharCode(a + i) + "': \n" + 
                      generate(buckets[i], pos+1));
            }
        }
        s += "default:\n";
    }

    if (newDefault) {
        var precond = "";
        if (newDefault[2])
            precond = newDefault[2] + " && ";
        s += "if (" + precond + "!notPartOfIdent(idx[" + (newDefault[0].length - 1)+ "])) \n";
        s += "goto bigswitch_end;\n";
        s += "idx += " + (newDefault[0].length - 1) + ";\n";
        s += "return " + newDefault[1] + ";\n";
    }
    else
        s += "goto bigswitch_end;\n";

    if (count > 1 && pos > 0)
        s += "}\n";
        
    return s;
}
