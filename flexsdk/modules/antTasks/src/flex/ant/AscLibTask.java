////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2006-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flex.ant;

import flex.ant.config.ConfigVariable;
import flex.ant.config.ConfigBoolean;
import flex.ant.config.ConfigString;
import flex.ant.config.OptionSpec;
import flex.ant.config.OptionSource;
import flex.ant.types.FlexFileSet;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.DynamicAttribute;
import org.apache.tools.ant.types.Commandline;

import java.util.ArrayList;
import java.util.Iterator;

/**
 *
 */
public final class AscLibTask extends FlexTask implements DynamicAttribute
{
    /*=======================================================================*
     *  Static variables and initializer                                     *
     *=======================================================================*/

    private static ConfigVariable[] ASC_VARIABLES;

    private static OptionSpec imSpec = new OptionSpec("import");
    private static OptionSpec inSpec = new OptionSpec("in");

    static
    {
        ASC_VARIABLES = new ConfigVariable[] {
            //Boolean Variables
            new ConfigBoolean(new OptionSpec(null, "AS3", "as3")),
            new ConfigBoolean(new OptionSpec("i")),
            new ConfigBoolean(new OptionSpec("strict")),
            new ConfigBoolean(new OptionSpec("optimize")),
            new ConfigString(new OptionSpec("out")),
            new ConfigString(new OptionSpec("outdir")),
        };
    }

    /*=======================================================================*
     *
     *=======================================================================*/

    private final ArrayList<FlexFileSet> nestedFileSets;

    private FlexFileSet fileSpec;

    /**
     *
     */
    public AscLibTask()
    {
        super("asclib", "macromedia.asc.embedding.ScriptCompiler", "asc.jar", ASC_VARIABLES);

        nestedFileSets = new ArrayList<FlexFileSet>();
    }

    /*=======================================================================*
     *  Child Elements                                                       *
     *=======================================================================*/

    public FlexFileSet createImport()
    {
        return createFileSet(imSpec);
    }

    public FlexFileSet createIn()
    {
        return createFileSet(inSpec);
    }

    public FlexFileSet createFilespec()
    {
        if (fileSpec == null)
            return fileSpec = new FlexFileSet();
        else
            throw new BuildException("Only one nested <filespec> element is allowed in an <asclib> task.");
    }

    private FlexFileSet createFileSet(OptionSpec spec)
    {
        FlexFileSet e = new FlexFileSet(spec);
        nestedFileSets.add(e);
        return e;
    }

    /*=======================================================================*
     *  Execute and Related Functions                                        *
     *=======================================================================*/

    protected void prepareCommandline()
    {
        for (int i = 0; i < variables.length; i++) {
            variables[i].addToCommandline(cmdl);
        }

        Iterator<FlexFileSet> it = nestedFileSets.iterator();

        while (it.hasNext()) {
            it.next().addToCommandline(cmdl);
        }

        fileSpec.addToCommandline(cmdl);
    }
} //End of AscLibTask
