#include "clang_glue.h"

// --------------------------------------------------------------------------

#define _P kMethod_Public
#define _C kMethod_Const
#define _S kMethod_Static
#define _F(F)   (intptr_t)(F)

#define CT_Index     cIndex
#define TY_Index  (cIndex->typeId)

#define CT_TranslationUnit     cTranslationUnit
#define TY_TranslationUnit  (cTranslationUnit->typeId)

#define CT_Cursor     cCursor
#define TY_Cursor  (cCursor->typeId)

static kbool_t clang_initPackage(KonohaContext *kctx, kNameSpace *ns, int argc, const char**args, kfileline_t pline)
{
	static KDEFINE_CLASS IndexDef = {
			STRUCTNAME(Index),
			.cflag = kClass_Final,
			.init = Index_init,
			.free = Index_free,
	};
	static KDEFINE_CLASS TranslationUnitDef = {
			STRUCTNAME(TranslationUnit),
			.cflag = kClass_Final,
			.init = TranslationUnit_init,
			.free = TranslationUnit_free,
	};
	static KDEFINE_CLASS CursorDef = {
			STRUCTNAME(Cursor),
			.cflag = kClass_Final,
			.init = Cursor_init,
			.free = Cursor_free,
	};
	KonohaClass *cIndex = KLIB Konoha_defineClass(kctx, ns->packageId, ns->packageDomain, NULL, &IndexDef, pline);
	KonohaClass *cTranslationUnit = KLIB Konoha_defineClass(kctx, ns->packageId, ns->packageDomain, NULL, &TranslationUnitDef, pline);
	KonohaClass *cCursor = KLIB Konoha_defineClass(kctx, ns->packageId, ns->packageDomain, NULL, &CursorDef, pline);

	KDEFINE_METHOD MethodData[] = {
		_P, _F(Index_new),   TY_Index,   TY_Index, MN_("new"), 0,
		_P, _F(Index_parse),   TY_TranslationUnit,   TY_Index, MN_("parse"), 1, TY_String, FN_("filename"),
		_P, _F(TranslationUnit_getCursor),   TY_Cursor,   TY_TranslationUnit, MN_("getCursor"), 0,
		_P, _F(Cursor_getKindName),   TY_Int,   TY_Cursor, MN_("getKindName"), 0,
		_P, _F(Cursor_getSpelling),   TY_String,   TY_Cursor, MN_("getSpelling"), 0,
		DEND,
	};
	KLIB kNameSpace_loadMethodData(kctx, ns, MethodData);
	return true;
}

static kbool_t clang_setupPackage(KonohaContext *kctx, kNameSpace *ns, isFirstTime_t isFirstTime, kfileline_t pline)
{
	return true;
}

static kbool_t clang_initNameSpace(KonohaContext *kctx,  kNameSpace *ns, kfileline_t pline)
{
	return true;
}

static kbool_t clang_setupNameSpace(KonohaContext *kctx, kNameSpace *ns, kfileline_t pline)
{
	return true;
}
KDEFINE_PACKAGE* clang_init(void)
{
	static KDEFINE_PACKAGE d = {
		KPACKNAME("clang", "1.0"),
		.initPackage = clang_initPackage,
		.setupPackage = clang_setupPackage,
		.initNameSpace = clang_initNameSpace,
		.setupNameSpace = clang_setupNameSpace,
	};
	return &d;
}

