
program 
  statementlist 
    config namespace  
      identifier CONFIG 
    import  
      packagename 
        packageidentifiers 
          identifier __AS3__
          identifier vec
          identifier Vector
    var  
      list 
        variablebinding 
          typedidentifier 
            qualifiedidentifier a
          literalstring:abcdefg
    expression 
      list 
        member 
          call lexical 
            identifier print
            argumentlist 
              binary plus 
                literalstring:charAt(Infinity) == 
                member 
                  member 
                    get lexical 
                      identifier a
                  call dot 
                    identifier charAt
                    argumentlist 
                      member 
                        get lexical 
                          identifier Infinity
    expression 
      list 
        member 
          call lexical 
            identifier print
            argumentlist 
              binary plus 
                literalstring:a.charAt(4294967296) == 
                member 
                  member 
                    get lexical 
                      identifier a
                  call dot 
                    identifier charAt
                    argumentlist 
                      literalnumber:4294967296
    expression 
      list 
        member 
          call lexical 
            identifier print
            argumentlist 
              binary plus 
                literalstring:a.charAt(4294967296+1) == 
                member 
                  member 
                    get lexical 
                      identifier a
                  call dot 
                    identifier charAt
                    argumentlist 
                      binary plus 
                        literalnumber:4294967296
                        literalnumber:1
    expression 
      list 
        member 
          call lexical 
            identifier print
            argumentlist 
              binary plus 
                literalstring:a.indexOf('2',4294967296) == 
                member 
                  member 
                    get lexical 
                      identifier a
                  call dot 
                    identifier indexOf
                    argumentlist 
                      literalstring:2
                      literalnumber:4294967296
    expression 
      list 
        member 
          call lexical 
            identifier print
            argumentlist 
              binary plus 
                literalstring:a.charCodeAt(4294967296) == 
                member 
                  member 
                    get lexical 
                      identifier a
                  call dot 
                    identifier charCodeAt
                    argumentlist 
                      literalnumber:4294967296
    expression 
      list 
        member 
          call lexical 
            identifier print
            argumentlist 
              binary plus 
                literalstring:a.substring(4294967296,4294967296+2) == 
                member 
                  member 
                    get lexical 
                      identifier a
                  call dot 
                    identifier substring
                    argumentlist 
                      literalnumber:4294967296
                      binary plus 
                        literalnumber:4294967296
                        literalnumber:2
    expression 
      list 
        member 
          call lexical 
            identifier print
            argumentlist 
              binary plus 
                literalstring:a.substring(NaN,Infinity) == 
                member 
                  member 
                    get lexical 
                      identifier a
                  call dot 
                    identifier substring
                    argumentlist 
                      member 
                        get lexical 
                          identifier NaN
                      member 
                        get lexical 
                          identifier Infinity
