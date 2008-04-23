/* internal */ namespace AVMPLUS = "avmplus";
/* internal */ namespace FLASH_UTILS = "flash.utils";

// load ast
{
    use namespace AVMPLUS;
    use namespace FLASH_UTILS;

    var fname = System.argv[0];
    var verbose = false;
    if( fname == "verbose" )
    {
        fname = System.argv[1];
        verbose = true;
    }
    if (fname==undefined) throw "no file name given";
    var bytes = ByteArray.readFile(fname+".abc");
}

{
    use namespace AVMPLUS;

    bytes = Abc::parseAbcFile(new ABCByteStream(bytes));

    //print ("encoding asm");
    var tx = AbcEncode::abcFile (bytes, verbose);
    writeFile (tx,fname+".asm");
    print (fname+".asm, "+tx.length+" chars written");
}
