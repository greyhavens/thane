constants----------------------------------------------------
      ConstantUtf8Info abcdefg -> 1
      ConstantUtf8Info print -> 2
      ConstantUtf8Info  -> 3
      ConstantPackageNamespace 3 -> 1
      ConstantUtf8Info b137879.as$1 -> 4
      ConstantPrivateNamespace 4 -> 2
      ConstantNamespaceSet 2 { 1 2 } -> 1
      ConstantMultiname 2 1 -> 1
      ConstantUtf8Info charAt(Infinity) ==  -> 5
      ConstantUtf8Info Infinity -> 6
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 6 1 -> 2
      ConstantUtf8Info Infinity
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 6 1
      ConstantUtf8Info charAt -> 7
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 7 1 -> 3
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info a.charAt(4294967296) ==  -> 8
      ConstantDoubleInfo +4.29496e9 -> 1
      ConstantUtf8Info charAt
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 7 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info a.charAt(4294967296+1) ==  -> 9
      ConstantDoubleInfo +4.29496e9
      ConstantUtf8Info charAt
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 7 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info a.indexOf('2',4294967296) ==  -> 10
      ConstantUtf8Info 2 -> 11
      ConstantDoubleInfo +4.29496e9
      ConstantUtf8Info indexOf -> 12
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 12 1 -> 4
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info a.charCodeAt(4294967296) ==  -> 13
      ConstantDoubleInfo +4.29496e9
      ConstantUtf8Info charCodeAt -> 14
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 14 1 -> 5
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info a.substring(4294967296,4294967296+2) ==  -> 15
      ConstantDoubleInfo +4.29496e9
      ConstantDoubleInfo +4.29496e9
      ConstantUtf8Info substring -> 16
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 16 1 -> 6
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info a.substring(NaN,Infinity) ==  -> 17
      ConstantUtf8Info NaN -> 18
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 18 1 -> 7
      ConstantUtf8Info NaN
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 18 1
      ConstantUtf8Info Infinity
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 6 1
      ConstantUtf8Info Infinity
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 6 1
      ConstantUtf8Info substring
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 16 1
      ConstantUtf8Info print
      ConstantNamespaceSet 2 { 1 2 }
      ConstantMultiname 2 1
      ConstantUtf8Info 
      ConstantUtf8Info a -> 19
      ConstantQualifiedName 19 2 -> 8
definitions--------------------------------------------------
      MethodInfo  param_count=0 return_type=0 param_types={ } debug_name_index=3 needs_arguments=false need_rest=false needs_activation=false has_optional=false ignore_rest=false native=false has_param_names =false -> 0
      MethodBody max_stack=6 max_locals=2 scope_depth=1 max_scope=2 code_length=150 traits_count=0 -> 0
      *Trait name_index=8 tag=var slot_id=1 type_index=0 value=0 value_kind=0
      ScriptInfo init_index=0 traits_count=1 -> 0
code---------------------------------------------------------
// ++StartProgram 
// ++StartMethod $init
AllocTemp 1
LoadThis
      0:Getlocal0 [1]
PushScope
      1:Pushscope [0]
PushString abcdefg
      2:Pushstring 1 [1]
StoreGlobal 0, *
      4:Getglobalscope
      5:Swap
      6:Setslot 1 [0]
FindProperty print 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      8:Findpropstrict 1 [1]
PushString charAt(Infinity) == 
      10:Pushstring 5 [2]
LoadGlobal 0, *
      12:Getglobalscope
      13:Getslot 1 [3]
FindProperty Infinity 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      15:Findpropstrict 2 [4]
GetProperty Infinity 2 { b137879.as$1(private) public } is_qualified=false is_super=false is_attr=false
      17:Getproperty 2 [4]
CallProperty charAt 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      19:Callproperty 3 1 [3]
InvokeBinary BinaryPlusOp
      22:Add [2]
CallProperty print 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      23:Callproperty 1 1 [1]
Pop
      26:Pop [0]
FindProperty print 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      27:Findpropstrict 1 [1]
PushString a.charAt(4294967296) == 
      29:Pushstring 8 [2]
LoadGlobal 0, *
      31:Getglobalscope
      32:Getslot 1 [3]
PushNumber +4.29496e9, double
      34:Pushdouble 1 [4]
CallProperty charAt 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      36:Callproperty 3 1 [3]
InvokeBinary BinaryPlusOp
      39:Add [2]
CallProperty print 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      40:Callproperty 1 1 [1]
Pop
      43:Pop [0]
FindProperty print 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      44:Findpropstrict 1 [1]
PushString a.charAt(4294967296+1) == 
      46:Pushstring 9 [2]
LoadGlobal 0, *
      48:Getglobalscope
      49:Getslot 1 [3]
PushNumber +4.29496e9, double
      51:Pushdouble 1 [4]
PushNumber 1, int
      53:Pushbyte 1 [5]
InvokeBinary BinaryPlusOp
      55:Add [4]
CallProperty charAt 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      56:Callproperty 3 1 [3]
InvokeBinary BinaryPlusOp
      59:Add [2]
CallProperty print 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      60:Callproperty 1 1 [1]
Pop
      63:Pop [0]
FindProperty print 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      64:Findpropstrict 1 [1]
PushString a.indexOf('2',4294967296) == 
      66:Pushstring 10 [2]
LoadGlobal 0, *
      68:Getglobalscope
      69:Getslot 1 [3]
PushString 2
      71:Pushstring 11 [4]
PushNumber +4.29496e9, double
      73:Pushdouble 1 [5]
CallProperty indexOf 2 { b137879.as$1(private) public } 2 is_qualified=false is_super=false is_attr=false
      75:Callproperty 4 2 [3]
InvokeBinary BinaryPlusOp
      78:Add [2]
CallProperty print 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      79:Callproperty 1 1 [1]
Pop
      82:Pop [0]
FindProperty print 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      83:Findpropstrict 1 [1]
PushString a.charCodeAt(4294967296) == 
      85:Pushstring 13 [2]
LoadGlobal 0, *
      87:Getglobalscope
      88:Getslot 1 [3]
PushNumber +4.29496e9, double
      90:Pushdouble 1 [4]
CallProperty charCodeAt 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      92:Callproperty 5 1 [3]
InvokeBinary BinaryPlusOp
      95:Add [2]
CallProperty print 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      96:Callproperty 1 1 [1]
Pop
      99:Pop [0]
FindProperty print 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      100:Findpropstrict 1 [1]
PushString a.substring(4294967296,4294967296+2) == 
      102:Pushstring 15 [2]
LoadGlobal 0, *
      104:Getglobalscope
      105:Getslot 1 [3]
PushNumber +4.29496e9, double
      107:Pushdouble 1 [4]
PushNumber +4.29496e9, double
      109:Pushdouble 1 [5]
PushNumber 2, int
      111:Pushbyte 2 [6]
InvokeBinary BinaryPlusOp
      113:Add [5]
CallProperty substring 2 { b137879.as$1(private) public } 2 is_qualified=false is_super=false is_attr=false
      114:Callproperty 6 2 [3]
InvokeBinary BinaryPlusOp
      117:Add [2]
CallProperty print 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      118:Callproperty 1 1 [1]
Pop
      121:Pop [0]
FindProperty print 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      122:Findpropstrict 1 [1]
PushString a.substring(NaN,Infinity) == 
      124:Pushstring 17 [2]
LoadGlobal 0, *
      126:Getglobalscope
      127:Getslot 1 [3]
FindProperty NaN 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      129:Findpropstrict 7 [4]
GetProperty NaN 2 { b137879.as$1(private) public } is_qualified=false is_super=false is_attr=false
      131:Getproperty 7 [4]
FindProperty Infinity 2 { b137879.as$1(private) public } is_strict=true is_qualified=false is_attr=false
      133:Findpropstrict 2 [5]
GetProperty Infinity 2 { b137879.as$1(private) public } is_qualified=false is_super=false is_attr=false
      135:Getproperty 2 [5]
CallProperty substring 2 { b137879.as$1(private) public } 2 is_qualified=false is_super=false is_attr=false
      137:Callproperty 6 2 [3]
InvokeBinary BinaryPlusOp
      140:Add [2]
CallProperty print 2 { b137879.as$1(private) public } 1 is_qualified=false is_super=false is_attr=false
      141:Callproperty 1 1 [1]
CheckType *
      144:Coerce.o [1]
StoreRegister 1, *
      145:Setlocal1 [0]
LoadRegister 1, *
      146:Getlocal1 [1]
Return
      147:Returnvalue [0]
FreeTemp 1
      148:Kill 1
// --FinishMethod $init 
// --FinishProgram 