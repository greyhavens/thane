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
*   Adobe AS3 Team. Modified by Shengnan Cong@Intel.
*   Tamarin. Refactored by Rick Reitmaier
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

#ifdef VTUNE

namespace avmplus
{
	void wtoc(char* Dest, const char* Source, int append)
	{
		int i = 0;
		if (Source) {
			if (append) {
				int j = -1;
				while(Source[i] != '\0')
				{
					Dest[i] = Source[i];
					if (Dest[i] == ':') {
						//Dest[i] = '|';
						j = i;
					} else if (Dest[i] == '.') {
						//Dest[i] = '-';
						j = i;
					} else if (Dest[i] == '$') {
						j = i;
					}
					++i;
				}

				// extract just the method name
				if (j >= 0) {
					i = 0;
					while(Source[++j] != '\0')
					{
						Dest[i++] = Source[j];
					}
				}

				Dest[i++] = '(';
				Dest[i++] = ')';
			} else {
				while(Source[i] != '\0')
				{
					Dest[i] = Source[i];
					++i;
				}
			}
		}
		Dest[i] = '\0';
	}

	void CallBack(AvmCore* core, void *UserData, iJIT_ModeFlags Flags)
	{
		// This is all busy work to prevent the resulting warnings
		// from causing an error.  I don't want to change the build
		// settings.
		AvmCore* dummy_core;
		void* dummy_user;
		iJIT_ModeFlags dummy_flags;

		dummy_core = core;
		dummy_user = UserData;
		dummy_flags = Flags;
	}

	void DumpVTuneMethodInfo(AvmCore* core, iJIT_Method_Load* ML)
	{
		core->console << "method_id = " << ML->method_id << "\n";
		core->console << "method_name = '" << ML->method_name << "'\n";
		core->console << "method_load_address = " << (int) ML->method_load_address << "\n";
		core->console << "method_size = " << ML->method_size << "\n";
		core->console << "line_number_size = " << ML->line_number_size << "\n";
		core->console << "line_number_table = " << (int) ML->line_number_table << "\n";
            
		if (ML->line_number_table) 
		{
			core->console << "\tline\toffset\n";
			LineNumberInfo* lines = ML->line_number_table; 
			for (int j = 0; j < (int) ML->line_number_size; j++) 
			{
				core->console 
					<< "\t" 
					<< lines[j].LineNumber 
					<< "\t" 
					<< lines[j].Offset 
					<< "\n";
			}
		}
		
		core->console << "class_id = " << ML->class_id << "\n";
		core->console << "class_file_name = '" << ML->class_file_name << "'\n";
		core->console << "source_file_name = '" << ML->source_file_name << "'\n";
		core->console << "user_data = " << (int) ML->user_data << "\n";
		core->console << "user_data_size = " << ML->user_data_size << "\n";
		core->console << "env = " << ML->env << "\n\n";
	}
	
	void VTune_RegisterMethod(MethodInfo* info, CodegenMIR *mir, AvmCore* core) 
	{
		// vtune method info record
		iJIT_Method_NIDS* vtune = mir->getVtuneInfo();
		
		// assume no method inlining so start/end of mir gen = method start/end
		uintptr startAt = mir->getMdStart();
		uintptr endAt = mir->getMdEnd();
		uint32 methodSize = endAt - startAt;
		
		// get the name of the method
		UTF8String* utf = (info) ? info->name->toUTF8String() : core->kEmptyString->toUTF8String();

		// register the method
		iJIT_Method_Load ML;
		memset(&ML, 0, sizeof(iJIT_Method_Load));
	
		ML.method_id = vtune->method_id;
		ML.method_name = (char *)(malloc((utf->length()+3)*sizeof(char)));  // +1 for \0 and +2 for wtoc's ()
		wtoc(ML.method_name, utf->c_str(), 1);	// method name (can be with or without the class and signature, in any case the class name will be added to it)

		ML.method_load_address = (void *)startAt;	// virtual address of that method  - This determines the method range for the iJVM_EVENT_TYPE_ENTER/LEAVE_METHOD_ADDR events
		ML.method_size = methodSize;	// Size in memory - Must be exact

		// md position / line number table
		SortedIntMap<LineNumberRecord*>* tbl = mir->getLineNumberMap();
		int size = tbl->size();
		LineNumberInfo* lines = 0;
		if (size)
		{
			int bsz = size*sizeof(LineNumberInfo);
			lines = (LineNumberInfo*) malloc(bsz);
			memset(lines, 0, bsz);
		}

		ML.line_number_table = lines;	// Pointer to the begining of the line numbers info array
		ML.line_number_size = size;		// Line Table size in number of entries - Zero if none

		String* fileName = 0; 
		for(int i=0; i<size; i++)
		{
			sintptr mdPos = tbl->keyAt(i);
			LineNumberRecord* entry = tbl->at(i);
			
 			if (!fileName) fileName = entry->filename;

			// @todo file name should be part of the lines[] record, no?
			lines[i].LineNumber = entry->lineno;
			lines[i].Offset = mdPos - startAt;
		}
		
		ML.class_id = 0;                // uniq class ID
		ML.class_file_name = NULL;      // class file name 

		utf = ( fileName ) ? fileName->toUTF8String() : core->kEmptyString->toUTF8String();
		ML.source_file_name = (char *)(malloc((utf->length()+3)*sizeof(char)));   // +1 for \0 and +2 for wtoc's ()
		wtoc(ML.source_file_name, utf->c_str(), 0);	// source file name 

		ML.user_data = NULL;            // bits supplied by the user for saving in the JIT file...
		ML.user_data_size = 0;          // the size of the user data buffer
		ML.env = iJDE_JittingAPI;

		// DumpVTuneMethodInfo(core, &ML); // Uncommented to debug VTune
		iJIT_NotifyEvent(iJVM_EVENT_TYPE_METHOD_LOAD_FINISHED, &ML);

		if (core->VTuneStatus == iJIT_CALLGRAPH_ON) 
		{
			MMgc::GCHeap* heap = core->GetGC()->GetGCHeap();
			heap->SetExecuteBit(vtune, sizeof (iJIT_Method_NIDS), false);   
		}
 
		// free everything we alloc'd  ( @todo did vtune really copy all the strings?!? )
		if (ML.line_number_table) free(ML.line_number_table);
		if (ML.method_name) free(ML.method_name);
		if (ML.source_file_name) free(ML.source_file_name);
	}		
}

#endif /* VTUNE */
