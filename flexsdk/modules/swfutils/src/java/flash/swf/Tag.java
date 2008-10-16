////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2003-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * Base class for all player tags
 * @author Clement Wong
 */
public abstract class Tag
        implements TagValues
{
	final public int code;

    public Tag(int code)
    {
        this.code = code;
    }

    /**
	 * Subclasses implement this method to callback one of the methods in TagHandler...
	 * @param h
	 */
	public abstract void visit(TagHandler h);

	/**
	 * many tags have zero or one reference, in which case they only need
	 * to override this method.  Tags that have two or more references
	 * should override getReferences() and provide an Iterator.
	 * @return
	 */
	protected Tag getSimpleReference()
    {
        return null;
    }

    /**
     * Find the immediate dependencies.  unlike visitDefs, it doesn't explore the entire tree.
     * The user must do a recursive walk if they care to go beyond the first order dependencies.
     * The default implementation provides an iterator over a single simple reference, defined
     * by the derived class via the getSimpleReference() call.
     * @return An iterator over the first order Tag dependencies.
     */

    public Iterator<Tag> getReferences()
    {
        return new Iterator<Tag>()
        {
            private boolean done = false;

            public boolean hasNext()
            {
                return (!((getSimpleReference() == null) || done));
            }
            public Tag next()
            {
                if ( hasNext() )
                {
                    done = true;
                    return getSimpleReference();
                }
                throw new NoSuchElementException();
            }
            public void remove()
            {
                throw new UnsupportedOperationException();
            }
        };
    }

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof Tag)
        {
            Tag tag = (Tag) object;

            if (tag.code == this.code)
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    public int hashCode()
    {
        return code;
    }

    public static boolean equals(Object o1, Object o2)
    {
        return o1 == o2 || o1 != null && o1.equals(o2);
    }
}
