// -*- mode: c -*-
//
// ***** BEGIN LICENSE BLOCK *****
// Version: MPL 1.1/GPL 2.0/LGPL 2.1
//
// The contents of this file are subject to the Mozilla Public License Version
// 1.1 (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at
// http://www.mozilla.org/MPL/
//
// Software distributed under the License is distributed on an "AS IS" basis,
// WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
// for the specific language governing rights and limitations under the
// License.
//
// The Original Code is [Open Source Virtual Machine.].
//
// The Initial Developer of the Original Code is
// Adobe System Incorporated.
// Portions created by the Initial Developer are Copyright (C) 2004-2006
// the Initial Developer. All Rights Reserved.
//
// Contributor(s):
//   Adobe AS3 Team
//
// Alternatively, the contents of this file may be used under the terms of
// either the GNU General Public License Version 2 or later (the "GPL"), or
// the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
// in which case the provisions of the GPL or the LGPL are applicable instead
// of those above. If you wish to allow use of your version of this file only
// under the terms of either the GPL or the LGPL, and not to allow others to
// use your version of this file under the terms of the MPL, indicate your
// decision by deleting the provisions above and replace them with the notice
// and other provisions required by the GPL or the LGPL. If you do not delete
// the provisions above, a recipient may use your version of this file under
// the terms of any one of the MPL, the GPL or the LGPL.
//
// ***** END LICENSE BLOCK ***** */

%%component mmgc
%%category basics

%%decls
private:
    MMgc::GC *gc;
    MMgc::FixedAlloc *fa;
    MMgc::FixedMalloc *fm;

%%prologue
	gc=new MMgc::GC(MMgc::GCHeap::GetGCHeap());
	if (gc==NULL) {
	    MMgc::GCHeap::Init();
	    MMgc::FixedMalloc::Init();
	    gc=new MMgc::GC(MMgc::GCHeap::GetGCHeap());
	}

%%test create_gc_instance
    %%verify gc != NULL

%%test create_gc_object
    MyGCObject *mygcobject;
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
    %%verify mygcobject!=NULL
    mygcobject->i=10;
    %%verify mygcobject->i==10

%%test is_gc_memory
    MyGCObject *mygcobject;
    MyObject *myobject;
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
    myobject = (MyObject *)new(MyObject);
    %%verify gc->IsGCMemory(mygcobject) == true
    %%verify gc->IsGCMemory(myobject) == false
    delete myobject;
    delete mygcobject;

%%test get_bytesinuse
    MyGCObject *mygcobject;
    int inuse=(int)gc->GetBytesInUse();
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
//    printf("bytes in use before %d after %d\n",inuse,(int)gc->GetBytesInUse());
    %%verify (int)gc->GetBytesInUse()==inuse+8
    delete mygcobject;

%%test collect
    MyGCObject *mygcobject;
    int inuse=(int)gc->GetBytesInUse();
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
    %%verify (int)gc->GetBytesInUse()>inuse
    delete mygcobject;
//    printf("collect: inuse=%d current=%d\n",inuse,(int)gc->GetBytesInUse());
    gc->Collect();      
//    printf("collect: inuse=%d current=%d\n",inuse,(int)gc->GetBytesInUse());
    %%verify (int)gc->GetBytesInUse()<=inuse

%%test getgcheap
    %%verify gc->GetGCHeap()!=NULL

%%test fixedAlloc
    MMgc::FixedAlloc *fa;
    fa=new MMgc::FixedAlloc(2048,MMgc::GCHeap::GetGCHeap());                                        
    %%verify (int)fa->GetMaxAlloc()==0
    %%verify (int)fa->GetNumChunks()==0
    void *data1=fa->Alloc(4096);
    %%verify MMgc::FixedAlloc::GetFixedAlloc(data1)==fa
    %%verify (int)fa->GetBytesInUse()==2048
    %%verify (int)fa->GetItemSize()==2048
    void *data2=fa->Alloc(4096);
    %%verify MMgc::FixedAlloc::GetFixedAlloc(data2)==fa
//    printf("fa->GetItemSize=%d\n",(int)fa->GetItemSize());
    %%verify (int)fa->GetItemSize()==2048
    fa->Free(data1);
    %%verify (int)fa->GetItemSize()==2048
    %%verify (int)fa->GetMaxAlloc()==2
    %%verify (int)fa->GetNumChunks()==2
    gc->Collect();

%%test fixedMalloc
    fm=MMgc::FixedMalloc::GetInstance();
    int start=(int)fm->GetBytesInUse();
    int starttotal=(int)fm->GetTotalSize();
//    printf("fm->GetBytesInUse()=%d\n",(int)fm->GetBytesInUse());
    %%verify (int)fm->GetBytesInUse()==start
//    printf("fm->GetTotalSize()=%d\n",(int)fm->GetTotalSize());
    %%verify (int)fm->GetTotalSize()==starttotal
    void *obj=fm->Alloc(8192);
//    printf("fm->GetBytesInUse()=%d\n",(int)fm->GetBytesInUse());
    %%verify (int)fm->GetBytesInUse()==start+8192
//    printf("fm->GetTotalSize()=%d\n",(int)fm->GetTotalSize());
    %%verify (int)fm->GetTotalSize()==starttotal+2
    fm->Free(obj);
//    printf("fm->GetBytesInUse()=%d\n",(int)fm->GetBytesInUse());
    %%verify (int)fm->GetBytesInUse()==start
//    printf("fm->GetTotalSize()=%d\n",(int)fm->GetTotalSize());
    %%verify (int)fm->GetTotalSize()==starttotal
    obj=fm->Calloc(1024,10);
//    printf("fm->GetBytesInUse()=%d\n",(int)fm->GetBytesInUse());
    %%verify (int)fm->GetBytesInUse()==start+1024*12
//    printf("fm->GetTotalSize()=%d\n",(int)fm->GetTotalSize());
    %%verify (int)fm->GetTotalSize()==starttotal+3
    fm->Free(obj);
    %%verify (int)fm->GetBytesInUse()==start
    %%verify (int)fm->GetTotalSize()==starttotal
    gc->Collect();

%%test gcheap
    MMgc::GCHeap *gh=MMgc::GCHeap::GetGCHeap();
    int startfreeheap=(int)gh->GetFreeHeapSize();
    %%verify (int)gh->GetTotalHeapSize()==128
    %%verify (int)gh->GetCodeMemorySize()==0
//    printf("gh->GetFreeHeapSize()=%d\n",(int)gh->GetFreeHeapSize());
    %%verify (int)gh->GetFreeHeapSize()==startfreeheap
    gh->SetHeapLimit(1024);
    %%verify (int)gh->GetTotalHeapSize()==128
//    printf("gh->GetFreeHeapSize()=%d\n",(int)gh->GetFreeHeapSize());
    %%verify (int)gh->GetFreeHeapSize()==startfreeheap
    gh->Alloc(1024*10,true,true);
    %%verify (int)gh->GetTotalHeapSize()==10368
//    printf("gh->GetFreeHeapSize()=%d\n",(int)gh->GetFreeHeapSize());
    %%verify (int)gh->GetFreeHeapSize()==startfreeheap

%%test gcmethods

    MyGCObject *mygcobject;
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
    %%verify (MyGCObject *)gc->FindBeginning(mygcobject)==mygcobject

%%test gcLargeAlloc
    MyGCObject *mygcobject;
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
    MMgc::GCLargeAlloc *gcl=new MMgc::GCLargeAlloc(gc);
    void *obj=gcl->Alloc(1024,0);
    %%verify MMgc::GCLargeAlloc::IsLargeBlock(obj)==true
    %%verify MMgc::GCLargeAlloc::FindBeginning(obj)==obj
    %%verify MMgc::GCLargeAlloc::IsFinalized(obj)==false
    MMgc::GCLargeAlloc::SetFinalize(obj);
    %%verify MMgc::GCLargeAlloc::IsFinalized(obj)==true
    MMgc::GCLargeAlloc::ClearFinalized(obj);
    gcl->Free(obj);
    %%verify MMgc::GCLargeAlloc::IsFinalized(obj)==false
    %%verify MMgc::GCLargeAlloc::ContainsPointers(obj)==false
    %%verify MMgc::GCLargeAlloc::HasWeakRef(obj)==false
    MMgc::GCLargeAlloc::SetHasWeakRef(obj,true);
    %%verify MMgc::GCLargeAlloc::HasWeakRef(obj)==true
    MMgc::GCLargeAlloc::SetHasWeakRef(obj,false);
    %%verify MMgc::GCLargeAlloc::HasWeakRef(obj)==false
    %%verify MMgc::GCLargeAlloc::GetMark(obj)==false
    MMgc::GCLargeAlloc::SetMark(obj);
    %%verify MMgc::GCLargeAlloc::GetMark(obj)==true
    MMgc::GCLargeAlloc::SetQueued(obj);
    %%verify MMgc::GCLargeAlloc::IsRCObject(obj)==false
    %%verify MMgc::GCLargeAlloc::IsWhite(obj)==false
    %%verify MMgc::GCLargeAlloc::IsWhite(mygcobject)==false


