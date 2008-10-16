////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Written by Jeff Dyer
 * Copyright (c) 1998-2003 Mountain View Compiler Company
 * All rights reserved.
 */

package macromedia.asc.embedding.avmplus;

import macromedia.asc.semantics.*;
import macromedia.asc.util.Context;
import macromedia.asc.util.Namespaces;
import macromedia.asc.parser.NodeFactory;
/*
 * Package object builder.
 */

/**
 * @author Jeff Dyer
 */
public class ImportBuilder extends Builder
{
	public ImportBuilder(QName name)
	{
		classname = name;
	}

    public void build(Context cx, ObjectValue ob)
    {
		objectValue = ob;
		contextId = cx.getId();
        var_offset = 0;
        reg_offset = 1;  // this, in $init

        Builder globbui = new GlobalBuilder();
        globbui.build(cx,ob);
    }

    public int Variable( Context cx, ObjectValue ob )
    {

        // If building an intrinsic instance, then do nothing here.

        if( is_intrinsic )
        {
            return -1;
        }

        // front-end

        int var_id = super.Variable(cx,ob);

        // back-end

        return var_id;
    }

    public int Method( Context cx, ObjectValue ob, String name, Namespaces namespaces, boolean is_final )
    {
        // If building an intrinsic instance, then do nothing here
        if( is_intrinsic )
        {
            return -1;
        }

        return 0; //cx.getEmitter()->GetMethodId(classname+"$"+name,namespaces);
    }

	public int ExplicitVar( Context cx, ObjectValue ob, String name, Namespaces namespaces, TypeValue type, int expected_id)
	{
		return ExplicitVar(cx, ob, name, namespaces, type, expected_id,-1,-1);
	}

    public int ExplicitVar( Context cx, ObjectValue ob, String name, Namespaces namespaces, TypeValue type, int expected_id, int method_id , int var_id )
    {
        int slot_id = super.ExplicitVar(cx,ob,name,namespaces,type,expected_id,-1/*method_id*/,-1/*var_id*/);
        ob.getSlot(cx,slot_id).addType(type.getDefaultTypeInfo());
        Slot slot = ob.getSlot(cx,slot_id);
        NodeFactory nf = cx.getNodeFactory();
        slot.setBaseNode(nf.memberExpression(null,nf.getExpression(nf.identifier(classname.toString(),0))));
        return slot_id;
    }

	public int ExplicitGet( Context cx, ObjectValue ob, String name, Namespaces namespaces, TypeValue type, boolean is_final, boolean is_override)
	{
		return ExplicitGet(cx, ob, name, namespaces, type, is_final, is_override, -1, -1, -1);
	}

    public int ExplicitGet( Context cx, ObjectValue ob, String name, Namespaces namespaces, TypeValue type, boolean is_final, boolean is_override, int expected_id , int method_id , int var_id  )
    {
        int slot_id = super.ExplicitGet(cx,ob,name,namespaces,type,is_final,is_override,-1/*expected_id*/,-1/*method_id*/,-1/*var_id*/);
        Slot slot = ob.getSlot(cx,slot_id);
        NodeFactory nf = cx.getNodeFactory();
        slot.setBaseNode(nf.memberExpression(null,nf.getExpression(nf.identifier(classname.toString(),-1))));
        return slot_id;
    }

    public int ExplicitSet( Context cx, ObjectValue ob, String name, Namespaces namespaces, TypeValue type, boolean is_final, boolean is_override)
    {
        return ExplicitSet(cx, ob, name, namespaces, type, is_final, is_override, -1, -1, -1);
    }


    public int ExplicitSet( Context cx, ObjectValue ob, String name, Namespaces namespaces, TypeValue type, boolean is_final, boolean is_override, int expected_id , int method_id , int var_id  )
    {
        int slot_id = super.ExplicitSet(cx,ob,name,namespaces,type,is_final,is_override,-1/*expected_id*/,-1/*method_id*/,-1/*var_id*/);
        Slot slot = ob.getSlot(cx,slot_id);
        NodeFactory nf = cx.getNodeFactory();
        slot.setBaseNode(nf.memberExpression(null,nf.getExpression(nf.identifier(classname.toString(),-1))));
        return slot_id;
    }

    public int ExplicitCall( Context cx, ObjectValue ob, String name, Namespaces namespaces, TypeValue type, boolean is_final, boolean is_override)
    {
        return ExplicitCall(cx, ob, name, namespaces, type, is_final, is_override, -1, -1, -1);
    }


    public int ExplicitCall( Context cx, ObjectValue ob, String name, Namespaces namespaces, TypeValue type, boolean is_final, boolean is_override, int expected_id , int method_id , int var_id  )
    {
		TypeValue functionType = cx.useStaticSemantics() ? cx.functionType() : type;
		int slot_id = super.ExplicitCall(cx,ob,name,namespaces,functionType,is_final,is_override,-1/*expected_id*/,-1/*method_id*/,-1/*var_id*/);
        Slot slot = ob.getSlot(cx,slot_id);
		if (cx.useStaticSemantics())
			slot.setConst(true);
        NodeFactory nf = cx.getNodeFactory();
        slot.setBaseNode(nf.memberExpression(null,nf.getExpression(nf.identifier(classname.toString(),-1))));
        return slot_id;
    }

	public void Name(Context cx, int kind, String name, ObjectValue qualifier){}
}
