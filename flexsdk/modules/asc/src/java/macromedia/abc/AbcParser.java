////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2005-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package macromedia.abc;

import static macromedia.asc.parser.Tokens.EMPTY_TOKEN;
import static macromedia.asc.semantics.Slot.*;
import macromedia.asc.util.ObjectList;
import macromedia.asc.util.Context;
import macromedia.asc.util.Namespaces;
import macromedia.asc.util.IntList;
import macromedia.asc.util.Decimal128;
import macromedia.asc.parser.*;
import macromedia.asc.semantics.*;
import macromedia.asc.embedding.avmplus.ActionBlockConstants;
import macromedia.asc.embedding.avmplus.ClassBuilder;
import macromedia.asc.embedding.avmplus.InstanceBuilder;
import macromedia.asc.embedding.avmplus.FunctionBuilder;
import macromedia.asc.embedding.avmplus.GlobalBuilder;
import macromedia.asc.embedding.avmplus.RuntimeConstants;
import macromedia.asc.embedding.avmplus.ActivationBuilder;

import java.io.IOException;
import java.util.Map;
import java.util.HashMap;

/**
 * @author Erik Tierney
 */
public final class AbcParser
{
    private Context ctx;
    private BytecodeBuffer buf;
    private static final boolean debug = false;

    public AbcParser(Context cx, String name) throws IOException
    {
        this.ctx = cx;
        this.buf = new BytecodeBuffer(name);

    }

    public AbcParser(Context cx, byte[] bytes)
    {
        this.ctx = cx;
        this.buf = new BytecodeBuffer(bytes);
    }


    private int[] cPoolIntPositions;
    private int[] cPoolUIntPositions;
    private int[] cPoolDoublePositions;
    private int[] cPoolDecimalPositions;
    private String[] cPoolStrs;
    private int[] cPoolNsPositions;
    private int[] cPoolNsSetPositions;
    private int[] cPoolMnPositions;

    private int[] methodPositions;
    private int[] metadataPositions;
    private int[] instancePositions;
    private int[] classPositions;
    private int[] scriptPositions;

    private final Map<String, Integer> fun_names = new HashMap<String, Integer>();

    private final Map<String, ClassDefinitionNode> class_nodes = new HashMap<String, ClassDefinitionNode>();

    private final ObjectList<ObjectList<ClassDefinitionNode>> clsdefs_sets = new ObjectList<ObjectList<ClassDefinitionNode>>();
    private final ObjectList<String> region_name_stack = new ObjectList<String>();

    public ProgramNode parseAbc()
    {
        try
        {
            int minor_version = buf.readU16();
            /*int major_version =*/ buf.readU16();

            // Scan the bytecode for the stuff we care about
            scanCpool(minor_version >= ActionBlockConstants.MINORwithDECIMAL);
            scanMethods();
            scanMetadata();
            scanClasses();
            scanScripts();

            // Create a program node that will represent the data in the abc file.
            // It won't actually be a parse tree, but it will contain the correct builder
            // objects which are usually built up by the FlowAnalyzer
            BinaryProgramNode program = ctx.getNodeFactory().binaryProgram(ctx, ctx.getNodeFactory().statementList(null, (StatementListNode)null));
            program.frame = new ObjectValue(ctx, new GlobalBuilder(), ctx.noType());
			GlobalBuilder bui = (GlobalBuilder)(program.frame.builder);
			bui.is_in_package = true; // cn: necessary for proper slot creation for top level functions

			ctx.pushScope(program.frame);

            clsdefs_sets.add(new ObjectList<ClassDefinitionNode>());
            region_name_stack.push_back("");
            for( int i = 0; i < scriptPositions.length; ++i )
            {
                parseScript(i, program);
            }

            program.clsdefs = clsdefs_sets.last();
			// For proper const and type enforcement of top level functions and vars, we need to 
			//  process those def nodes during CE.  These definitions need to be evaluted in the
			//  scope of the BinaryProgramNode's frame, however, while the classes need to be
			//  evaluated outside of that scope.  Remove the class definition nodes from the 
			//  statementlist, they we be processed seperately by iterating over node->clsdefs.
			for (int i = 0; i < program.statements.items.size(); i++) // A B C
			{
				Node def = program.statements.items.get(i);
				if (def instanceof ClassDefinitionNode)
				{
					NodeFactory nf = ctx.getNodeFactory();
					program.statements.items.set(i, nf.emptyStatement());
				}
			}
			clsdefs_sets.removeLast();
            region_name_stack.pop_back();
            ctx.popScope(); // global

            ProgramNode prog = ctx.getNodeFactory().program(ctx, ctx.getNodeFactory().statementList(null, program));

            // Uncomment next line to get some debugging info to print to stdout.
            // dumpProgram(program);
            return prog;
        }
        catch(Exception t)
        {
        	if(debug) t.printStackTrace();
            // Clean up any scope we may have pushed and didn't pop when the exception was thrown
            ObjectValue scope = ctx.scope();
            ObjectValue global = ctx.globalScope();
            while( scope != global )
            {
                ctx.popScope();
                scope = ctx.scope();
            }
            return null;
        }
    }

    void scanCpool(boolean hasDecimal)
    {
    	int size;
        size = buf.readU32();
        cPoolIntPositions = new int[size];
        if (debug) System.out.println("ints "+size);
        for (int i = 1; i < size; i++)
        {
            cPoolIntPositions[i] = buf.pos();
            buf.readU32();
        }
        size = buf.readU32();
        cPoolUIntPositions = new int[size];
        if (debug) System.out.println("uints "+size);
        for (int i = 1; i < size; i++)
        {
            cPoolUIntPositions[i] = buf.pos();
            buf.readU32();
        }
        size = buf.readU32();
        cPoolDoublePositions = new int[size];
        if (debug) System.out.println("doubles "+size);
        for (int i = 1; i < size; i++)
        {
            cPoolDoublePositions[i] = buf.pos();
            buf.readDouble();
        }
        if (hasDecimal) {
        	size = buf.readU32();
            if (debug) System.out.println("decimals "+size);
        	cPoolDecimalPositions = new int[size];
        	for (int i = 1; i < size; i++) {
        		cPoolDecimalPositions[i] = buf.pos();
        		buf.readDouble(); // cheapest way to skip 64 bits
        		buf.readDouble(); // decimals are 128 bits
        	}
        }
        else {
        	cPoolDecimalPositions = new int[0];
        }
        size = buf.readU32();
        cPoolStrs = new String[size>0?size:1];
        if (debug) System.out.println("strings "+size);
        cPoolStrs[0] = "";
        for (int i = 1; i < size; i++)
        {
            long length = buf.readU32();
        	cPoolStrs[i] = buf.readString((int)length).intern();
            buf.skip(length);
        }
        size = buf.readU32();
        cPoolNsPositions = new int[size];
        if (debug) System.out.println("ns "+size);
        for (int i = 1; i < size; i++)
        {
            cPoolNsPositions[i] = buf.pos();
            buf.readU8(); // kind byte
            buf.readU32();
        }
        size = buf.readU32();
        cPoolNsSetPositions = new int[size];
        if (debug) System.out.println("nssets "+size);
        for (int i = 1; i < size; i++)
        {
            cPoolNsSetPositions[i] = buf.pos();
            long count = buf.readU32(); // count
            for(long q =0; q < count; ++q)
            {
                buf.readU32();
            }
        }
        size = buf.readU32();
        cPoolMnPositions = new int[size];
        if (debug) System.out.println("names "+size);
        for (int i = 1; i < size; i++)
        {
            cPoolMnPositions[i] = buf.pos();
            int kind = buf.readU8();
            switch (kind)
            {
                case ActionBlockConstants.CONSTANT_Qname:
                case ActionBlockConstants.CONSTANT_QnameA:
                    buf.readU32();
                    buf.readU32();
                    break;
                case ActionBlockConstants.CONSTANT_RTQname:
                case ActionBlockConstants.CONSTANT_RTQnameA:
                    buf.readU32();
                    break;
                case ActionBlockConstants.CONSTANT_Multiname:
                case ActionBlockConstants.CONSTANT_MultinameA:
                    buf.readU32();
                    buf.readU32();
                    break;
                case ActionBlockConstants.CONSTANT_MultinameL:
                case ActionBlockConstants.CONSTANT_MultinameLA:
                    buf.readU32();
                    break;
                case ActionBlockConstants.CONSTANT_TypeName:
                    buf.readU32(); // name index
                    long count = buf.readU32(); // param count;
                    buf.skipEntries(count);
                    break;
                case ActionBlockConstants.CONSTANT_RTQnameL:
                case ActionBlockConstants.CONSTANT_RTQnameLA:
					break;
                default:
					throw new RuntimeException("bad multiname type: " + kind);

            }
        }

    }

    void scanMethods()
    {
        long methodEntries = buf.readU32();

        methodPositions = new int [(int)methodEntries];
        for (int i = 0; i < methodEntries; i++)
        {
            methodPositions[i] = buf.pos();
            long paramCount = buf.readU32();
            buf.readU32(); //returnType
            buf.skipEntries(paramCount);  //parameters
            buf.readU32(); //name_index
            int flags = buf.readU8();

            long optionalCount = ((flags & ActionBlockConstants.METHOD_HasOptional) != 0) ? buf.readU32() : 0;
            for( long q = 0; q < optionalCount; ++q )
            {
                buf.readU32();  // optionals
                buf.readU8();  //kind byte
            }

            long paramNameCount = ((flags & ActionBlockConstants.METHOD_HasParamNames)!=0) ? paramCount : 0;
            for( long q = 0; q < paramNameCount; ++q )
            {
                buf.readU32();  // param name
            }
        }
    }

    void scanMetadata()
    {
        long metadataEntries = buf.readU32();

        metadataPositions = new int [(int)metadataEntries];
        for (int i = 0; i < metadataEntries; i++)
        {
            metadataPositions[i] = buf.pos();
            buf.readU32();
            long value_count = buf.readU32(); //returnType
            buf.skipEntries(value_count * 2);  //keys & values
        }
    }

    void scanClasses()
    {
        long classEntries = buf.readU32();

        classPositions = new int[(int)classEntries];
        instancePositions = new int[(int)classEntries];
        // InstanceInfos
        for(int i = 0; i < classEntries; ++i)
        {
            instancePositions[i] = buf.pos();
            buf.readU32(); //name_index
            buf.readU32(); //super_index
            int flags = buf.readU8();
            if ((flags & ActionBlockConstants.CLASS_FLAG_protected) != 0)
            {
            	buf.readU32();
            }
            
            long interfaces = buf.readU32();
            buf.skipEntries(interfaces); // interfaces
            buf.readU32(); //init index
            skipTraits();
        }

        // ClassInfos
        for(int i = 0; i < classEntries; ++i)
        {
            classPositions[i] = buf.pos();
            buf.readU32(); //init index
            skipTraits();
        }

    }

    void scanScripts()
    {
        long scriptEntries = buf.readU32();
        scriptPositions = new int[(int)scriptEntries];
        for(int i = 0 ; i < scriptEntries; ++i )
        {
            scriptPositions[i] = buf.pos();
            buf.readU32(); // init index
            skipTraits();
        }
    }

// skips traits - we don't care about what they are when scanning the bytecode
    void skipTraits()
    {
        long count = buf.readU32();

        for (int i = 0; i < count; i++)
        {
            buf.readU32();
            int kind = buf.readU8();
            int tag = kind & 0x0f;
            int temp;

            switch (tag)
            {
            case ActionBlockConstants.TRAIT_Var:
            case ActionBlockConstants.TRAIT_Const:
                buf.skipEntries(2);
                temp = buf.readU32();
                if( temp > 0 )
                    buf.readU32(); //kind byte
                break;
            case ActionBlockConstants.TRAIT_Method:
            case ActionBlockConstants.TRAIT_Getter:
            case ActionBlockConstants.TRAIT_Setter:
                buf.skipEntries(2);
                break;
            case ActionBlockConstants.TRAIT_Class:
            case ActionBlockConstants.TRAIT_Function:
                buf.skipEntries(2);
                break;
            default:
                break;
                //throw new DecoderException("Invalid trait kind: " + kind);
            }
            if( ( (kind >> 4) & ActionBlockConstants.TRAIT_FLAG_metadata) != 0 )
            {
                long metadata = buf.readU32();
                buf.skipEntries(metadata);
            }
        }
    }

    static private class DefAndSlot
    {
        public DefinitionNode def;
        public Slot slot;
    }
    DefAndSlot slotTrait(int nameID, int slotID, int typeID, int valueID, int value_kind, boolean is_const)
    {
        ObjectValue obj = ctx.scope();

        DefAndSlot ret = new DefAndSlot();

        QName qName = getQNameFromCPool(nameID);
        String simpleName = getStringFromCPool(qName.name);

        ObjectValue ns = getNamespace(qName.nameSpace);

        Namespaces names = new Namespaces();
        names.push_back(ns);
        TypeValue type = ctx.noType(); // cn: type will be set correctly during CE,
										   //   we can't use getTypeFromQName(typeID); here because type could be farther
										   //   along in the file.

        int var_id = obj.builder.Variable(ctx, obj);
        int slot_id = obj.builder.ExplicitVar(ctx,obj,simpleName,names, type,-1,-1,var_id);

        Slot slot = obj.getSlot(ctx,slot_id);
		slot.setConst(is_const);
		slot.setImported(true);

        ret.slot = slot;

        IdentifierNode id = identifierNode(simpleName, ns);

        AttributeListNode attr = attributeList(false, false, false, ns, obj.builder);

        if( value_kind == ActionBlockConstants.CONSTANT_Namespace )
        {
            ObjectValue nsValue = getNamespace(valueID);
            slot.setObjectValue(nsValue);
			slot.setConst(true);

            ret.def = ctx.getNodeFactory().namespaceDefinition(attr, id, ctx.getNodeFactory().literalString(nsValue.name));
            return ret;
        }
        macromedia.asc.semantics.QName typeName;
        MemberExpressionNode typeExpr = null;
        IdentifierNode typeIdNode = null;
        if( typeID != 0 )
        {
            QName t = getQNameFromCPool(typeID);
            typeExpr = memberExprFromName(t);
        }
        else
        {
            typeExpr = null;
        }

        Node init = getInitValue(valueID,value_kind);     // (0,0) means undefined

        TypedIdentifierNode typedID = ctx.getNodeFactory().typedIdentifier(id, typeExpr);
        int tok = is_const ? Tokens.CONST_TOKEN : Tokens.VAR_TOKEN;
        VariableBindingNode bind = ctx.getNodeFactory().variableBinding(attr, tok, typedID, init);

        bind.ref = id.ref;
        if( typeExpr != null )
        {
            bind.typeref = typeExpr.ref;
        }

        ret.def = (DefinitionNode) ctx.getNodeFactory().variableDefinition(attr, tok, ctx.getNodeFactory().list(null, bind));
        return ret;
                // This is always a definition node. only while parsing prototype vars could it be a statement list
    }

    private MemberExpressionNode memberExprFromName(QName q)
    {
        MemberExpressionNode typeExpr = null;
        macromedia.asc.semantics.QName typeName = getFullName(q);
        if( typeName instanceof ParameterizedName )
        {
            ParameterizedQName pqn = (ParameterizedQName)q;
            ParameterizedName pn = (ParameterizedName)typeName;

            NodeFactory nf = ctx.getNodeFactory();

            Node apply;
            IdentifierNode baseIdNode = identifierNode(pn.name, pn.ns);


            ListNode list = null;
            MemberExpressionNode param_node = memberExprFromName(getQNameFromCPool(pqn.params.at(0)));
            list = nf.list(list, param_node);

            apply = nf.applyTypeExpr(baseIdNode, list, -1);
            typeExpr = nf.memberExpression(null, (SelectorNode)apply);
            typeExpr.ref = baseIdNode.ref;
            typeExpr.ref.addTypeParam(param_node.ref);
        }
        else
        {
            IdentifierNode typeIdNode = identifierNode(typeName.name, typeName.ns);
            GetExpressionNode getNode = ctx.getNodeFactory().getExpression(typeIdNode);
            typeExpr = ctx.getNodeFactory().memberExpression(null, getNode);
            typeExpr.ref = typeIdNode.ref;
        }
        return typeExpr;
    }
    // Creates an identifier node, and fills in the ref with the correct reference value
    private IdentifierNode identifierNode(String simpleName, ObjectValue ns)
    {
        IdentifierNode id = ctx.getNodeFactory().identifier(simpleName);
        //IdentifierNode will clean this up
        id.ref = new ReferenceValue(ctx, null, id.name, ns);
        return id;
    }


    DefAndSlot methodTrait(int nameID, int dispID, int methInfo, int attrs, int kind)
    {
        QName methQName = getQNameFromCPool(nameID);

        String methName = getStringFromCPool(methQName.name);
        ObjectValue ns = getNamespace(methQName.nameSpace);

        return methodTrait(methName, ns, dispID, methInfo, attrs, kind);
    }

    DefAndSlot methodTrait(String methName, ObjectValue ns, int dispID, int methInfo, int attrs, int kind)
    {
        ObjectValue obj = ctx.scope();

        DefAndSlot ret = new DefAndSlot();

        int method_id;
        int method_slot;

        boolean isFinal = (attrs & ActionBlockConstants.TRAIT_FLAG_final) != 0;
        boolean isOverride = (attrs & ActionBlockConstants.TRAIT_FLAG_override) != 0;

        Namespaces names = new Namespaces();
        names.push_back(ns);
        // Create the right type of method based on what kind it is.  Either a setter, getter, or regular method
        switch(kind)
        {
        case ActionBlockConstants.TRAIT_Setter: //Set property
            method_id = obj.builder.Method(ctx,obj,methName,names,false);
            method_slot = obj.builder.ExplicitSet(ctx,obj,methName,names,ctx.noType(),isFinal,isOverride,-1,method_id,-1);
            break;
        case ActionBlockConstants.TRAIT_Getter: //get propert
            method_id = obj.builder.Method(ctx,obj,methName,names,false);
            method_slot = obj.builder.ExplicitGet(ctx,obj,methName,names,ctx.noType(),isFinal,isOverride,-1,method_id,-1);
            break;
        default: // regular method
            method_id = obj.builder.Method(ctx,obj,methName,names,false);
            method_slot = obj.builder.ExplicitCall(ctx,obj,methName,names,ctx.noType(),isFinal,isOverride,-1,method_id,-1);
            break;
        }

        ObjectValue funcObj = new ObjectValue(ctx,new FunctionBuilder(),ctx.functionType());
        Slot slot = obj.getSlot(ctx, method_slot);
        slot.setObjectValue(funcObj);
		slot.setImported(true);

        // Calculate the internal name - this matter because with get/set properties you can end up with
        // multiple methods with the same name, but they must each have different internal names.
        StringBuilder internal_name = new StringBuilder(methName.length() + 5);
        if( !fun_names.containsKey(methName) )
        {
            fun_names.put(methName,0);
        }
        internal_name.append(methName).append('$');
        int num = fun_names.get(methName);
        internal_name.append(num);
        num++;
        fun_names.put(methName, num);

        int slot_id = obj.getImplicitIndex(ctx,method_slot,Tokens.EMPTY_TOKEN);
        Slot implied_slot = obj.getSlot(ctx, slot_id);
		implied_slot.setImported(true);

	    String n = internal_name.toString();
        implied_slot.setMethodName(n);//node->fexpr->internal_name;
        slot.getObjectValue().name = n;
        ret.slot = implied_slot;

        // Make sure the FunctionBuilder gets cleaned up
        int functionKind;
        switch( kind )
        {
            case ActionBlockConstants.TRAIT_Getter:
                functionKind = Tokens.GET_TOKEN;
                break;
            case ActionBlockConstants.TRAIT_Setter:
                functionKind = Tokens.SET_TOKEN;
                break;
            default:
                functionKind = Tokens.EMPTY_TOKEN;
                break;
        }

        // Come up with the function signature
        int original = buf.pos();
        buf.seek(methodPositions[methInfo] );

        int paramCount = buf.readU32();
        int returnType = buf.readU32(); //returnType
        int paramTypes[] = new int[paramCount];
        for( int i = 0; i < paramCount; ++i )
        {
            paramTypes[i] = buf.readU32();
        }

        buf.readU32(); // skip the name
        int flags = buf.readU8();
        boolean needs_rest = (flags & ActionBlockConstants.METHOD_Needrest) != 0 || (flags & ActionBlockConstants.METHOD_IgnoreRest) != 0;
        boolean has_optional = (flags & ActionBlockConstants.METHOD_HasOptional) != 0;
        boolean has_param_names = (flags & ActionBlockConstants.METHOD_HasParamNames) != 0;
        int optional_count = 0;
        ObjectList<Node> optional_nodes = null;
        if( has_optional )
        {
            optional_nodes = parseOptionalParams();
            optional_count = optional_nodes.size();
        }
        String[] param_names = new String[paramCount + optional_count];
        if( has_param_names )
        {
            for( int i = 0; i < paramCount; ++i )
            {
                param_names[i] = getStringFromCPool(buf.readU32());
            }
        }
        buf.seek(original);

        MemberExpressionNode retTypeNode = null;
        if( returnType != 0 )
        {
            QName retQName = getQNameFromCPool(returnType);
            //macromedia.asc.semantics.QName retName = getFullName(retQName);

            retTypeNode = memberExprFromName(retQName);
            /*IdentifierNode ident = identifierNode(retName.name, retName.ns);

            GetExpressionNode getNode = ctx.getNodeFactory().getExpression(ident);
            retTypeNode = ctx.getNodeFactory().memberExpression(null, getNode);

            retTypeNode.ref = ident.ref;
            */
        }
        ParameterListNode paramList = null;
        funcObj.activation = new ObjectValue(ctx, new ActivationBuilder(),ctx.noType());
        for( int i = 0, cur_optional = 0; i < paramCount; ++i )
        {
            macromedia.asc.semantics.QName typeName = null;
            QName typeQName = null;
            if( paramTypes[i] != 0 )
            {
                typeQName = getQNameFromCPool(paramTypes[i]);
                typeName = getFullName(typeQName);
            }

            String simple_param_name = param_names[i] != null ? param_names[i] : ("param" + (i+1)).intern();
            ParameterNode param = parameterNode(simple_param_name, typeQName);

            paramList = ctx.getNodeFactory().parameterList(paramList, param);

            int var_id = funcObj.activation.builder.Variable(ctx, funcObj.activation);
            funcObj.activation.builder.ExplicitVar(ctx,funcObj.activation,simple_param_name,ctx.publicNamespace(), ctx.noType(),-1,-1,var_id);
            if( has_optional && i >= paramCount - optional_count )
            {
                param.init = optional_nodes.get(cur_optional++);
            }
        }
        if( needs_rest )
        {
            ParameterNode param = parameterNode("rest", ctx.arrayType().name);

            int var_id = funcObj.activation.builder.Variable(ctx, funcObj.activation);
            funcObj.activation.builder.ExplicitVar(ctx,funcObj.activation,"rest",ctx.publicNamespace(), ctx.arrayType(),-1,-1,var_id);

            RestParameterNode restNode = ctx.getNodeFactory().restParameter(param , -1);
            restNode.ref = param.ref;
            restNode.typeref = param.typeref;
            paramList = ctx.getNodeFactory().parameterList(paramList, restNode);

	    // rsun 11.22.05 porting over a fix made to the 8ball_AS3 branch a long time
	    // ago, but only to the C++ compiler. has_rest needs to be reset here since 
	    // MovieClipMaker is a special entry point that creates function nodes itself. 
	    // otherwise, has_rest is unnecessarily true for all functions processed after
	    // this.
	    ctx.getNodeFactory().has_rest = false;
        }

        FunctionSignatureNode fsn = ctx.getNodeFactory().functionSignature(paramList,retTypeNode);
        if( retTypeNode != null )
        {
	        fsn.typeref = retTypeNode.ref;
        }

        IdentifierNode idNode = identifierNode(methName, ns);
        FunctionCommonNode fcn = ctx.getNodeFactory().functionCommon(ctx, idNode, fsn, null);
        fcn.fun = funcObj;
        fcn.kind = functionKind;
        FunctionNameNode fn = ctx.getNodeFactory().functionName(functionKind, idNode);

        AttributeListNode attr = attributeList(isFinal, isOverride, false, ns, obj.builder);
        FunctionDefinitionNode fdn = ctx.getNodeFactory().binaryFunctionDefinition(ctx, attr, fn, fcn, -1);

        fn.identifier.ref = fcn.ref = new ReferenceValue(ctx, null, idNode.name, ns);

        ret.def = fdn;
        return ret;
    }

    private Node getInitValue(int value_index,int value_kind)
    {
        Node current_node;
        NodeFactory nf = ctx.getNodeFactory();
        double val;
        switch (value_kind)
        {
            case ActionBlockConstants.CONSTANT_Void:
                current_node = nf.unaryExpression(Tokens.VOID_TOKEN,nf.literalNumber("0",-1),-1);
                break;
            case ActionBlockConstants.CONSTANT_Integer:
            case ActionBlockConstants.CONSTANT_UInteger:
            case ActionBlockConstants.CONSTANT_Double:
                val = getNumberFromCPool(value_index, value_kind);
                current_node = nf.literalNumber(String.valueOf(val));
                break;
            case ActionBlockConstants.CONSTANT_Decimal: {
            	Decimal128 dval = getDecimalFromCPool(value_index);
            	String sval = dval.toString();
            	if (ctx.statics.es4_numerics)
            		sval += "m";
            	current_node = nf.literalNumber(sval);
            	break;
            }
            case ActionBlockConstants.CONSTANT_True:
                current_node = nf.literalBoolean(true);
                break;
            case ActionBlockConstants.CONSTANT_False:
                current_node = nf.literalBoolean(false);
                break;
            case ActionBlockConstants.CONSTANT_Utf8:
                current_node = nf.literalString(getStringFromCPool(value_index));
                break;
            case ActionBlockConstants.CONSTANT_Null:
                current_node = nf.literalNull();
                break;
            case ActionBlockConstants.CONSTANT_Namespace:
                ObjectValue ns = getNamespace(value_index);
                current_node = nf.literalString(ns.name);
                break;
            default:
                current_node = nf.literalString("");
                break;
        }
        return current_node;
    }


    private ObjectList<Node> parseOptionalParams()
    {
        int optional_count = buf.readU32();
        ObjectList<Node> optionals = new ObjectList<Node>(optional_count);
        for( int i = 0; i < optional_count; ++i)
        {
            int value_index = buf.readU32();
            int value_kind = buf.readU8();
            Node current_node = getInitValue(value_index,value_kind);
            optionals.push_back(current_node);
        }
        return optionals;
    }

    private ParameterNode parameterNode(String simple_param_name, macromedia.asc.semantics.QName typeName)
    {
        IdentifierNode ident = identifierNode(simple_param_name, ctx.publicNamespace());

        IdentifierNode type_ident = null;
        MemberExpressionNode paramTypeNode = null;
        if( typeName != null )
        {
            type_ident = identifierNode(typeName.name, typeName.ns);

            GetExpressionNode getNode = ctx.getNodeFactory().getExpression(type_ident);
            paramTypeNode = ctx.getNodeFactory().memberExpression(null, getNode);
        }
        ParameterNode param = ctx.getNodeFactory().parameter(Tokens.VAR_TOKEN, ident, paramTypeNode);
        if( type_ident != null )
        {
            param.typeref = type_ident.ref;
        }
        param.ref = ident.ref;

        return param;
    }

    private ParameterNode parameterNode(String simple_param_name, QName typeName)
    {
        IdentifierNode ident = identifierNode(simple_param_name, ctx.publicNamespace());

        MemberExpressionNode paramTypeNode = null;
        if( typeName != null )
        {
            paramTypeNode = memberExprFromName(typeName);
        }
        ParameterNode param = ctx.getNodeFactory().parameter(Tokens.VAR_TOKEN, ident, paramTypeNode);
        if( paramTypeNode != null )
        {
            param.typeref = paramTypeNode.ref;
        }        
        param.ref = ident.ref;

        return param;
    }

    AttributeListNode attributeList(boolean isFinal, boolean isOverride, boolean isDynamic, ObjectValue ns, Builder builder)
    {
        AttributeListNode attr = ctx.getNodeFactory().attributeList(ctx.getNodeFactory().literalString(""), null);

        attr.hasFinal = isFinal;
        attr.hasOverride = isOverride;
        attr.hasDynamic = isDynamic;

        if( builder instanceof ClassBuilder ) //Statics are built by classbuilder, non statics by instancebuilder
        {
            attr.hasStatic = true;
        }
        if( ns != null && (ns == ctx.publicNamespace() || (ns.getNamespaceKind() == Context.NS_PUBLIC && ns.isPackage()) ) )
        {
            // Only package public namespaces will have been qualified with the 'public' access specifier
            // user defined namespaces may also be public namespaces in the cpool, but they will not be package namespaces
            attr.hasPublic = true;
        }
        else if( ns != null && ns.isInternal() )
        {
            attr.hasInternal = true;
        }
        else if( ns != null && ns.isProtected() )
        {
            attr.hasProtected = true;
        }
        else if( ns != null && ns.isPrivate() )
        {
            attr.hasPrivate = true;
        }
        else if( ns != null)
        {
            if( builder.classname != null && ns == builder.classname.ns)
            {
                attr.hasPublic = true;
            }
            else
            {
                attr.namespaces.add(ns);
            }
        }
        return attr;
    }
// Utility to get the full name based on a qname (ns + "/" + name)
    macromedia.asc.semantics.QName getFullName(QName q)
    {
        if( q instanceof ParameterizedQName )
        {
            ParameterizedQName p = (ParameterizedQName)q;
            QName base = getQNameFromCPool(p.name);
            macromedia.asc.semantics.QName base_qn = getFullName(base);
            ObjectList<macromedia.asc.semantics.QName> params = new ObjectList();
            for( int i = 0; i < p.params.size(); ++i) {
                params.add(getFullName(getQNameFromCPool(p.params.at(i))));
            }
            ParameterizedName pn = new ParameterizedName(params, base_qn.ns, base_qn.name)    ;
            return pn;
        }
        else
        {
            String fullName = getStringFromCPool(q.name);
            ObjectValue ns = getNamespace(q.nameSpace);
            return ctx.computeQualifiedName("", fullName, ns, EMPTY_TOKEN);
        }
    }

    DefAndSlot classTrait(int nameID, int slotID, int classID)
    {
        if( classID >= classPositions.length || classID < 0 )
        {
            return null;
        }
        DefAndSlot ret = new DefAndSlot();

        ObjectValue obj = ctx.scope();

        // Class name
        QName classQName = getQNameFromCPool(nameID);
        String className = getStringFromCPool(classQName.name);
        ObjectValue ns = getNamespace(classQName.nameSpace);

        String region_name = region_name_stack.back();
        if( region_name.length() > 0 )
        {
            region_name += "/";
            ns = ctx.getNamespace(region_name + ns.name);
        }
        int instancePos = instancePositions[classID];
        int classPos = classPositions[classID];

        int orig = buf.pos();

        // Instance name
        buf.seek(instancePos);
        long instanceNameID = buf.readU32();
        QName instanceQName = getQNameFromCPool((int)instanceNameID);
        macromedia.asc.semantics.QName fullName = getFullName(instanceQName);

        long superID = buf.readU32();
        boolean hasSuper = false;
        macromedia.asc.semantics.QName superName = null;
        String simpleSuperName ="";
        ObjectValue superNS = null;
        if( superID != 0 )
        {
            hasSuper = true;

            QName superQName = getQNameFromCPool((int)superID);
            superNS = getNamespace(superQName.nameSpace);
            superName = getFullName(superQName);
            simpleSuperName = getStringFromCPool(superQName.name);
        }

        int flags = buf.readU8();

        if ((flags & ActionBlockConstants.CLASS_FLAG_protected) != 0)
        {
        	buf.readU32();
        }

        long interfaces = buf.readU32();

        ListNode interface_nodes = null;
		
		if(debug&&interfaces>0) System.out.println("parsing " + interfaces);
        for( long i = 0; i < interfaces; ++i )
        {

//            buf.skipEntries(interfaces);
            int int_index = buf.readU32();
            QName intName = getQNameFromCPool(int_index);
			String simpleIntName = getStringFromCPool(intName.name);
			Namespaces intNamespaces;
			if(intName.nsIsSet)
			  intNamespaces = getNamespaces(intName.nameSpace);
			else {
				intNamespaces = new Namespaces(1); 
				intNamespaces.add(getNamespace(intName.nameSpace));
			}
		
            IdentifierNode ident = ctx.getNodeFactory().identifier(simpleIntName);

            ident.ref = new ReferenceValue(ctx, null, simpleIntName, intNamespaces);
            GetExpressionNode getNode = ctx.getNodeFactory().getExpression(ident);
            MemberExpressionNode interface_node = ctx.getNodeFactory().memberExpression(null, getNode);
            interface_node.ref = ident.ref;
            interface_nodes = ctx.getNodeFactory().list(interface_nodes, interface_node);
            interface_nodes.values.push_back(ident.ref);
        }
        long methID = buf.readU32();

        boolean isFinal = (flags & ActionBlockConstants.CLASS_FLAG_final) != 0;
        boolean isDynamic = ( flags & ActionBlockConstants.CLASS_FLAG_sealed ) == 0;
        boolean isInterface = (flags & ActionBlockConstants.CLASS_FLAG_interface) != 0;
        TypeValue cframe;
        ObjectValue iframe;
        ClassDefinitionNode cdn = null;
        IdentifierNode idNode = ctx.getNodeFactory().identifier(className);
        idNode.ref = new ReferenceValue(ctx, null, idNode.name, ns);
        
        AttributeListNode attr = attributeList(isFinal, false, isDynamic, ns, obj.builder);
        
        if (isInterface)
        {
        	cdn = ctx.getNodeFactory().binaryInterfaceDefinition(ctx, attr, idNode, null, ctx.getNodeFactory().statementList(null, (StatementListNode)null));
        }
        else
        {
        	cdn = ctx.getNodeFactory().binaryClassDefinition(ctx, attr, idNode, null, ctx.getNodeFactory().statementList(null, (StatementListNode)null));
        }
        cdn.ref = idNode.ref;
        cdn.interfaces = interface_nodes;
        cdn.protected_namespace = ctx.getNamespace(fullName.toString(), Context.NS_PROTECTED);
        cdn.static_protected_namespace = ctx.getNamespace(fullName.toString(), Context.NS_STATIC_PROTECTED);

        boolean is_builtin = ctx.isBuiltin(fullName.toString());
        if( is_builtin )
        {
            cdn.cframe = cframe = ctx.builtin(fullName.toString());
            cdn.iframe = iframe = cframe.prototype;
        }
        else
        {
            cdn.cframe = cframe = TypeValue.newTypeValue(ctx,new ClassBuilder(fullName,cdn.protected_namespace,cdn.static_protected_namespace),fullName,RuntimeConstants.TYPE_object);
            cdn.iframe = iframe = new ObjectValue(ctx,new InstanceBuilder(fullName),cframe);
            cframe.prototype = iframe;
            ((InstanceBuilder)iframe.builder).canEarlyBind = false;

            cdn.debug_name = fullName.toString();
            // Make sure the ClassBuilder and InstanceBuilders get cleaned up
            cdn.owns_cframe = true;
        }

        if( isInterface )
        {
            ((ClassBuilder)cdn.cframe.builder).is_interface = true;
        }
        cdn.cframe.builder.is_dynamic = cdn.iframe.builder.is_dynamic = isDynamic;
        cdn.cframe.builder.is_final = cdn.iframe.builder.is_final = isFinal;

        class_nodes.put(fullName.toString(),cdn);
        clsdefs_sets.last().add(cdn);

        if( hasSuper )
        {
            TypeValue superType;
            cdn.baseclass = ctx.getNodeFactory().literalString(superName.toString(), -1);
            if( ctx.isBuiltin(superName.toString()) )
            {
                // Set the super type, this important for int/uint which derive from Number
                superType = ctx.builtin(superName.toString());
                cframe.baseclass = superType;

                cdn.baseref = new ReferenceValue(ctx, null, superName.toString(), ctx.publicNamespace());
            }
            else
            {
                //cdn.baseclass = ctx.getNodeFactory().literalString(superName.toString(), -1);
                cdn.baseref = new ReferenceValue(ctx, null, simpleSuperName, superNS);
                cdn.baseref.getSlot(ctx);
            }
        }
        else if (cdn.cframe != ctx.objectType())
		{
				NodeFactory nf = ctx.getNodeFactory();
				cdn.baseclass = nf.memberExpression(null, nf.getExpression(nf.identifier("Object")));
				cframe.baseclass = ctx.objectType();
		}
        else
        {
            cdn.baseclass = null;
            cframe.baseclass = null;
		}
        
        cframe.builder.is_final = isFinal;
        cframe.builder.is_dynamic = isDynamic;

        int var_id  = obj.builder.Variable(ctx,obj);
        int slot_id  = obj.builder.ExplicitVar(ctx,obj,className,ns,ctx.typeType(),-1,-1,var_id);
        Slot slot = obj.getSlot(ctx,slot_id);
        slot.setObjectValue(cframe);
		slot.setImported(true);
		slot.setConst(true);		// all class definitions are const.

        ret.slot = slot;

        obj.builder.ImplicitCall(ctx,obj,slot_id,cframe,CALL_Method,-1,-1);
		obj.builder.ImplicitConstruct(ctx,obj,slot_id,cframe,CALL_Method,-1,-1);

        if( isInterface )
        {
            ((ClassBuilder)cframe.builder).is_interface = true;
            slot.setImplNode(cdn);
        }

        clsdefs_sets.add(new ObjectList<ClassDefinitionNode>());

        region_name_stack.push_back(fullName.toString());
        ctx.pushStaticClassScopes(cdn); // class
        ctx.pushScope(iframe); // instance

        StatementListNode instance_stmts = ctx.getNodeFactory().statementList(null, (StatementListNode)null);
        parseTraits(null, instance_stmts); // Traits for the instance
        cdn.instanceinits = new ObjectList<Node>(instance_stmts.items.size());
        if( instance_stmts.items.size() > 0)
        {
            cdn.instanceinits.addAll(instance_stmts.items);
        }
        // Add nodes for the constructor, which doesn't have a traits entry.
        DefinitionNode iinit_node = methodTrait("$construct", ctx.publicNamespace(),0,(int)methID,0,0).def;
        cdn.instanceinits.add(iinit_node);

        ctx.popScope(); // instance

        buf.seek(classPos);
        buf.readU32();

        parseTraits(null, cdn.statements); // Traits for the class (statics)

        ctx.popStaticClassScopes(cdn); //class
        buf.seek(orig);
        clsdefs_sets.removeLast();
        region_name_stack.pop_back();

        ret.def = cdn;
        return ret;
    }

    void parseTraits(BinaryProgramNode node, StatementListNode statements)
    {
        long count = buf.readU32();

        for (long i = 0; i < count; i++)
        {
            long nameID = buf.readU32();
            int kind = buf.readU8();
            int tag = kind & 0x0f;
            int attrs;
            long slotID, typeID, valueID, methInfo, dispID, classID;
            int value_kind = 0;

            DefAndSlot d = null;
            switch (tag)
            {
            case ActionBlockConstants.TRAIT_Var:
            case ActionBlockConstants.TRAIT_Const:
                slotID = buf.readU32();
                typeID = buf.readU32();
                valueID = buf.readU32();
                if( valueID > 0)
                    value_kind = buf.readU8();
                d = slotTrait((int)nameID, (int)slotID, (int)typeID, (int)valueID, value_kind, (tag == ActionBlockConstants.TRAIT_Const));
	            statements.items.add(d.def);
	            if (node != null)
	            {
		            QName qName = getQNameFromCPool((int)nameID);
		            String simpleName = getStringFromCPool(qName.name);
		            ObjectValue ns = getNamespace(qName.nameSpace);

		            node.toplevelDefinitions.add(new macromedia.asc.semantics.QName(ns, simpleName));
	            }
                break;
            case ActionBlockConstants.TRAIT_Method:
            case ActionBlockConstants.TRAIT_Getter:
            case ActionBlockConstants.TRAIT_Setter:
                dispID = buf.readU32();
                methInfo = buf.readU32();
                attrs = (kind >> 4);
                d = methodTrait((int)nameID, (int)dispID, (int)methInfo, attrs, tag);
                statements.items.add(d.def);
	            if (node != null)
	            {
		            QName qName = getQNameFromCPool((int)nameID);
		            String simpleName = getStringFromCPool(qName.name);
		            ObjectValue ns = getNamespace(qName.nameSpace);

		            node.toplevelDefinitions.add(new macromedia.asc.semantics.QName(ns, simpleName));
	            }
                break;
            case ActionBlockConstants.TRAIT_Class:
                slotID =  buf.readU32();
                classID = buf.readU32();
                d = classTrait((int)nameID, (int)slotID, (int)classID);
                statements.items.add(d.def);
	            if (node != null)
	            {
		            QName qName = getQNameFromCPool((int)nameID);
		            String simpleName = getStringFromCPool(qName.name);
		            ObjectValue ns = getNamespace(qName.nameSpace);

		            node.toplevelDefinitions.add(new macromedia.asc.semantics.QName(ns, simpleName));
	            }
                break;
            case ActionBlockConstants.TRAIT_Function:
                buf.skipEntries(2);
                break;
            default:
                break;
                //throw new DecoderException("Invalid trait kind: " + kind);
            }
            if( ( (kind >> 4) & ActionBlockConstants.TRAIT_FLAG_metadata) != 0 )
            {
                long metadatacount = buf.readU32();
                for( int q = 0; q < metadatacount; ++q)
                {
                    long metaIndex = buf.readU32();
                    MetaDataNode metaNode = parseMetadataInfo((int)metaIndex);
                    if( d!=null )
                    {
                        if( d.def != null )
                        {
                            metaNode.def = d.def;
                            d.def.addMetaDataNode(metaNode);
                            statements.items.add(metaNode);
                        }
                        if( d.slot != null && "Deprecated" == metaNode.id) // Should be ok since the cpool entries are interned
                        {
                            d.slot.addMetadata(metaNode);
                        }
                    }
                }
            }
        }
    }

    void parseScript(int scriptIndex, BinaryProgramNode program)
    {
        if( scriptIndex < 0 || scriptIndex >= scriptPositions.length )
        {
            return;
        }
        int orig = buf.pos();
        buf.seek( scriptPositions[scriptIndex] );
        /*long initID =*/ buf.readU32();

        parseTraits(program, program.statements);

        buf.seek(orig);

    }

    MetaDataNode parseMetadataInfo( int index )
    {
        int metaPos = metadataPositions[index];
        int orig = buf.pos();
        buf.seek(metaPos);

        long idIndex = buf.readU32();
        long valueCount = buf.readU32();

        MetaDataNode metaNode = ctx.getNodeFactory().metaData(null, -1);

        metaNode.id = this.getStringFromCPool((int)idIndex);
        if( valueCount > 0 )
        {
            IntList keys = new IntList((int)valueCount);
            IntList values = new IntList((int)valueCount);
            for(int i = 0; i < valueCount; ++i )
            {
                // read keys
                keys.add(buf.readU32());
            }
            for(int j = 0; j < valueCount; ++j )
            {
                // read keys
                values.add(buf.readU32());
            }
            metaNode.values = new Value[(int)valueCount];
            for( int k = 0; k < valueCount; ++k )
            {
                int key = keys.get(k);
                int value = values.get(k);
                Value val = null;
                if( key == 0 )
                    val = new MetaDataEvaluator.KeylessValue( getStringFromCPool(value) );
                else
                    val = new MetaDataEvaluator.KeyValuePair( getStringFromCPool(key), getStringFromCPool(value));

                metaNode.values[k] = val;
            }
        }
        buf.seek(orig);
        return metaNode;

    }
    
    String getStringFromCPool(int id)
    {
    	return cPoolStrs[id];
    }

    Decimal128 getDecimalFromCPool(int id) {
        int pos = cPoolDecimalPositions[id];
        int orig = buf.pos();
        buf.seek(pos);
        byte rep[] = buf.readBytes(16);
    	Decimal128 dval = new Decimal128(rep);
        buf.seek(orig);
    	return dval;
    }
    
    double getNumberFromCPool(int id, int kind)
    {
        double ret = 0.0;
        int[] cpool = null;
        switch(kind)
        {
            case ActionBlockConstants.CONSTANT_Integer:
                cpool = cPoolIntPositions;
                break;
            case ActionBlockConstants.CONSTANT_UInteger:
                cpool = cPoolUIntPositions;
                break;
            case ActionBlockConstants.CONSTANT_Double:
                cpool = cPoolDoublePositions;
                break;
        }

        if( cpool != null )
        {
            int pos = cpool[id];
            int orig = buf.pos();
            buf.seek(pos);
			switch (kind)
			{
            case ActionBlockConstants.CONSTANT_Integer:
                ret = buf.readU32();
                break;
            case ActionBlockConstants.CONSTANT_UInteger:
				ret = buf.readU32() & 0xFFFFFFFFL;
                break;
            case ActionBlockConstants.CONSTANT_Double:
                ret = buf.readDouble();
                break;
			}
            buf.seek(orig);
        }
        return ret;
    }

    static class QName
    {
        int name = 0;
        int nameSpace = 0;
        boolean nsIsSet = false;
    };

    static class ParameterizedQName extends QName
    {
        IntList params;

        ParameterizedQName(int name, IntList params)
        {
            this.name = name;
            this.params = params;
        }
    }

    QName getQNameFromCPool(int index)
    {
        int orig = buf.pos();
        QName value = new QName();

        buf.seek( cPoolMnPositions[index] );
        int kind = buf.readU8();

        switch(kind)
        {
        case ActionBlockConstants.CONSTANT_Qname:
        case ActionBlockConstants.CONSTANT_QnameA:
            value.nameSpace = buf.readU32();
            value.name = buf.readU32();
            break;
        case ActionBlockConstants.CONSTANT_TypeName:
            int name = buf.readU32();
            int count = buf.readU32();
            IntList params = new IntList(count);
            for( int i = 0; i < count; ++i )
                params.add(buf.readU32());
            value = new ParameterizedQName(name, params);
            break;
        case ActionBlockConstants.CONSTANT_Multiname:
        case ActionBlockConstants.CONSTANT_MultinameA:
                value.name = buf.readU32();
                value.nameSpace = buf.readU32();
                value.nsIsSet = true;
            break;
        default:
        	System.err.println("Unexpected multiname type: " + kind);
            break;
        }
        buf.seek(orig);
        return value;
    }

    Namespaces getNamespaces(int namespaceSetID)
    {
        int orig = buf.pos();

        buf.seek( cPoolNsSetPositions[namespaceSetID] );

        int count = buf.readU32();
        Namespaces val = new Namespaces(count);
        for(int i = 0; i < count; ++i)
        {
            val.add(getNamespace(buf.readU32()));
        }
        buf.seek(orig);
        return val;
    }

    ObjectValue getNamespace(int namespaceID)
    {
        ObjectValue ns = null;
        if( namespaceID == 0 )
        {
            //
            ns = ctx.anyNamespace();
        }
        else
        {
            int orig = buf.pos();

            buf.seek( cPoolNsPositions[namespaceID] );

            int kind = buf.readU8();
            switch(kind)
            {
            case ActionBlockConstants.CONSTANT_Namespace:
                ns = ctx.getNamespace(getStringFromCPool(buf.readU32()));
                break;
            case ActionBlockConstants.CONSTANT_PackageNamespace:
                ns = ctx.getNamespace(getStringFromCPool(buf.readU32()));
                ns.setPackage(true);
                break;
            case ActionBlockConstants.CONSTANT_ProtectedNamespace:
                ns = ctx.getNamespace(getStringFromCPool(buf.readU32()), Context.NS_PROTECTED);
                break;
            case ActionBlockConstants.CONSTANT_PackageInternalNs:
                ns = ctx.getNamespace(getStringFromCPool(buf.readU32()), Context.NS_INTERNAL);
                break;
            case ActionBlockConstants.CONSTANT_PrivateNamespace:
                ns = ctx.getNamespace(getStringFromCPool(buf.readU32()), Context.NS_PRIVATE);
                break;
            case ActionBlockConstants.CONSTANT_ExplicitNamespace:
                ns = ctx.getNamespace(getStringFromCPool(buf.readU32()), Context.NS_EXPLICIT);
                break;
            case ActionBlockConstants.CONSTANT_StaticProtectedNs:
                ns = ctx.getNamespace(getStringFromCPool(buf.readU32()), Context.NS_STATIC_PROTECTED);
                break;

            }
            buf.seek(orig);
        }
        return ns;
    }

/*
    // These dump functions below are just some debugging aids.
    private void dumpProgram(BinaryProgramNode program)
    {

        if( program.statements != null)
        {
            dumpStatementList(program.statements);
        }
    }

    private void dumpClass(BinaryClassDefNode bincls)
    {
        if( bincls.attrs != null)
            System.out.print(attrsToString(bincls.attrs));
        System.out.print("class " + bincls.cframe.name);
        if( bincls.baseref != null)
        {
            System.out.print(" extends " + typeRefToString(bincls.baseref));
        }
        if( bincls.interfaces != null )
        {
            System.out.print(" implements " );
            for( int i = 0; i < bincls.interfaces.size(); ++ i)
            {
                Node n = bincls.interfaces.items.get(i);
                if( n instanceof MemberExpressionNode )
                {
                    System.out.print( typeRefToString(((MemberExpressionNode)n).ref));
                }
            }
        }
        System.out.println("");
        System.out.println("{");
        if( bincls.statements != null)
        {
            dumpStatementList(bincls.statements);
        }

        if( bincls.instanceinits != null)
        {
            dumpNodeList(bincls.instanceinits);
        }

        System.out.println("} //"+bincls.cframe.name);
    }

    private void dumpStatementList(StatementListNode stmts)
    {
        dumpNodeList(stmts.items);
    }
    private void dumpNodeList(ObjectList<Node> stmts)
    {
        for( Node n : stmts)
        {
            if( n instanceof FunctionDefinitionNode )
                dumpFunction((FunctionDefinitionNode)n);
            else if( n instanceof VariableDefinitionNode )
                dumpVar((VariableDefinitionNode)n);
            else if( n instanceof BinaryClassDefNode)
                dumpClass((BinaryClassDefNode)n);
            else if( n instanceof NamespaceDefinitionNode)
                dumpNamespace((NamespaceDefinitionNode)n);
        }
    }
    private void dumpFunction(FunctionDefinitionNode funcDef)
    {

        if( funcDef.attrs != null)
            System.out.print(attrsToString(funcDef.attrs));
        System.out.print("function ");
        if( funcDef.fexpr.kind == Tokens.GET_TOKEN)
            System.out.print("get ");
        else if (funcDef.fexpr.kind == Tokens.SET_TOKEN)
            System.out.print("set ");
        System.out.print( typeRefToString(funcDef.fexpr.identifier.ref) );
        System.out.print("(");
        if(funcDef.fexpr.signature.parameter != null)
        {
            ParameterListNode params = funcDef.fexpr.signature.parameter;
            for(int i = 0; i < params.size(); ++ i)
            {
                ParameterNode param = params.items.get(i);
                if(param != null && param.typeref != null)
                {
                    System.out.print(typeRefToString(param.typeref) );
                }
                System.out.print(", ");
            }
        }
        System.out.print(")");
        if( funcDef.fexpr.signature.result != null)
        {
            System.out.println( " : " + typeRefToString( ((MemberExpressionNode)funcDef.fexpr.signature.result).ref) );
        }
    }

    private void dumpVar(VariableDefinitionNode varDef)
    {
        if(varDef.attrs != null)
            System.out.print(attrsToString(varDef.attrs));
        Node first = varDef.list.items.at(0);
        System.out.print(" " + typeRefToString( ((VariableBindingNode)first).ref));
        System.out.println(" : " + typeRefToString( ((VariableBindingNode)first).typeref) );
//        /System.out.println("  Name: " + varDef.);
    }

    private void dumpNamespace(NamespaceDefinitionNode nsDef)
    {
        System.out.print( attrsToString(nsDef.attrs) );
        System.out.println( " namespace " + nsDef.name.name + " = \"" + ((LiteralStringNode)nsDef.value).value + "\"");

    }
    private String attrsToString(AttributeListNode attr)
    {
        String ret = "";
        if( attr.hasStatic )
            ret += "static ";
        if( attr.hasPublic)
            ret += "public ";
        else if( attr.hasPrivate)
            ret += "private ";
        if(attr.hasOverride)
            ret += " override ";
        if(attr.hasFinal)
            ret += " final ";
        if(attr.namespaces.size() > 0)
            ret += attr.namespaces.at(0).name + " ";

        return ret;
    }
    private String typeRefToString(ReferenceValue typeref)
    {
        String value = "";
        if( typeref.getImmutableNamespaces().size() == 1)
        {
            ObjectValue ns = typeref.getImmutableNamespaces().first();
            if( ns != null && !ns.name.equals("") )
            {
                value = ns.name + ":";
            }
            value += typeref.name;
        }
        else
        {
            value += "{";
            for( int i = 0; i < typeref.getImmutableNamespaces().size(); ++i )
            {
                ObjectValue ns = typeref.getImmutableNamespaces().get(i);
                value += ns.name + ", ";
            }
            value += "}:" + typeref.name;
        }
        return value;
    }
*/
}

