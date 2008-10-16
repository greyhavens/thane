////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2003-2007 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.swf.types;

import java.util.List;

import flash.swf.tags.DefineTag;

/**
 * @author Clement Wong
 */
public class ButtonRecord
{
	public boolean hitTest;
	public boolean down;
	public boolean over;
	public boolean up;

    public DefineTag characterRef;
	public int placeDepth;
	public Matrix placeMatrix;
	
    /** only valid if this record is in a DefineButton2 */
	public CXFormWithAlpha colorTransform;
	public List filters;
	public int blendMode = -1; // -1 ==> not set

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof ButtonRecord)
        {
            ButtonRecord buttonRecord = (ButtonRecord) object;

            if ( (buttonRecord.hitTest == this.hitTest) &&
                 (buttonRecord.down == this.down) &&
                 (buttonRecord.over == this.over) &&
                 (buttonRecord.up == this.up) &&
                 (buttonRecord.blendMode == this.blendMode) &&
                 compareFilters(buttonRecord.filters, this.filters) &&
                 ( ( (buttonRecord.characterRef == null) && (this.characterRef == null) ) ||
                   ( (buttonRecord.characterRef != null) && (this.characterRef != null) &&
                     buttonRecord.characterRef.equals(this.characterRef) ) ) &&
                 (buttonRecord.placeDepth == this.placeDepth) &&
                 ( ( (buttonRecord.placeMatrix == null) && (this.placeMatrix == null) ) ||
                   ( (buttonRecord.placeMatrix != null) && (this.placeMatrix != null) &&
                     buttonRecord.placeMatrix.equals(this.placeMatrix) ) ) &&
                 ( ( (buttonRecord.colorTransform == null) && (this.colorTransform == null) ) ||
                   ( (buttonRecord.colorTransform != null) && (this.colorTransform != null) &&
                     buttonRecord.colorTransform.equals(this.colorTransform) ) ) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

    private boolean compareFilters(List filterList1, List filterList2)
    {
    	if (filterList1 == filterList2) return true;
    	if (filterList1 == null || filterList2 == null) return false;
    	if (filterList1.size() != filterList2.size()) return false;
    	for (int i = 0, size = filterList1.size(); i < size; i++)
    	{
    		// TODO: should really be comparing content...
    		if (filterList1.get(i) != filterList2.get(i))
    		{
    			return false;
    		}
    	}
    	return true;
    }
    
    public String getFlags()
    {
        StringBuilder b = new StringBuilder();
        if (blendMode != -1) b.append("hasBlendMode,");
        if (filters != null) b.append("hasFilterList,");
        if (hitTest) b.append("hitTest,");
        if (down) b.append("down,");
        if (over) b.append("over,");
        if (up) b.append("up,");
        if (b.length() > 0)
            b.setLength(b.length()-1);
        return b.toString();
    }
}
