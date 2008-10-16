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

package flash.swf.types;

/**
 * @author Clement Wong
 */
public class ButtonCondAction
{
	/**
	 * SWF 4+: key code
		Otherwise: always 0
		Valid key codes:
		1: left arrow
		2: right arrow
		3: home
		4: end
		5: insert
		6: delete
		8: backspace
		13: enter
		14: up arrow
		15: down arrow
		16: page up
		17: page down
		18: tab
		19: escape
		32-126: follows ASCII
	 */
	public int keyPress;

	public boolean overDownToIdle;
	public boolean idleToOverDown;
	public boolean outDownToIdle;
	public boolean outDownToOverDown;
	public boolean overDownToOutDown;
	public boolean overDownToOverUp;
	public boolean overUpToOverDown;
	public boolean overUpToIdle;
	public boolean idleToOverUp;

	/**
	 * actions to perform when this event is detected.
	 */
	public ActionList actionList;

    public boolean equals(Object object)
    {
        boolean isEqual = false;

        if (object instanceof ButtonCondAction)
        {
            ButtonCondAction buttonCondAction = (ButtonCondAction) object;

            if ( (buttonCondAction.keyPress == this.keyPress) &&
                 (buttonCondAction.overDownToIdle == this.overDownToIdle) &&
                 (buttonCondAction.idleToOverDown == this.idleToOverDown) &&
                 (buttonCondAction.outDownToIdle == this.outDownToIdle) &&
                 (buttonCondAction.outDownToOverDown == this.outDownToOverDown) &&
                 (buttonCondAction.overDownToOutDown == this.overDownToOutDown) &&
                 (buttonCondAction.overDownToOverUp == this.overDownToOverUp) &&
                 (buttonCondAction.overUpToOverDown == this.overUpToOverDown) &&
                 (buttonCondAction.overUpToIdle == this.overUpToIdle) &&
                 (buttonCondAction.idleToOverUp == this.idleToOverUp) &&
                 ( ( (buttonCondAction.actionList == null) && (this.actionList == null) ) ||
                   ( (buttonCondAction.actionList != null) && (this.actionList != null) &&
                     buttonCondAction.actionList.equals(this.actionList) ) ) )
            {
                isEqual = true;
            }
        }

        return isEqual;
    }

	public String toString()
	{
		// return the flags as a string
		StringBuilder b = new StringBuilder();

        if (keyPress != 0)      b.append("keyPress<"+keyPress+">,");
		if (overDownToIdle)		b.append("overDownToIdle,");
		if (idleToOverDown)		b.append("idleToOverDown,");
		if (outDownToIdle)		b.append("outDownToIdle,");
		if (outDownToOverDown)	b.append("outDownToOverDown,");
		if (overDownToOutDown)	b.append("overDownToOutDown,");
		if (overDownToOverUp)	b.append("overDownToOverUp,");
		if (overUpToOverDown)	b.append("overUpToOverDown,");
		if (overUpToIdle)		b.append("overUpToIdle,");
		if (idleToOverUp)		b.append("idleToOverUp,");

        // trim trailing comma
		if (b.length() > 0)
			b.setLength(b.length()-1);

		return b.toString();
	}
}
