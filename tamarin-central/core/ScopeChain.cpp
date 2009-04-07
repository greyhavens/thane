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

// FIXME the following is required because FrameState has dependencies on the jitters
#if defined FEATURE_NANOJIT
    #include "CodegenLIR.h"
#endif

#include "FrameState.h"

namespace avmplus
{
	/*static*/ ScopeTypeChain* ScopeTypeChain::create(MMgc::GC* gc, const ScopeTypeChain* outer, const FrameState* state, Traits* append, Traits* extra)
	{
		const int stateScopeDepth = (state ? state->scopeDepth : 0);
		const int capture = stateScopeDepth + (append ? 1 : 0);
		const int extraEntries = extra ? 1 : 0;
		const int outerSize = (outer ? outer->size : 0);
		const int pad = capture + extraEntries;
		const size_t padSize = sizeof(uintptr_t) * (((pad > 0) ? (pad - 1) : 0) + outerSize);
		ScopeTypeChain* nscope = new(gc, padSize) ScopeTypeChain(outerSize + capture, outerSize + capture + extraEntries);
		int j = 0;
		for (int i = 0; i < outerSize; i++)
		{
			nscope->_scopes[j++] = outer->_scopes[i];
		}
		for (int i = 0; i < stateScopeDepth; i++)
		{
			const Value& v = state->scopeValue(i);
			nscope->setScopeAt(j++, v.traits, v.isWith);
		}
		if (append)
		{
			nscope->setScopeAt(j++, append, false);
		}
		if (extra)
		{
			nscope->setScopeAt(j++, extra, false);
		}
		AvmAssert(j == nscope->fullsize);
		return nscope;
	}

	#if VMCFG_METHOD_NAMES
	Stringp ScopeTypeChain::format(AvmCore* core) const
	{
		Stringp r = core->kEmptyString;
		r = r->appendLatin1("STC:[");
		for (int i = 0; i < fullsize; i++)
		{
			if (i > 0)
				r = r->appendLatin1(",");
			Traits* t = getScopeTraitsAt(i);
			bool b = getScopeIsWithAt(i);
			r = r->append(t->format(core));
			r = r->appendLatin1(b?":1":":0");
		}
		r = r->appendLatin1("]");
		return r;
	}
	#endif

	/*static*/ ScopeChain* ScopeChain::create(MMgc::GC* gc, const ScopeTypeChain* scopeTraits, const ScopeChain* outer, Namespacep dxns)
	{
		const int scopeTraitsSize = scopeTraits->size;
		const int outerSize = outer ? outer->_scopeTraits->size : 0;
		const size_t padSize = scopeTraitsSize > 0 ? sizeof(Atom) * (scopeTraitsSize-1) : 0;
		ScopeChain* nscope = new(gc, padSize) ScopeChain(scopeTraits, dxns);
		for (int i=0; i < outerSize; i ++)
		{
			nscope->setScope(gc, i, outer->_scopes[i]);
		}
		return nscope;
	}

	void ScopeChain::setScope(MMgc::GC* gc, int i, Atom value)
	{
		AvmAssert(i >= 0 && i < _scopeTraits->size);
		//scopes[i] = value;
		WBATOM(gc, this, &_scopes[i], value);
	}

	#if VMCFG_METHOD_NAMES
	Stringp ScopeChain::format(AvmCore* core) const
	{
		Stringp r = core->kEmptyString;
		r = r->appendLatin1("SC:{dxns=(");
		r = r->append(_defaultXmlNamespace->format(core));
		r = r->appendLatin1("),");
		r = r->append(_scopeTraits->format(core));
		r = r->appendLatin1(",V:[");
		for (int i = 0; i < _scopeTraits->size; i++)
		{
			if (i > 0)
				r = r->appendLatin1(",");
			r = r->append(core->format(_scopes[i]));
		}
		r = r->appendLatin1("]}");
		return r;
	}
	#endif
}
