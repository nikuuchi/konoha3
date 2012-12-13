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
	printf("malloc:%zd\n",size);
	void *ptr = malloc(size);
	do_bzero(ptr, size);
	return ptr;
}

static inline void *do_calloc(size_t count, size_t size)
{
	printf("calloc:%zd\n",size*count);
	void *ptr = calloc(count, size);
	return ptr;
}

static inline void *do_realloc(void *ptr, size_t oldSize, size_t newSize)
{
	printf("realloc:%zd\n",newSize);
	char *newptr = (char *) realloc(ptr, newSize);
	do_bzero(newptr+oldSize, newSize-oldSize);
	return (void *) newptr;
}

static inline void do_Free(void *ptr, size_t size)
{
	printf("free:%zd\n",size);
	//memset(ptr, 0xa, size);
	free(ptr);
}

static void *Kmalloc(KonohaContext *kctx, size_t s, KTraceInfo *trace)
{
	size_t *p = (size_t *)do_malloc(s);
	return (void *)p;
}

static void *Kzmalloc(KonohaContext *kctx, size_t s, KTraceInfo *trace)
{
	size_t *p = (size_t *)do_calloc(1, s);
	return (void *)(p);
}

static void Kfree(KonohaContext *kctx, void *p, size_t s)
{
	size_t *pp = (size_t *)p;
	do_Free(pp, s);
}

static void KdeleteGcContext(KonohaContext *kctx)
{
}

static void KnewGcContext(KonohaContext *kctx)
{
}
static void Kwrite_barrier(KonohaContext *kctx, kObject *parent)
{
	printf("Write Barrier\n");
}

static void DeclementReference(/*KonohaContext *kctx,*/ kObject *oldValPtr)
{
	((kObjectVar*)oldValPtr)->h.refc--;
	if(oldValPtr->h.refc < 0) {
		do_Free(oldValPtr, oldValPtr->h.ct->cstruct_size);
	}
}

static void KupdateObjectField(kObject *parent, kObject *oldValPtr, kObject *newVal)
{
	((kObjectVar*)(newVal))->h.refc++;
	if(oldValPtr != NULL) {
		if() {
			DeclementReference(oldValPtr);
		}
	}
}

static kObjectVar *KallocObject(KonohaContext *kctx, size_t size, KTraceInfo *trace)
{
	kObjectVar* o = (kObjectVar*)do_malloc(size);
	o->h.refc = 0;
	return o;
}

static kbool_t KisObject(KonohaContext *kctx, void *ptr)
{
	return (((kObject*)ptr)->h.refc > 0) ? true : false;
}

static void KscheduleGC(KonohaContext *kctx, KTraceInfo *trace)
{
}
/* ------------------------------------------------------------------------ */

kbool_t LoadrcgcModule(KonohaFactory *factory, ModuleType type)
{
	static KModuleInfo ModuleInfo = {
		"rcgc", "0.1", 0, "rcgc",
	};
	factory->GCInfo            = &ModuleInfo;
	factory->Kmalloc           = Kmalloc;
	factory->Kzmalloc          = Kzmalloc;
	factory->Kfree             = Kfree;
	factory->InitGcContext     = KnewGcContext;
	factory->DeleteGcContext   = KdeleteGcContext;
	factory->ScheduleGC        = KscheduleGC;
	factory->AllocObject       = KallocObject;
	factory->WriteBarrier      = Kwrite_barrier;   // check this
	factory->UpdateObjectField = KupdateObjectField;  // check this
	factory->IsKonohaObject    = KisObject;
	return true;
}

/* ------------------------------------------------------------------------ */
#ifdef __cplusplus
}
#endif
