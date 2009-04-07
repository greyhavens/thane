/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 1993-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "avmplus.h"

namespace avmplus
{
	//
	// XMLParser
	//	

	bool XMLParser::skipWhiteSpace()
	{
		bool eof = atEnd();
		while (!eof)
		{
			wchar ch = m_str[m_pos];
			if (!String::isSpace(ch))
				break;
			m_pos++;
			eof = atEnd();
		}
		return !eof;
	}

	// !!@ I'm not sure what this was supposed to do originally but I've rewritten it
	// to remove the leading and trailing white space for text elements.  
	// "     5     4     3     " becomes "5     4     3"
	// This is to simulate the E4X XML parser
	static Stringp _condenseWhitespace(Stringp text)
	{
		StringIndexer str (text);
		// leading space
		int32_t start = 0;
		while (start < text->length() && String::isSpace(str[start]))
			start++;
		// trailing space
		int32_t end = text->length() - 1;
		while ((end > start) && String::isSpace(str[end]))
			end--;
		return text->substring(start, end + 1);
	}

	int XMLParser::getNext(XMLTag& tag)
	{
		wchar ch;
		int32_t start, end;
		tag.reset();

		// If there's nothing left, exit.
		if (atEnd()) {
			return XMLParser::kEndOfDocument;
		}

		// R41
		// If the ignore whitespace flag is on, don't produce
		// all-whitespace text nodes.
		if (m_ignoreWhite) 
		{
			if (!skipWhiteSpace())
				return XMLParser::kEndOfDocument;
		}
		// end R41

		// If it starts with <, it's an XML element.
		// If it doesn't, it must be a text element.
		start = m_pos;
		ch = m_str[m_pos];
		if (ch != '<') 
		{
			// Treat it as text.  Scan up to the next < or until EOF.
			m_pos = m_str->indexOfLatin1("<", 1, m_pos + 1);
			if (m_pos < 0)
				m_pos = m_str->length();

			tag.text = unescape(start, m_pos, false);

			// Condense whitespace if desired
			if (m_ignoreWhite && m_condenseWhite)
				tag.text = _condenseWhitespace(tag.text);

			tag.nodeType = XMLTag::kTextNodeType;
			return XMLParser::kNoError;
		}

		// Is this a <?xml> declaration?
		start = m_pos;
		if (m_str->matchesLatin1("<?xml ", 6, start))
		{
			end = m_str->indexOfLatin1("?>", 2, start + 6);
			if (end >= 0)
			{
				// We have the end of the XML declaration
				// !!@ changed to not return <?...?> parts
				tag.text = m_str->substring(start + 2, end);
				m_pos = end + 2;
				tag.nodeType = XMLTag::kXMLDeclaration;
				return XMLParser::kNoError;
			}
			return XMLParser::kUnterminatedXMLDeclaration;
		}

		// Is this a <!DOCTYPE> declaration?
		if (m_str->matchesLatin1("<!DOCTYPE", 8, start))
		{
			// Scan forward for '>', but check for embedded <>
			int32_t depth = 0;
			end = start + 1;
			while (!atEnd())
			{
				ch = m_str[end++];
				if (ch == '<')
					depth++;
				else if (ch == '>')
				{
					if (!depth) 
					{
						// We've reached the end of the DOCTYPE.
						tag.text = m_str->substring(start, end - 1);
						tag.nodeType = XMLTag::kDocTypeDeclaration;
						m_pos = end;
						return XMLParser::kNoError;
					}
					depth--;
				}
				m_pos = end;
			}
			return XMLParser::kUnterminatedDocTypeDeclaration;
		}

		// Is this a CDATA section?
		if (m_str->matchesLatin1("<![CDATA[", 9, start))
		{
			start += 9;
			end = m_str->indexOfLatin1("]]>", 3, start);
			if (end >= 0)
			{
				// We have the end of the CDATA section.
				tag.text = m_str->substring(start, end);
				tag.nodeType = XMLTag::kCDataSection;
				m_pos = end + 3;
				return XMLParser::kNoError;
			}
			return XMLParser::kUnterminatedCDataSection;
		}

		// Is this a processing instruction?
		if (m_str->matchesLatin1("<?", 2, start))
		{
			// Scan forward for "?>"
			start += 2;
			end = m_str->indexOfLatin1("?>", 2, start);
			if (end >= 0)
			{
				// We have the end of the processing instruction.
				tag.text = m_str->substring(start, end);
				tag.nodeType = XMLTag::kProcessingInstruction;
				m_pos = end + 2;
				return XMLParser::kNoError;
			}
			return XMLParser::kUnterminatedProcessingInstruction;
		}

		// Advance past the "<"
		start = ++m_pos;

		// Is this a comment?  Return a comment tag->
		if (m_str->matchesLatin1("!--", 3, start)) 
		{
			// Skip up to '-->'.
			start += 3;
			end = m_str->indexOfLatin1("-->", 3, start);
			if (end >= 0)
			{
				tag.text = m_str->substring(start, end);
				tag.nodeType = XMLTag::kComment;
				m_pos = end  + 3;
				return XMLParser::kNoError;
			}
			// Got to the end of the buffer without finding a new tag->
			return XMLParser::kUnterminatedComment;
		}

		// Extract the tag name.  Scan up to ">" or whitespace.
		start = m_pos;
		while (!atEnd())
		{
			ch = m_str[m_pos];
			if (ch == '>' || String::isSpace(ch))
				break;
			if (ch == '/' && (m_pos < m_str->length() - 1) && m_str[m_pos+1] == '>')
			{
				// Found close of an empty element.
				// Exit!
				break;
			}
			m_pos++;
		}
		if (atEnd() || m_pos == start)
			// Premature end, or empty tag name
			return XMLParser::kMalformedElement;

		tag.text = unescape(start, m_pos, true);
		tag.nodeType = XMLTag::kElementType;

		// Extract attributes.
		for (;;) 
		{
			if (!skipWhiteSpace())
				// Premature end!
				return XMLParser::kMalformedElement;

			ch = m_str[m_pos];
			if (ch == '>')
				break;

			if (ch == '/' && (m_pos < m_str->length() - 1) && m_str[m_pos+1] == '>')
			{
				// Found close of an empty element.
				// Exit!
				tag.empty = true;
				ch = m_str[++m_pos];
				break;
			}

			// Extract the attribute name.
			start = m_pos;
			while (!String::isSpace(ch) && ch != '=' && ch != '>') 
			{
				if (atEnd())
					// Premature end!
					return XMLParser::kMalformedElement;
				ch = m_str[++m_pos];
			}
			if (start == m_pos)
				// Empty attribute name?
				return XMLParser::kMalformedElement;

			Stringp attributeName = unescape(start, m_pos, true);

			if (!skipWhiteSpace())
				// No attribute value, error!
				return XMLParser::kMalformedElement;

			ch = m_str[m_pos++];
			if (ch != '=')
				// No attribute value, error!
				return XMLParser::kMalformedElement;

			if (!skipWhiteSpace())
				// No attribute value, error!
				return XMLParser::kMalformedElement;

			wchar delimiter = m_str[m_pos++];
			// Extract the attribute value.
			if (delimiter != '"' && delimiter != '\'')
				// Error; no opening quote for attribute value.
				return XMLParser::kMalformedElement;

			// Extract up to the next quote.
			start = m_pos;
			ch = 0;
			while (ch != delimiter) 
			{
				if (atEnd() || ch == '<')
					// '<' is not permitted in an attribute value
					// Changed this from kMalformedElement to kUnterminatedAttributeValue for bug 117058(105422)
					return XMLParser::kUnterminatedAttributeValue;
				ch = m_str[m_pos++];
			}

			Stringp attributeValue = unescape(start, m_pos - 1, false);

			AvmAssert(attributeName->isInterned());
			tag.attributes.add(attributeName);
			tag.attributes.add(attributeValue);
		}

		// Advance past the end > of this element.
		if (ch == '>')
			m_pos++;

		return XMLParser::kNoError;
	}

	Stringp XMLParser::unescape(int32_t start, int32_t last, bool bIntern)
	{
		if (start == last)
			return core->kEmptyString;

		int32_t bgn = m_str->indexOfLatin1("&", 1, start, last);
		int32_t end = start;
		Stringp dest = core->kEmptyString;
		while (bgn >= start && bgn < last)
		{
			int32_t ampEnd = m_str->indexOfLatin1(";", 1, ++bgn, last);
			if (ampEnd < 0)
				// &xxx without semicolon - we are done
				break;
			// add the text between the last sequence and this sequence
			dest = String::concatStrings(dest, m_str->substring(end, bgn-1));
			end = ampEnd;
			int32_t len = end - bgn;
			// an &xx; sequence is at least two characters
			bool ok = true;
			if (len >= 2)
			{
				int32_t ch = m_str[bgn];
				if (ch == '#')
				{
					// Parse a &#xx; decimal sequence.  Or a &#xDD hex sequence
					ch = m_str[++bgn];
					len--;
					int base = 10;
					if (len >= 2 && ch == 'x')
						base = 16, bgn++, len--;
					if (len > 0)
					{
						int32_t value = 0;
						while (len-- && ok)
						{
							ch = m_str[bgn++];
							if (ch >= 'A' && ch <= 'F')
								ch -= 7;
							ch -= '0';
							if (ch >= 0 && ch < base)
								value = (value * base) + ch;
							else
								ok = false;
							if (value > 0xFFFF)
								ok = false;
						}
						if (ok)
						{
							wchar c = (wchar) value;
							// note: this code is allowed to construct a string
							// containing illegal UTF16 sequences!
							dest = dest->append16(&c, 1);
							bgn = ++end;
						}
					}
				} 
				else if (len <= 4) // Our xmlEntities are only 4 characters or less
				{
					Atom entityAtom = core->internString(m_str->substring(bgn, end))->atom();
					Atom result = core->xmlEntities->get(entityAtom);
					if (result != undefinedAtom) 
					{
						AvmAssert(atomKind(result) == kIntegerType);
						wchar c = (wchar) (result>>3);
						// note: this code is allowed to construct a string
						// containing illegal UTF16 sequences!
						dest = dest->append16(&c, 1);
						bgn = ++end;
					}
					else
						ok = false;
				}
				else
					ok = false;
			}
			if (!ok)
				bgn = end + 1;
			bgn = m_str->indexOfLatin1("&", 1, bgn, last);
		}
		// add any remaining text
		if (end < last)
			dest = String::concatStrings(dest, m_str->substring(end, last));

		return (bIntern) ? core->internString(dest) : dest;
	}

	XMLParser::XMLParser(AvmCore *core, Stringp str) : m_str (str), m_pos (0)
	{
		this->core = core;

		if (!core->xmlEntities)
		{
			// Lazy creation of the XML entities table.
			core->xmlEntities = new (core->GetGC()) Hashtable(core->GetGC());

			const char *entities = "&amp\0\"quot\0'apos\0<lt\0>gt\0\xA0nbsp\0";
		
			while (*entities)
			{
				core->xmlEntities->add(core->internConstantStringLatin1(entities+1)->atom(),
							   (void*)core->intToAtom(*entities));
				while (*entities++) {
					// do nothing
				}
			}
		}
	}

	void XMLParser::parse(bool ignoreWhite /*=false*/ )
	{
		m_pos = 0;
		m_ignoreWhite = ignoreWhite;
	}

	bool XMLTag::nextAttribute(uint32& index,
							   Stringp& name,
							   Stringp& value)
	{
		if (index >= attributes.size()) {
			return false;
		}
		name  = attributes.get(index++);
		value = attributes.get(index++);
		return true;
	}

} // namespace
