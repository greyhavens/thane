// Generated from ST_mmgc_basics.st
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

#include "avmshell.h"
#ifdef AVMPLUS_SELFTEST
namespace avmplus {
class ST_mmgc_basics : public Selftest {
public:
ST_mmgc_basics(AvmCore* core);
virtual void run(int n);
virtual void prologue();
private:
static const char* ST_names[];
void test0();
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();
void test9();
void test10();
private:
    MMgc::GC *gc;
    MMgc::FixedAlloc *fa;
    MMgc::FixedMalloc *fm;

};
ST_mmgc_basics::ST_mmgc_basics(AvmCore* core)
    : Selftest(core, "mmgc", "basics", ST_mmgc_basics::ST_names)
{}
const char* ST_mmgc_basics::ST_names[] = {"create_gc_instance","create_gc_object","is_gc_memory","get_bytesinuse","collect","getgcheap","fixedAlloc","fixedMalloc","gcheap","gcmethods","gcLargeAlloc", NULL };
void ST_mmgc_basics::run(int n) {
switch(n) {
case 0: test0(); return;
case 1: test1(); return;
case 2: test2(); return;
case 3: test3(); return;
case 4: test4(); return;
case 5: test5(); return;
case 6: test6(); return;
case 7: test7(); return;
case 8: test8(); return;
case 9: test9(); return;
case 10: test10(); return;
}
}
void ST_mmgc_basics::prologue() {
	gc=new MMgc::GC(MMgc::GCHeap::GetGCHeap());
	if (gc==NULL) {
	    MMgc::GCHeap::Init();
	    MMgc::FixedMalloc::Init();
	    gc=new MMgc::GC(MMgc::GCHeap::GetGCHeap());
	}

}
void ST_mmgc_basics::test0() {
verifyPass(gc != NULL, "gc != NULL", __FILE__, __LINE__);

}
void ST_mmgc_basics::test1() {
    MyGCObject *mygcobject;
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
verifyPass(mygcobject!=NULL, "mygcobject!=NULL", __FILE__, __LINE__);
    mygcobject->i=10;
verifyPass(mygcobject->i==10, "mygcobject->i==10", __FILE__, __LINE__);

}
void ST_mmgc_basics::test2() {
    MyGCObject *mygcobject;
    MyObject *myobject;
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
    myobject = (MyObject *)new(MyObject);
verifyPass(gc->IsGCMemory(mygcobject) == true, "gc->IsGCMemory(mygcobject) == true", __FILE__, __LINE__);
verifyPass(gc->IsGCMemory(myobject) == false, "gc->IsGCMemory(myobject) == false", __FILE__, __LINE__);
    delete myobject;
    delete mygcobject;

}
void ST_mmgc_basics::test3() {
    MyGCObject *mygcobject;
    int inuse=(int)gc->GetBytesInUse();
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
//    AvmLog("bytes in use before %d after %d\n",inuse,(int)gc->GetBytesInUse());
verifyPass((int)gc->GetBytesInUse()==inuse+8, "(int)gc->GetBytesInUse()==inuse+8", __FILE__, __LINE__);
    delete mygcobject;

}
void ST_mmgc_basics::test4() {
    MyGCObject *mygcobject;
    int inuse=(int)gc->GetBytesInUse();
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
verifyPass((int)gc->GetBytesInUse()>inuse, "(int)gc->GetBytesInUse()>inuse", __FILE__, __LINE__);
    delete mygcobject;
//    AvmLog("collect: inuse=%d current=%d\n",inuse,(int)gc->GetBytesInUse());
    gc->Collect();      
//    AvmLog("collect: inuse=%d current=%d\n",inuse,(int)gc->GetBytesInUse());
verifyPass((int)gc->GetBytesInUse()<=inuse, "(int)gc->GetBytesInUse()<=inuse", __FILE__, __LINE__);

}
void ST_mmgc_basics::test5() {
verifyPass(gc->GetGCHeap()!=NULL, "gc->GetGCHeap()!=NULL", __FILE__, __LINE__);

}
void ST_mmgc_basics::test6() {
    MMgc::FixedAlloc *fa;
    fa=new MMgc::FixedAlloc(2048,MMgc::GCHeap::GetGCHeap());                                        
verifyPass((int)fa->GetMaxAlloc()==0, "(int)fa->GetMaxAlloc()==0", __FILE__, __LINE__);
verifyPass((int)fa->GetNumChunks()==0, "(int)fa->GetNumChunks()==0", __FILE__, __LINE__);
    void *data1=fa->Alloc(4096);
verifyPass(MMgc::FixedAlloc::GetFixedAlloc(data1)==fa, "MMgc::FixedAlloc::GetFixedAlloc(data1)==fa", __FILE__, __LINE__);
verifyPass((int)fa->GetBytesInUse()==2048, "(int)fa->GetBytesInUse()==2048", __FILE__, __LINE__);
verifyPass((int)fa->GetItemSize()==2048, "(int)fa->GetItemSize()==2048", __FILE__, __LINE__);
    void *data2=fa->Alloc(4096);
verifyPass(MMgc::FixedAlloc::GetFixedAlloc(data2)==fa, "MMgc::FixedAlloc::GetFixedAlloc(data2)==fa", __FILE__, __LINE__);
//    AvmLog("fa->GetItemSize=%d\n",(int)fa->GetItemSize());
verifyPass((int)fa->GetItemSize()==2048, "(int)fa->GetItemSize()==2048", __FILE__, __LINE__);
    fa->Free(data1);
verifyPass((int)fa->GetItemSize()==2048, "(int)fa->GetItemSize()==2048", __FILE__, __LINE__);
verifyPass((int)fa->GetMaxAlloc()==2, "(int)fa->GetMaxAlloc()==2", __FILE__, __LINE__);
verifyPass((int)fa->GetNumChunks()==2, "(int)fa->GetNumChunks()==2", __FILE__, __LINE__);
    gc->Collect();

}
void ST_mmgc_basics::test7() {
    fm=MMgc::FixedMalloc::GetInstance();
    int start=(int)fm->GetBytesInUse();
    int starttotal=(int)fm->GetTotalSize();
//    AvmLog("fm->GetBytesInUse()=%d\n",(int)fm->GetBytesInUse());
verifyPass((int)fm->GetBytesInUse()==start, "(int)fm->GetBytesInUse()==start", __FILE__, __LINE__);
//    AvmLog("fm->GetTotalSize()=%d\n",(int)fm->GetTotalSize());
verifyPass((int)fm->GetTotalSize()==starttotal, "(int)fm->GetTotalSize()==starttotal", __FILE__, __LINE__);
    void *obj=fm->Alloc(8192);
//    AvmLog("fm->GetBytesInUse()=%d\n",(int)fm->GetBytesInUse());
verifyPass((int)fm->GetBytesInUse()==start+8192, "(int)fm->GetBytesInUse()==start+8192", __FILE__, __LINE__);
//    AvmLog("fm->GetTotalSize()=%d\n",(int)fm->GetTotalSize());
verifyPass((int)fm->GetTotalSize()==starttotal+2, "(int)fm->GetTotalSize()==starttotal+2", __FILE__, __LINE__);
    fm->Free(obj);
//    AvmLog("fm->GetBytesInUse()=%d\n",(int)fm->GetBytesInUse());
verifyPass((int)fm->GetBytesInUse()==start, "(int)fm->GetBytesInUse()==start", __FILE__, __LINE__);
//    AvmLog("fm->GetTotalSize()=%d\n",(int)fm->GetTotalSize());
verifyPass((int)fm->GetTotalSize()==starttotal, "(int)fm->GetTotalSize()==starttotal", __FILE__, __LINE__);
    obj=fm->Calloc(1024,10);
//    AvmLog("fm->GetBytesInUse()=%d\n",(int)fm->GetBytesInUse());
verifyPass((int)fm->GetBytesInUse()==start+1024*12, "(int)fm->GetBytesInUse()==start+1024*12", __FILE__, __LINE__);
//    AvmLog("fm->GetTotalSize()=%d\n",(int)fm->GetTotalSize());
verifyPass((int)fm->GetTotalSize()==starttotal+3, "(int)fm->GetTotalSize()==starttotal+3", __FILE__, __LINE__);
    fm->Free(obj);
verifyPass((int)fm->GetBytesInUse()==start, "(int)fm->GetBytesInUse()==start", __FILE__, __LINE__);
verifyPass((int)fm->GetTotalSize()==starttotal, "(int)fm->GetTotalSize()==starttotal", __FILE__, __LINE__);
    gc->Collect();

}
void ST_mmgc_basics::test8() {
    MMgc::GCHeap *gh=MMgc::GCHeap::GetGCHeap();
    int startfreeheap=(int)gh->GetFreeHeapSize();
verifyPass((int)gh->GetTotalHeapSize()==128, "(int)gh->GetTotalHeapSize()==128", __FILE__, __LINE__);
//    AvmLog("gh->GetFreeHeapSize()=%d\n",(int)gh->GetFreeHeapSize());
verifyPass((int)gh->GetFreeHeapSize()==startfreeheap, "(int)gh->GetFreeHeapSize()==startfreeheap", __FILE__, __LINE__);
    gh->SetHeapLimit(1024);
verifyPass((int)gh->GetTotalHeapSize()==128, "(int)gh->GetTotalHeapSize()==128", __FILE__, __LINE__);
//    AvmLog("gh->GetFreeHeapSize()=%d\n",(int)gh->GetFreeHeapSize());
verifyPass((int)gh->GetFreeHeapSize()==startfreeheap, "(int)gh->GetFreeHeapSize()==startfreeheap", __FILE__, __LINE__);
    gh->Alloc(1024*10,true,true);
verifyPass((int)gh->GetTotalHeapSize()==10368, "(int)gh->GetTotalHeapSize()==10368", __FILE__, __LINE__);
//    AvmLog("gh->GetFreeHeapSize()=%d\n",(int)gh->GetFreeHeapSize());
verifyPass((int)gh->GetFreeHeapSize()==startfreeheap, "(int)gh->GetFreeHeapSize()==startfreeheap", __FILE__, __LINE__);

}
void ST_mmgc_basics::test9() {

    MyGCObject *mygcobject;
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
verifyPass((MyGCObject *)gc->FindBeginning(mygcobject)==mygcobject, "(MyGCObject *)gc->FindBeginning(mygcobject)==mygcobject", __FILE__, __LINE__);

}
void ST_mmgc_basics::test10() {
    MyGCObject *mygcobject;
    mygcobject = (MyGCObject *)new (gc) MyGCObject();
    MMgc::GCLargeAlloc *gcl=new MMgc::GCLargeAlloc(gc);
    void *obj=gcl->Alloc(1024,0);
verifyPass(MMgc::GCLargeAlloc::IsLargeBlock(obj)==true, "MMgc::GCLargeAlloc::IsLargeBlock(obj)==true", __FILE__, __LINE__);
verifyPass(MMgc::GCLargeAlloc::FindBeginning(obj)==obj, "MMgc::GCLargeAlloc::FindBeginning(obj)==obj", __FILE__, __LINE__);
verifyPass(MMgc::GCLargeAlloc::IsFinalized(obj)==false, "MMgc::GCLargeAlloc::IsFinalized(obj)==false", __FILE__, __LINE__);
    MMgc::GCLargeAlloc::SetFinalize(obj);
verifyPass(MMgc::GCLargeAlloc::IsFinalized(obj)==true, "MMgc::GCLargeAlloc::IsFinalized(obj)==true", __FILE__, __LINE__);
    MMgc::GCLargeAlloc::ClearFinalized(obj);
    gcl->Free(obj);
verifyPass(MMgc::GCLargeAlloc::IsFinalized(obj)==false, "MMgc::GCLargeAlloc::IsFinalized(obj)==false", __FILE__, __LINE__);
verifyPass(MMgc::GCLargeAlloc::ContainsPointers(obj)==false, "MMgc::GCLargeAlloc::ContainsPointers(obj)==false", __FILE__, __LINE__);
verifyPass(MMgc::GCLargeAlloc::HasWeakRef(obj)==false, "MMgc::GCLargeAlloc::HasWeakRef(obj)==false", __FILE__, __LINE__);
    MMgc::GCLargeAlloc::SetHasWeakRef(obj,true);
verifyPass(MMgc::GCLargeAlloc::HasWeakRef(obj)==true, "MMgc::GCLargeAlloc::HasWeakRef(obj)==true", __FILE__, __LINE__);
    MMgc::GCLargeAlloc::SetHasWeakRef(obj,false);
verifyPass(MMgc::GCLargeAlloc::HasWeakRef(obj)==false, "MMgc::GCLargeAlloc::HasWeakRef(obj)==false", __FILE__, __LINE__);
verifyPass(MMgc::GCLargeAlloc::GetMark(obj)==false, "MMgc::GCLargeAlloc::GetMark(obj)==false", __FILE__, __LINE__);
    MMgc::GCLargeAlloc::SetMark(obj);
verifyPass(MMgc::GCLargeAlloc::GetMark(obj)==true, "MMgc::GCLargeAlloc::GetMark(obj)==true", __FILE__, __LINE__);
    MMgc::GCLargeAlloc::SetQueued(obj);
verifyPass(MMgc::GCLargeAlloc::IsRCObject(obj)==false, "MMgc::GCLargeAlloc::IsRCObject(obj)==false", __FILE__, __LINE__);
verifyPass(MMgc::GCLargeAlloc::IsWhite(obj)==false, "MMgc::GCLargeAlloc::IsWhite(obj)==false", __FILE__, __LINE__);
verifyPass(MMgc::GCLargeAlloc::IsWhite(mygcobject)==false, "MMgc::GCLargeAlloc::IsWhite(mygcobject)==false", __FILE__, __LINE__);



}
void create_mmgc_basics(AvmCore* core) { new ST_mmgc_basics(core); }
}
#endif
