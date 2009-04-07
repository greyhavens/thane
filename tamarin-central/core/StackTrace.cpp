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
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
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
#ifdef DEBUGGER
	void CallStackNode::init(
					MethodEnv*				env
					, Atom*					framep
					, Traits**				frameTraits
					, intptr_t volatile*	eip
					, bool                  boxed
			)
	{
		AvmAssert(env != NULL);
		m_core			= env->core();
		m_env			= env;
		m_next			= m_core->callStack; m_core->callStack = this;
		m_fakename		= NULL;
		m_depth			= m_next ? (m_next->m_depth + 1) : 1;
		m_eip			= eip;     // ptr to where the current instruction pointer is stored
		m_filename		= NULL;
		m_framep		= framep;
		m_traits		= frameTraits;
		m_linenum		= 0;
		m_boxed			= boxed;
	}

	void CallStackNode::init(AvmCore* core, Stringp name)
	{
		// careful, core and/or name can be null
		m_env			= NULL;
		m_fakename		= name;
		if (name)
		{
			AvmAssert(core != NULL);
			m_core		= core;
			m_next		= core->callStack; core->callStack = this;
			m_depth		= m_next ? (m_next->m_depth + 1) : 1;
		}
		else
		{
			m_core		= NULL;
			m_next		= NULL;
			m_depth		= 0;
		}
		m_eip			= 0;    
		m_filename		= 0;
		m_framep		= 0;
		m_traits		= 0;
		m_linenum		= 0;
		m_boxed			= false;
	}

	void FASTCALL CallStackNode::exit()
	{
		// m_env might be null (for fake CallStackNode), be careful
		AvmAssert(m_core != NULL);
		m_core->callStack = m_next;
		m_next = NULL;
		m_core = NULL; // so the dtor doesn't call exit() again
	}

	CallStackNode::~CallStackNode()
	{
		// The destructor /must not/ do anything except call reset()
		reset();
	}
	
	void FASTCALL  CallStackNode::reset()
	{
		AvmCore* core = m_core; // save it since exit() resets to null
		if (core)
		{
			exit();
			core->sampleCheck();
		}
	}

	void** FASTCALL CallStackNode::scopeBase()
	{
		// If we were given a real frame, calculate the scope base; otherwise return NULL
		if (m_framep && m_env)
		{
			return (void**) (m_framep + m_env->method->getMethodSignature()->local_count());
		}
		return NULL;
	}

	// Dump a filename.  The incoming filename is of the form
	// "C:\path\to\package\root;package/package;filename".  The path format
	// will depend on the platform on which the movie was originally
	// compiled, NOT the platform the the player is running in.
	//
	// We want to replace the semicolons with path separators.  We'll take
	// a guess at the appropriate path separator of the compilation
	// platform by looking for any backslashes in the path.  If there are
	// any, then we'll assume backslash is the path separator.  If not,
	// we'll use forward slash.
	void StackTrace::dumpFilename(Stringp _filename, PrintWriter& out) const
	{
		StringIndexer filename(_filename);
		
		wchar semicolonReplacement = '/';
		int length = filename->length();
		wchar ch;
		int i;

		// look for backslashes; if there are any, then semicolons will be
		// replaced with backslashes, not forward slashes
		for (i=0; i<length; ++i) {
			ch = filename[i];
			if (ch == '\\') {
				semicolonReplacement = '\\';
				break;
			}
		}

		// output the entire path
		bool previousWasSlash = false;
		for (i=0; i<length; ++i) {
			ch = filename[i];
			if (ch == ';') {
				if (previousWasSlash)
					continue;
				ch = semicolonReplacement;
				previousWasSlash = true;
			} else if (ch == '/' || ch == '\\') {
				previousWasSlash = true;
			} else {
				previousWasSlash = false;
			}
			out << ch;
		}
	}

	static Stringp getStackTraceLine(MethodInfo* method, Stringp filename) 
	{
		AvmCore *core = method->pool()->core;
		Stringp s = core->newStringLatin1("\tat ");
		s = core->concatStrings(s, method->format(core));
		if (filename)
		{
			s = s->appendLatin1("[");
			s = s->append(filename);
			s = s->appendLatin1(":");
		}
		return s;
	}

	Stringp StackTrace::format(AvmCore* core)
	{
		if(!stringRep)
		{
			Stringp s = core->kEmptyString;
			int displayDepth = depth;
			if (displayDepth > kMaxDisplayDepth) {
				displayDepth = kMaxDisplayDepth;
			}
			const Element *e = elements;
			for (int i=0; i<displayDepth; i++, e++)
			{
				// env will be NULL if the element is from a fake CallStackNode
				// omit them since they are only for profiling purposes
				if (!e->info())
					continue;

				if(i != 0)
					s = s->appendLatin1("\n");

				Stringp filename=NULL;
				if(e->filename())
				{
					StringBuffer sb(core->gc);
					dumpFilename(e->filename(), sb);
					filename = core->newStringUTF8(sb.c_str());
				}
				s = core->concatStrings(s, getStackTraceLine(e->info(), filename));
				if(e->filename())
				{
					s = core->concatStrings(s, core->intToString(e->linenum()));
					s = s->appendLatin1("]");
				}
			}
			stringRep = s;
		}
		return stringRep;
	}

#endif 
}
