////////////////////////////////////////////////////////////////////////////////
//
//  ADOBE SYSTEMS INCORPORATED
//  Copyright 2004-2006 Adobe Systems Incorporated
//  All Rights Reserved.
//
//  NOTICE: Adobe permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////

package flash.util;

/**
 * Format a sequence of characters into a fixed length field
 * justifying if applied.
 */
public class FieldFormat
{
	public static final int ALIGN_UNKNOWN	= 0;
	public static final int ALIGN_LEFT		= 1;
	public static final int ALIGN_RIGHT		= 2;
	public static final int ALIGN_CENTER	= 3;

	 // Right justifies a long value into a hex field with leading zeros
	public static StringBuilder formatLongToHex(StringBuilder sb, long v, int length) { return formatLongToHex(sb,v,length,true); }

	 // Right justifies a long value into a field optionally zero filling the opening. 
	public static StringBuilder formatLongToHex(StringBuilder sb, long v, int length, boolean leadingZeros)
	{
		return format(sb, Long.toHexString(v), length, ALIGN_RIGHT, ((leadingZeros) ? '0' : ' '), ' ');
	}

	 // Right justifies a long value into a fixed length field
	public static StringBuilder formatLong(StringBuilder sb, long v, int length) { return formatLong(sb,v,length,false); }
	public static StringBuilder formatLong(StringBuilder sb, long v, int length, boolean leadingZeros)	
	{ 
		return format(sb, Long.toString(v), length, ((leadingZeros) ? ALIGN_RIGHT : ALIGN_LEFT), ((leadingZeros) ? '0' : ' '), ' '); 
	}

	// basis for all formats 
	public static StringBuilder format(StringBuilder sb, String chars, int length, int alignment, char preFieldCharacter, char postFieldCharacter)
	{
		// find the length of our string 
		int charsCount = chars.length();

		// position within the field
		int startAt = 0;
		if (alignment == ALIGN_RIGHT)
			startAt = length - charsCount;
		else if (alignment == ALIGN_CENTER)
			startAt = (length - charsCount)/2;

		// force to right it off edge
		if (startAt < 0)
			startAt = 0;

		// truncate it
		if ((startAt + charsCount) > length)
			charsCount = length-startAt;

		// now add the pre-field if any
		for(int i=0; i<startAt; i++)
			sb.append(preFieldCharacter);

		// the content
		sb.append(chars.substring(0, charsCount));

		// post field if any
		for(int i=startAt+charsCount; i<length; i++)
			sb.append(postFieldCharacter);

		return sb;
	}
	
}
