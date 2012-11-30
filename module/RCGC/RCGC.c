/****************************************************************************
 * Copyright (c) 2012, the Konoha project authors. All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

/* ************************************************************************ */

#include <stdio.h>

//#define GCDEBUG 1

#include "minikonoha/minikonoha.h"
#include "minikonoha/local.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(GCDEBUG) && !defined(GCSTAT)
#define GCSTAT 1
#endif

/* memory config */

#ifndef unlikely
#define unlikely(x)   __builtin_expect(!!(x), 0)
#endif

#ifndef likely
#define likely(x)     __builtin_expect(!!(x), 1)
#endif

#ifdef __GNUC__
#define prefetch_(addr, rw, locality) __builtin_prefetch(addr, rw, locality)
#else
#define prefetch_(addr, rw, locality)
#endif

/* ------------------------------------------------------------------------ */
#define ShiftPointer(p, size)   ((char*)p + (size))
#define MemorySize(p, p2)      (((char*)p) - ((char*)p2))

#define PageObjectSize(i) (K_PAGESIZE / sizeof(kGCObject##i))

#define ArenaTable_InitSize     32

#define Object_unsetMark(o) TFLAG_set0(uintptr_t,(o)->h.magicflag,kObject_GCFlag)
#define Object_setMark(o)   TFLAG_set1(uintptr_t,(o)->h.magicflag,kObject_GCFlag)
#define Object_isMark(o)   (TFLAG_is(uintptr_t,(o)->h.magicflag, kObject_GCFlag))
static inline void *do_malloc(size_t size);
static inline void *do_calloc(size_t count, size_t size);
static inline void *do_realloc(void *ptr, size_t oldSize, size_t newSize);
static inline void  do_Free(void *ptr, size_t size);
static inline void  do_bzero(void *ptr, size_t size);

/* [RCGC Deta Structure] */

typedef struct kGCObject0 {
	KonohaObjectHeader h;
	struct kGCObject0 *ref;
	void *ref2_unused;
	void *ref3_unused;
	//void *ref4_unused;
	struct kGCObject0 *ref5_tail;
} kGCObject0;

typedef struct kGCObject1 {
	KonohaObjectHeader h;
	struct kGCObject1 *ref;
	void *ref2_unused;
	void *ref3_unused;
	//void *ref4_unused;
	struct kGCObject1 *ref5_tail;
	uint8_t unused[sizeof(kGCObject0)*2-sizeof(kGCObject0)];
} kGCObject1;

typedef struct kGCObject2 {
	KonohaObjectHeader h;
	struct kGCObject2 *ref;
	void *ref2_unused;
	void *ref3_unused;
	//void *ref4_unused;
	struct kGCObject2 *ref5_tail;
	uint8_t unused[sizeof(kGCObject0)*4-sizeof(kGCObject0)];
} kGCObject2;

typedef union kGCObject {
	kGCObject0 o0;
	kGCObject1 o1;
	kGCObject2 o2;
} kGCObject;

typedef struct ObjectPage0_t {
	kGCObject0  slots[PageObjectSize(0)];
} ObjectPage0_t;

typedef struct ObjectPage1_t {
	kGCObject1  slots[PageObjectSize(1)];
} ObjectPage1_t;

typedef struct ObjectPage2_t {
	kGCObject2  slots[PageObjectSize(2)];
} ObjectPage2_t;

typedef struct ObjectPageTable_t {
	union {
		ObjectPage0_t      *head0;
		ObjectPage1_t      *head1;
		ObjectPage2_t      *head2;
	};
	union {
		ObjectPage0_t      *bottom0;
		ObjectPage1_t      *bottom1;
		ObjectPage2_t      *bottom2;
	};
	size_t          arena_size;
} ObjectPageTable_t;

#define ARENA_COUNT 3

typedef struct MarkStack {
	kObject **stack;
	size_t tail;
	size_t capacity;
	size_t capacity_log2;
} MarkStack;

struct GcContext {
	uintptr_t flags;

	ObjectPageTable_t  *ArenaTable[ARENA_COUNT];
	size_t              ArenaTable_size[ARENA_COUNT];
	size_t              ArenaTable_capacity[ARENA_COUNT];

	kGCObject *FreeList[ARENA_COUNT];
	size_t     FreeList_size[ARENA_COUNT];

	KonohaContext *kctx;
	MarkStack mstack;
};

typedef struct gc_stat {
	size_t total_object;
	//size_t object_count[];
	size_t gc_count;
	//size_t marked[];
	//size_t collected[];
	size_t markingTime;
	size_t sweepingTime;
	size_t current_request_size;
	//AllocationBlock *managed_heap;
	//AllocationBlock *managed_heap_end;
	FILE *fp;
} rcgc_stat;

#ifdef GCSTAT
static rcgc_stat global_gc_stat = {};
#endif

/* ------------------------------------------------------------------------ */
/* [MSGC API declaration] */
/* ------------------------------------------------------------------------ */
static void mark_mstack(GcContext *kctx, kObject *ref, MarkStack *mstack);

static GcContext *Arena_Init(KonohaContext *kctx)
{
	GcContext *gcContext = (GcContext *)do_malloc(sizeof(GcContext));
	size_t i;
	for(i = 0; i < ARENA_COUNT; ++i) {
		gcContext->ArenaTable[i] = (ObjectPageTable_t *)do_malloc(ArenaTable_InitSize * sizeof(ObjectPageTable_t));
		gcContext->ArenaTable_size[i] = 0;
		gcContext->ArenaTable_capacity[i] = ArenaTable_InitSize;
	}
	return gcContext;
}

#define ARENA_FREE(j) do {\
	size_t i;\
	DBG_ASSERT(gcContext->ArenaTable[j] != NULL);\
	for(i = 0; i < gcContext->ArenaTable_size[j]; i++) {\
		ObjectPageTable_t *oat = gcContext->ArenaTable[j] + i;\
		DBG_ASSERT(MemorySize(oat->bottom##j, oat->head##j) == oat->arena_size);\
		do_Free(oat->head##j, oat->arena_size);\
	}\
	do_Free(gcContext->ArenaTable[j], gcContext->ArenaTable_capacity[j] * sizeof(ObjectPageTable_t));\
} while (0)

static void Arena_Free(GcContext *gcContext)
{
	ARENA_FREE(0);
	ARENA_FREE(1);
	ARENA_FREE(2);
}

/* ------------------------------------------------------------------------ */
/* malloc */

static void THROW_OutOfMemory(KonohaContext *kctx, size_t size)
{
	KExit(EXIT_FAILURE);
}

static inline void do_bzero(void *ptr, size_t size)
{
	memset(ptr, 0, size);
}

static inline void do_memcpy(void *s1, void *s2, size_t size)
{
	memcpy(s1, s2, size);
}

static inline void *do_malloc(size_t size)
{
	void *ptr = malloc(size);
	do_bzero(ptr, size);
	//printf("malloc:%zd\n",size);
	//DBG_CHECK_MALLOCED_INC_SIZE(size);
	return ptr;
}

static inline void *do_calloc(size_t count, size_t size)
{
	void *ptr = calloc(count, size);
	//printf("calloc:%zd\n",size);
	//DBG_CHECK_MALLOCED_INC_SIZE(size);
	return ptr;
}

static inline void *do_realloc(void *ptr, size_t oldSize, size_t newSize)
{
	char *newptr = (char *) realloc(ptr, newSize);
	do_bzero(newptr+oldSize, newSize-oldSize);
	printf("realloc:%zd->%zd\n",oldSize,newSize);
	//DBG_CHECK_MALLOCED_INC_SIZE(newSize - oldSize);
	return (void *) newptr;
}

static inline void do_Free(void *ptr, size_t size)
{
#if GCDEBUG
	memset(ptr, 0xa, size);
#endif
	printf("free:%zd\n",size);
	//DBG_CHECK_MALLOCED_DEC_SIZE(size);
	free(ptr);
}

static ssize_t klib_malloced = 0;

static void *Kmalloc(KonohaContext *kctx, size_t s, KTraceInfo *trace)
{
	size_t *p = (size_t *)do_malloc(s
#ifdef MEMORY_DEBUG
			+ sizeof(size_t)
#endif
			);
	if(unlikely(p == NULL)) {
		KTraceApi(trace, SystemFault|UserFault, "malloc",
			LogUint("size", s), LogUint("UsedMemorySize", klib_malloced));
		THROW_OutOfMemory(kctx, s);
	}
#if GCDEBUG
	OLDTRACE_SWITCH_TO_KTrace(LOGPOL_DEBUG,
			LogText("@", "malloc"),
			KeyValue_p("from", p),
			KeyValue_p("to", ((char *)p)+s),
			LogUint("size", s));
#endif
	klib_malloced += s;
#ifdef MEMORY_DEBUG
	p[0] = s;
	p += 1;
#endif
	return (void *)p;
}

static void *Kzmalloc(KonohaContext *kctx, size_t s, KTraceInfo *trace)
{
	size_t *p = (size_t *)do_calloc(1, s
#ifdef MEMORY_DEBUG
			+ sizeof(size_t)
#endif
			);
	if(unlikely(p == NULL)) {
		THROW_OutOfMemory(kctx, s);
	}
	klib_malloced += s;
#ifdef MEMORY_DEBUG
	p[0] = s;
	p += 1;
#endif
	return (void *)(p);
}

static void Kfree(KonohaContext *kctx, void *p, size_t s)
{
	size_t *pp = (size_t *)p;
	klib_malloced -= s;
#ifdef MEMORY_DEBUG
	DBG_ASSERT(pp[-1] == s);
	pp -= 1;
#endif
#if GCDEBUG
	OLDTRACE_SWITCH_TO_KTrace(LOGPOL_DEBUG,
			LogText("@", "free"),
			KeyValue_p("from", p),
			KeyValue_p("to", ((char *)p)+s),
			LogUint("size", s));
#endif
	do_Free(pp, s
#ifdef MEMORY_DEBUG
			+ sizeof(size_t)
#endif
			);
}

/* ------------------------------------------------------- */
#define FREELIST_POP(o,i) do {\
	if(gcContext->FreeList[i] == NULL){\
		printf("Heap extend%d\n",(i));\
		extendHeap[i](gcContext);\
	}\
	o = gcContext->FreeList[i];\
	gcContext->FreeList[i] = (kGCObject *)((kGCObject0 *)o)->ref;\
	((kGCObject0 *)o)->ref = NULL;\
} while (0)

#define FREELIST_PUSH(o,i) do {\
	kGCObject0 *tmp0 = (kGCObject0 *) o;\
	tmp0->ref = (kGCObject0 *) gcContext->FreeList[i];\
	gcContext->FreeList[i] = (kGCObject *)o;\
	gcContext->FreeList_size[i] += 1;\
} while (0)

#define OBJECT_REUSE(used,i) do {\
	((kObjectVar*)(used))->h.ct = NULL;\
	((kObjectVar*)(used))->h.magicflag = 0;\
	((kObjectVar*)(used))->h.refc = 0;\
	printf("GC:%lx\n",((uintptr_t)(used)));\
	FREELIST_PUSH(used,i);\
} while (0)

#define ObjectPage_Init(j) do {\
	size_t i = 0;\
	kGCObject##j *o = opage->slots;\
	size_t t = PageObjectSize(j) - 1;\
	for(i = 0; i < t; ++i) {\
		o[i].h.ct = NULL;\
		o[i].h.refc = 0;\
		o[i].ref = &(o[i+1]);\
	}\
	opage->slots[t].h.ct = NULL;\
	opage->slots[t].ref = opage[1].slots;\
} while(0)

static void ObjectPage_Init0(ObjectPage0_t *opage)
{
	ObjectPage_Init(0);
}

static void ObjectPage_Init1(ObjectPage1_t *opage)
{
	ObjectPage_Init(1);
}

static void ObjectPage_Init2(ObjectPage2_t *opage)
{
	ObjectPage_Init(2);
}

#define ObjectArenaTable_Init(j) do {\
	ObjectPage##j##_t *opage = (ObjectPage##j##_t *)do_malloc(arenasize);\
	oat->head##j   = opage;\
	oat->bottom##j = (ObjectPage##j##_t *)ShiftPointer(opage, arenasize);\
	oat->arena_size = arenasize;\
	for(; opage < oat->bottom##j; opage++) {\
		ObjectPage_Init##j(opage);\
		gcContext->FreeList_size[j] += PageObjectSize(j);\
	}\
	(opage-1)->slots[PageObjectSize(j) - 1].ref = NULL;\
} while(0)

static void ObjectArenaTable_Init0(GcContext *gcContext, ObjectPageTable_t *oat, size_t arenasize)
{
	ObjectArenaTable_Init(0);
}

static void ObjectArenaTable_Init1(GcContext *gcContext, ObjectPageTable_t *oat, size_t arenasize)
{
	ObjectArenaTable_Init(1);
}

static void ObjectArenaTable_Init2(GcContext *gcContext, ObjectPageTable_t *oat, size_t arenasize)
{
	ObjectArenaTable_Init(2);
}

static kGCObject0 *new_ObjectArena0(GcContext *gcContext, size_t arenasize)
{
	ObjectPageTable_t *oat;
	size_t pageindex = gcContext->ArenaTable_size[0];
	if(unlikely(!(pageindex < gcContext->ArenaTable_capacity[0]))) {
		size_t oldsize = gcContext->ArenaTable_capacity[0];
		size_t newsize = oldsize * 2;
		gcContext->ArenaTable[0] = (ObjectPageTable_t*)do_realloc(gcContext->ArenaTable[0], oldsize * sizeof(ObjectPageTable_t), newsize * sizeof(ObjectPageTable_t));
		gcContext->ArenaTable_capacity[0] = newsize;
	}
	gcContext->ArenaTable_size[0] += 1;
	oat = &gcContext->ArenaTable[0][pageindex];
	ObjectArenaTable_Init0(gcContext, oat, arenasize);
	kGCObject0 *p = oat->head0->slots;
	p->ref5_tail = (kGCObject0 *)&(oat->bottom0[-1]);

	int i = 0;
	kGCObject0 *tmp = p;
	while (tmp != &oat->head0->slots[PageObjectSize(0)]) {
		tmp = tmp->ref;
		i++;
	}
	assert(i == PageObjectSize(0));
	return p;
}

static kGCObject1 *new_ObjectArena1(GcContext *gcContext, size_t arenasize)
{
	ObjectPageTable_t *oat;
	size_t pageindex = gcContext->ArenaTable_size[1];
	if(unlikely(!(pageindex < gcContext->ArenaTable_capacity[1]))) {
		size_t oldsize = gcContext->ArenaTable_capacity[1];
		size_t newsize = oldsize * 2;
		gcContext->ArenaTable[1] = (ObjectPageTable_t*)do_realloc(gcContext->ArenaTable[1], oldsize * sizeof(ObjectPageTable_t), newsize * sizeof(ObjectPageTable_t));
		gcContext->ArenaTable_capacity[1] = newsize;
	}
	gcContext->ArenaTable_size[1] += 1;
	oat = &gcContext->ArenaTable[1][pageindex];
	ObjectArenaTable_Init1(gcContext, oat, arenasize);
	kGCObject1 *p = oat->head1->slots;
	p->ref5_tail = (kGCObject1 *)&(oat->bottom1[-1]);

	int i = 0;
	kGCObject1 *tmp = p;
	while (tmp != &oat->head1->slots[PageObjectSize(1)]) {
		tmp = tmp->ref;
		i++;
	}
	assert(i == PageObjectSize(1));
	return p;
}

static kGCObject2 *new_ObjectArena2(GcContext *gcContext, size_t arenasize)
{
	ObjectPageTable_t *oat;
	size_t pageindex = gcContext->ArenaTable_size[2];
	if(unlikely(!(pageindex < gcContext->ArenaTable_capacity[2]))) {
		size_t oldsize = gcContext->ArenaTable_capacity[2];
		size_t newsize = oldsize * 2;
		gcContext->ArenaTable[2] = (ObjectPageTable_t*)do_realloc(gcContext->ArenaTable[2], oldsize * sizeof(ObjectPageTable_t), newsize * sizeof(ObjectPageTable_t));
		gcContext->ArenaTable_capacity[2] = newsize;
	}
	gcContext->ArenaTable_size[2] += 1;
	oat = &gcContext->ArenaTable[2][pageindex];
	ObjectArenaTable_Init2(gcContext, oat, arenasize);
	kGCObject2 *p = oat->head2->slots;
	p->ref5_tail = (kGCObject2 *)&(oat->bottom2[-1]);

	int i = 0;
	kGCObject2 *tmp = p;
	while (tmp != &oat->head2->slots[PageObjectSize(2)]) {
		tmp = tmp->ref;
		i++;
	}
	assert(i == PageObjectSize(2));
	return p;
}

#define ObjectArenaTable_Free(j) do {\
	ObjectPage##j##_t *opage = oat->head##j;\
	while(opage < oat->bottom##j) {\
		size_t i;\
		for(i = 0; i < PageObjectSize(j); ++i) {\
			kGCObject##j *o = &opage->slots[i];\
			if(o->h.ct == NULL) continue;\
			KLIB kObjectProto_Free(kctx, (kObjectVar*)o);\
		}\
		opage++;\
	}\
} while (0)

static void ObjectArenaTable_Free0(KonohaContext *kctx, const ObjectPageTable_t *oat)
{
	ObjectArenaTable_Free(0);
}

static void ObjectArenaTable_Free1(KonohaContext *kctx, const ObjectPageTable_t *oat)
{
	ObjectArenaTable_Free(1);
}

static void ObjectArenaTable_Free2(KonohaContext *kctx, const ObjectPageTable_t *oat)
{
	ObjectArenaTable_Free(2);
}

#define ObjectArena_FinalFree(j) do {\
	size_t i;\
	DBG_ASSERT(oat != NULL);\
	for(i = 0; i < oatSize; i++) {\
		ObjectPageTable_t *t = oat + i;\
		ObjectArenaTable_Free##j(kctx, t);\
	}\
} while (0)

static void ObjectArena_FinalFree0(KonohaContext *kctx, ObjectPageTable_t *oat, size_t oatSize)
{
	ObjectArena_FinalFree(0);
}

static void ObjectArena_FinalFree1(KonohaContext *kctx, ObjectPageTable_t *oat, size_t oatSize)
{
	ObjectArena_FinalFree(1);
}

static void ObjectArena_FinalFree2(KonohaContext *kctx, ObjectPageTable_t *oat, size_t oatSize)
{
	ObjectArena_FinalFree(2);
}

#define K_ARENASIZE  ((sizeof(kGCObject0) * K_PAGESIZE) * 16) /*4MB*/

#define gc_extendObjectArena(N) do {\
	size_t i = 0;\
	size_t size = gcContext->ArenaTable_size[N];\
	for(;i<size;++i) {\
		kGCObject##N *block = new_ObjectArena##N(gcContext, K_ARENASIZE);\
		kGCObject##N *tail  = block->ref5_tail;\
		tail->ref = (kGCObject##N *)gcContext->FreeList[N];\
		gcContext->FreeList[N] = (kGCObject *)block;\
	}\
} while (0)

static void gc_extendObjectArena0(GcContext *gcContext)
{
	gc_extendObjectArena(0);
}

static void gc_extendObjectArena1(GcContext *gcContext)
{
	gc_extendObjectArena(1);
}

static void gc_extendObjectArena2(GcContext *gcContext)
{
	gc_extendObjectArena(2);
}
typedef void (*ExtendHeap)(GcContext *gcContext);
static ExtendHeap extendHeap[3] = {gc_extendObjectArena0, gc_extendObjectArena1, gc_extendObjectArena2};

/* ------------------------------------------------------------------------ */
/* [mstack] */

static MarkStack *mstack_Init(MarkStack *mstack)
{
	if(mstack->capacity == 0) {
		mstack->capacity_log2 = 12;
		mstack->capacity = (1 << mstack->capacity_log2) - 1;
		DBG_ASSERT(K_PAGESIZE == 1 << 12);
		mstack->stack = (kObject**)do_malloc(sizeof(kObject *)*(mstack->capacity + 1));
	}
	mstack->tail = 0;
	return mstack;
}

static void mstack_Push(MarkStack *mstack, kObject *ref)
{
	size_t ntail = (mstack->tail + 1) & mstack->capacity;
	if(unlikely(ntail == 0)) {
		size_t capacity = 1 << mstack->capacity_log2;
		size_t stacksize = sizeof(kObject *) * capacity;
		mstack->stack = (kObject**)do_realloc(mstack->stack, stacksize, stacksize * 2);
		mstack->capacity_log2 += 1;
		mstack->capacity = (1 << mstack->capacity_log2) - 1;
		ntail = (mstack->tail + 1) & mstack->capacity;
	}
	mstack->stack[mstack->tail] = ref;
	mstack->tail = ntail;
}

static kObject *mstack_next(MarkStack *mstack)
{
	kObject *ref = NULL;
	if(likely(mstack->tail != 0)) {
		mstack->tail -=1;
		ref = mstack->stack[mstack->tail];
		prefetch_(ref, 0, 0);
	}
	return ref;
}

/* --------------------------------------------------------------- */

#define RCGC_SETUP(i) do {\
	kGCObject##i *p = new_ObjectArena##i(gcContext, K_ARENASIZE);\
	gcContext->FreeList[i] = (kGCObject *)p;\
} while (0)

static void KnewGcContext(KonohaContext *kctx)
{
	((KonohaContextVar*)kctx)->gcContext = Arena_Init(kctx);
	GcContext *gcContext = (GcContext *)kctx->gcContext;
	gcContext->kctx = kctx;
	RCGC_SETUP(0);
	RCGC_SETUP(1);
	RCGC_SETUP(2);

#ifdef GCSTAT
	global_gc_stat.fp = fopen("KONOHA_RCGC_INFO", "a");
#endif
	((KonohaContextVar*)kctx)->gcContext = gcContext;
}

#define  FinalFree(n) ObjectArena_FinalFree##n(kctx, gcContext->ArenaTable[n], gcContext->ArenaTable_size[n]);
static void KdeleteGcContext(KonohaContext *kctx)
{
	GcContext *gcContext = (GcContext *)kctx->gcContext;
	FinalFree(0);
	FinalFree(1);
	FinalFree(2);
	Arena_Free(gcContext);
	if(gcContext->mstack.capacity > 0) {
		do_Free(gcContext->mstack.stack,  (gcContext->mstack.capacity + 1) * sizeof(kObject*));
		gcContext->mstack.capacity = 0;
	}
#ifdef GCSTAT
	fclose(global_gc_stat.fp);
#endif
}

/* ------------------------------------------------------------------------ */

typedef struct ObjectGraphTracer {
	KObjectVisitor base;
	GcContext  *gcContext;
	MarkStack  *mstack;
} ObjectGraphTracer;

static void ObjectGraphTracer_visit(KObjectVisitor *visitor, kObject *object)
{
	ObjectGraphTracer *tracer = (ObjectGraphTracer *) visitor;
	mark_mstack(tracer->gcContext, object, tracer->mstack);
}

static void ObjectGraphTracer_visitRange(KObjectVisitor *visitor, kObject **begin, kObject **end)
{
	ObjectGraphTracer *tracer = (ObjectGraphTracer *) visitor;
	kObject **itr;
	for (itr = begin; itr != end; ++itr) {
		mark_mstack(tracer->gcContext, *itr, tracer->mstack);
	}
}


/* ------------------------------------------------------------------------ */
static void inc_ref_cnt(kObject *obj)
{
	((kObjectVar*)obj)->h.refc++;
}

static void dec_ref_cnt(KonohaContext *kctx, kObject *obj, ObjectGraphTracer *tracer, MarkStack *mstack)
{
	kObject *ref = NULL;
	GcContext *gcContext = kctx->gcContext;
	((kObjectVar*)obj)->h.refc--;
	if(obj->h.refc < 0) {
		int page_size = obj->h.ct->cstruct_size / sizeof(kGCObject0) >> 1;

		KLIB kObjectProto_Reftrace(kctx, obj, &tracer->base);
		ref = mstack_next(mstack);
		OBJECT_REUSE(obj,page_size);
		if(unlikely(ref == NULL))
			return;
		do {
			printf("ref:0x%lx\n",(uintptr_t)ref);
			dec_ref_cnt(kctx, ref, tracer, mstack);
		} while((ref = mstack_next(mstack)) != NULL);
	}
}

static void Kwrite_barrier(KonohaContext *kctx, kObject *parent)
{
	//TODO
	printf("Write Barrier!!!!\n");
}

static void KupdateObjectField(KonohaContext *kctx, kObject *parent, kObject *oldValPtr, kObject *newVal)
{
	GcContext *gcContext      = kctx->gcContext;
	MarkStack *mstack         = mstack_Init(&gcContext->mstack);
	ObjectGraphTracer tracer  = {};
	tracer.base.fn_visit      = ObjectGraphTracer_visit;
	tracer.base.fn_visitRange = ObjectGraphTracer_visitRange;
	tracer.gcContext          = gcContext;
	tracer.mstack             = mstack;

	/*
	if(oldValPtr != NULL) {
		printf("inc_ref:0x%lx,count:%d,  dec_ref:0x%lx,count:%d\n",newVal,newVal->h.refc,oldValPtr,oldValPtr->h.refc);
	}else{
		//printf("inc_ref:0x%lx,count:%d\n",newVal,newVal->h.refc);
	}
	*/
	inc_ref_cnt(newVal);
	if(oldValPtr != NULL) {
		dec_ref_cnt(kctx, oldValPtr, &tracer, mstack);
	}
}

static void mark_mstack(GcContext *gcContext, kObject *ref, MarkStack *mstack)
{
	mstack_Push(mstack, ref);
}

/* ------------------------------------------------------------------------ */
static kObjectVar *KallocObject(KonohaContext *kctx, size_t size, KTraceInfo *trace)
{
	GcContext *gcContext = (GcContext *)kctx->gcContext;
	int page_size = (size / sizeof(kGCObject0)) >> 1;
	DBG_ASSERT(page_size <= 4);
	kGCObject *o = NULL;
	//printf("KallocObject:%zd\n",size);
	FREELIST_POP(o,page_size);
	gcContext->FreeList_size[page_size] -= 1;
	do_bzero((void*)o, size);
#if GCDEBUG
	OLDTRACE_SWITCH_TO_KTrace(LOGPOL_DEBUG,
			LogText("@", "new"),
			KeyValue_p("ptr", o),
			LogUint("size", size));
#endif
	return (kObjectVar *)o;
}

#define IS_Managed(n) do {\
	ObjectPageTable_t *oat = gcContext->ArenaTable[n];\
	size_t atindex, size = gcContext->ArenaTable_size[n];\
	for(atindex = 0; atindex < size; atindex++) {\
		uintptr_t start = (uintptr_t)oat[atindex].head##n;\
		uintptr_t end   = (uintptr_t)oat[atindex].bottom##n;\
		if(start < o && o < end) {\
			return true;\
		}\
	}\
} while (0)

static kbool_t KisObject(KonohaContext *kctx, void *ptr)
{
	GcContext *gcContext = (GcContext *)kctx->gcContext;
	uintptr_t o = (uintptr_t)ptr;
	IS_Managed(0);
	IS_Managed(1);
	IS_Managed(2);
	return false;
}

static void KscheduleGC(KonohaContext *kctx, KTraceInfo *trace)
{
	(void)kctx;(void)trace;
}
/* ------------------------------------------------------------------------ */

kbool_t LoadRCGCModule(KonohaFactory *factory, ModuleType type)
{
	static KModuleInfo ModuleInfo = {
		"RCGC", "0.1", 0, "rcgc",
	};
	factory->GCInfo            = &ModuleInfo;
	factory->Kmalloc = Kmalloc;
	factory->Kzmalloc = Kzmalloc;
	factory->Kfree = Kfree;
	factory->InitGcContext = KnewGcContext;
	factory->DeleteGcContext = KdeleteGcContext;
	factory->ScheduleGC = KscheduleGC;
	factory->AllocObject = KallocObject;
	factory->WriteBarrier = Kwrite_barrier;   // check this
	factory->UpdateObjectField = KupdateObjectField;  // check this
	factory->IsKonohaObject = KisObject;
	return true;
}

/* ------------------------------------------------------------------------ */
#ifdef __cplusplus
}
#endif
