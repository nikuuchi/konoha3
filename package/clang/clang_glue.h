#include "minikonoha/minikonoha.h"
#include "minikonoha/sugar.h"
#include "clang-c/Index.h"
#include <stdio.h>
#ifndef CLANG_GLUE_H_
#define CLANG_GLUE_H_

typedef struct _kIndex {
	KonohaObjectHeader h;
	CXIndex ptr;
} kIndex;

typedef struct _kTranslationUnit {
	KonohaObjectHeader h;
	CXTranslationUnit ptr;
} kTranslationUnit;

typedef struct _kClientData {
	KonohaObjectHeader h;
	CXClientData ptr;
} kClientData;

typedef struct _kCursor {
	KonohaObjectHeader h;
	CXCursor ptr;
} kCursor;

/* ------------------------------------------------------------- */
#define KIndex(o) (((kIndex*)o)->ptr)


/* ------------------------------------------------------------- */
static void Index_init(KonohaContext *kctx, kObject *o, void *conf)
{
}
static void Index_free(KonohaContext *kctx, kObject *o)
{
	clang_disposeIndex(KIndex(o));
}
static void Index_new(KonohaContext *kctx, KonohaStack *sfp)
{
	kIndex *index = (kIndex *)KLIB new_kObject(kctx, O_ct(sfp[K_RTNIDX].asObject), 0);
	index->ptr = clang_createIndex(0,0);
	RETURN_(index);
}

//TODO command_line_args
static void Index_parse(KonohaContext *kctx, KonohaStack *sfp)
{
	const char *source_filename = S_text(sfp[1].asString);
	kTranslationUnit *translationUnit = (kTranslationUnit*)KLIB new_kObject(kctx, O_ct(sfp[K_RTNIDX].asObject), 0);
	translationUnit->ptr = clang_parseTranslationUnit(KIndex((kIndex*)sfp[0].asObject),source_filename,NULL,0,NULL,0,0);
	RETURN_(translationUnit);
}

static void TranslationUnit_getCursor(KonohaContext *kctx, KonohaStack *sfp)
{
	kTranslationUnit *t = (kTranslationUnit *)sfp[0].asObject;
	CXCursor cursor = clang_getTranslationUnitCursor(t->ptr);
	kCursor *kcur = (kCursor *)KLIB new_kObject(kctx, O_ct(sfp[K_RTNIDX].asObject), 0);
	kcur->ptr = cursor;
	RETURN_(kcur);
}

static void TranslationUnit_init(KonohaContext *kctx, kObject *o, void *conf)
{
}

static void TranslationUnit_free(KonohaContext *kctx, kObject *o)
{
}

static void Cursor_init(KonohaContext *kctx, kObject *o, void *conf)
{
}

static void Cursor_free(KonohaContext *kctx, kObject *o)
{
}

//TODO
//int Cursor.getKindName()
static void Cursor_getKindName(KonohaContext *kctx, KonohaStack *sfp)
{
	kCursor *self = (kCursor *)sfp[0].asObject;
	int kind = self->ptr.kind;
	RETURNi_((kint_t)kind);

}

//String Cursor.getSpelling()
static void Cursor_getSpelling(KonohaContext *kctx, KonohaStack *sfp)
{
	kCursor *self = (kCursor *)sfp[0].asObject;
	const char *spelling = clang_getCString(clang_getCursorSpelling(self->ptr));
	RETURN_(KLIB new_kString(kctx, spelling, strlen(spelling), 0));
}

//Visitor pattern use in Cursor_getChildren()
enum CXChildVisitResult Visitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{

}

//Array[Cursor] Cursor.getChildren()
static void Cursor_getChildren(KonohaContext *kctx, KonohaStack *sfp)
{
	kCursor *self = (kCursor *)sfp[0].asObject;
	CXClientData data; // CXClientData is typedef void*, so I use Konoha Array.
	clang_visitChildren(self->ptr, Visitor,data);
}
/*
static KMETHOD CursorArray_newArray(KonohaContext *kctx, KonohaStack *sfp)
{
	kArrayVar *a = (kArrayVar *)sfp[0].asObject;
	size_t asize = (size_t)sfp[1].intValue;
	a->bytemax = asize * sizeof(void*);
	kArray_setsize((kArray*)a, asize);
	a->objectItems = (kObject**)KCALLOC(a->bytemax, 1);
	RETURN_(a);
}
*/
/*
//## void Cursor[].add(Cursor cur);
static KMETHOD CursorArray_add(KonohaContext *kctx, KonohaStack *sfp)
{
	kArrayVar *a = (kArrayVar *)sfp[0].asObject;
	CXClientData *ja = (CXClientData*)a->objectItems;

	kCursor *cur = (kCursor *)sfp[1].asObject;
	json_array_append(ja, json->obj);
	json_incref(json->obj);
	RETURNvoid_();
}

//## int Json[].getSize();
static KMETHOD JsonArray_getSize(KonohaContext *kctx, KonohaStack *sfp)
{
	kArray *a = sfp[0].asArray;
	const json_t *ja = (json_t*)a->objectItems;
	RETURNi_(json_array_size(ja));
}

static KMETHOD JsonArray_get(KonohaContext *kctx, KonohaStack *sfp)
{
	kArray *a = sfp[0].asArray;
	json_t *ja = (json_t*)a->objectItems;
	struct _kJson *json = (struct _kJson*)KLIB new_kObject(kctx, O_ct(sfp[K_RTNIDX].asObject), 0);
	json->obj = json_array_get(ja, sfp[1].intValue);
	RETURN_(json);
}

static KMETHOD JsonArray_append(KonohaContext *kctx, KonohaStack *sfp)
{
	kArray *a = sfp[0].asArray;
	json_t *ja = (json_t*)a->objectItems;
	kJson *json = (kJson*)sfp[1].asObject;
	json_array_append(ja, json->obj);
	RETURNvoid_();
}
*/
#endif /* end of include guard */
