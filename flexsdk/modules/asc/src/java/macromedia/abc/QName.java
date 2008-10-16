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

/**
 * @author Clement Wong
 */
public final class QName
{
	public QName(final String namespaceURI, final String localPart)
	{
		this.namespaceURI = namespaceURI;
		this.localPart = localPart;
	}

	private String namespaceURI;
	private String localPart;

	public String getNamespace()
	{
		return namespaceURI;
	}

	public String getLocalPart()
	{
		return localPart;
	}

	public boolean equals(Object obj)
	{
		if (obj instanceof QName)
		{
			QName qName = (QName) obj;
			return namespaceURI.equals(qName.namespaceURI) && localPart.equals(qName.localPart);
		}
		else
		{
			return false;
		}
	}

	public int hashCode()
	{
		return namespaceURI.hashCode() ^ localPart.hashCode();
	}

	public String toString()
	{
		return (namespaceURI == null || namespaceURI.length() == 0) ? localPart : namespaceURI + ":" + localPart;
	}
}
