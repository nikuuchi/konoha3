/* This file was automatically generated by GenLIR.k. Do not edit! */
#ifndef LIR_H
#define LIR_H

#define OPCODE_ThreadedCode 0
typedef struct OPThreadedCode {
	LIRHeader Header;
	uchar InvokedCount;
	uchar StackSize;
	ushort CodeSize;
} PACKED OPThreadedCode;

#define OPEXEC_ThreadedCode(PC) do {\
	uchar InvokedCount = ((OPThreadedCode *)PC)->InvokedCount;\
	uchar StackSize = ((OPThreadedCode *)PC)->StackSize;\
	ushort CodeSize = ((OPThreadedCode *)PC)->CodeSize;\
	InvokedCount += 1;\
	CodeThreading(PC, JUMP_TABLE);\
	((OPPrologue *)PC)->InvokedCount = InvokedCount;\
	(void)CodeSize;\
	return Val;\
} while(0)

#define OPFIELDSIZE_ThreadedCode 3
#define DUMP_ThreadedCode(OP_3) OP_3(ThreadedCode, uchar, InvokedCount, uchar, StackSize, ushort, CodeSize)

#define OPCODE_Prologue 1
typedef struct OPPrologue {
	LIRHeader Header;
	uchar InvokedCount;
	uchar StackSize;
	ushort CodeSize;
} PACKED OPPrologue;

#define OPEXEC_Prologue(PC) do {\
	uchar InvokedCount = ((OPPrologue *)PC)->InvokedCount;\
	uchar StackSize = ((OPPrologue *)PC)->StackSize;\
	ushort CodeSize = ((OPPrologue *)PC)->CodeSize;\
	InvokedCount += 1;\
	((OPPrologue *)PC)->InvokedCount = InvokedCount;\
	Stack += StackSize;\
	(void)CodeSize;\
	return Val;\
} while(0)

#define OPFIELDSIZE_Prologue 3
#define DUMP_Prologue(OP_3) OP_3(Prologue, uchar, InvokedCount, uchar, StackSize, ushort, CodeSize)

#define OPCODE_LoadConstant 2
typedef struct OPLoadConstant {
	LIRHeader Header;
	VMRegister Dst;
	SValue Src;
} PACKED OPLoadConstant;

#define OPEXEC_LoadConstant(PC) do {\
	VMRegister Dst = ((OPLoadConstant *)PC)->Dst;\
	SValue Src = ((OPLoadConstant *)PC)->Src;\
	Reg[Dst] = Src;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_LoadConstant 2
#define DUMP_LoadConstant(OP_2) OP_2(LoadConstant, VMRegister, Dst, SValue, Src)

#define OPCODE_LoadArgumentI 3
typedef struct OPLoadArgumentI {
	LIRHeader Header;
	VMRegister Dst;
	uint SrcIdx;
} PACKED OPLoadArgumentI;

#define OPEXEC_LoadArgumentI(PC) do {\
	VMRegister Dst = ((OPLoadArgumentI *)PC)->Dst;\
	uint SrcIdx = ((OPLoadArgumentI *)PC)->SrcIdx;\
	Reg[Dst].ival = Stack[SrcIdx].intValue;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_LoadArgumentI 2
#define DUMP_LoadArgumentI(OP_2) OP_2(LoadArgumentI, VMRegister, Dst, uint, SrcIdx)

#define OPCODE_LoadArgumentO 4
typedef struct OPLoadArgumentO {
	LIRHeader Header;
	VMRegister Dst;
	uint SrcIdx;
} PACKED OPLoadArgumentO;

#define OPEXEC_LoadArgumentO(PC) do {\
	VMRegister Dst = ((OPLoadArgumentO *)PC)->Dst;\
	uint SrcIdx = ((OPLoadArgumentO *)PC)->SrcIdx;\
	Reg[Dst].obj = Stack[SrcIdx].asObject;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_LoadArgumentO 2
#define DUMP_LoadArgumentO(OP_2) OP_2(LoadArgumentO, VMRegister, Dst, uint, SrcIdx)

#define OPCODE_LoadLocal 5
typedef struct OPLoadLocal {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister Src;
} PACKED OPLoadLocal;

#define OPEXEC_LoadLocal(PC) do {\
	VMRegister Dst = ((OPLoadLocal *)PC)->Dst;\
	VMRegister Src = ((OPLoadLocal *)PC)->Src;\
	Reg[Dst] = Reg[Src];\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_LoadLocal 2
#define DUMP_LoadLocal(OP_2) OP_2(LoadLocal, VMRegister, Dst, VMRegister, Src)

#define OPCODE_LoadField 6
typedef struct OPLoadField {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister Src;
	uint FieldIdx;
} PACKED OPLoadField;

#define OPEXEC_LoadField(PC) do {\
	VMRegister Dst = ((OPLoadField *)PC)->Dst;\
	VMRegister Src = ((OPLoadField *)PC)->Src;\
	uint FieldIdx = ((OPLoadField *)PC)->FieldIdx;\
	Reg[Dst] = Reg[Src].obj->fields[FieldIdx];\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_LoadField 3
#define DUMP_LoadField(OP_3) OP_3(LoadField, VMRegister, Dst, VMRegister, Src, uint, FieldIdx)

#define OPCODE_StoreLocal 7
typedef struct OPStoreLocal {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister Src;
} PACKED OPStoreLocal;

#define OPEXEC_StoreLocal(PC) do {\
	VMRegister Dst = ((OPStoreLocal *)PC)->Dst;\
	VMRegister Src = ((OPStoreLocal *)PC)->Src;\
	Reg[Dst] = Reg[Src];\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_StoreLocal 2
#define DUMP_StoreLocal(OP_2) OP_2(StoreLocal, VMRegister, Dst, VMRegister, Src)

#define OPCODE_StoreField 8
typedef struct OPStoreField {
	LIRHeader Header;
	VMRegister Dst;
	uint FieldIdx;
	VMRegister Src;
} PACKED OPStoreField;

#define OPEXEC_StoreField(PC) do {\
	VMRegister Dst = ((OPStoreField *)PC)->Dst;\
	uint FieldIdx = ((OPStoreField *)PC)->FieldIdx;\
	VMRegister Src = ((OPStoreField *)PC)->Src;\
	Reg[Dst].obj->fields[FieldIdx] = Reg[Src];\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_StoreField 3
#define DUMP_StoreField(OP_3) OP_3(StoreField, VMRegister, Dst, uint, FieldIdx, VMRegister, Src)

#define OPCODE_And 9
typedef struct OPAnd {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPAnd;

#define OPEXEC_And(PC) do {\
	VMRegister Dst = ((OPAnd *)PC)->Dst;\
	VMRegister LHS = ((OPAnd *)PC)->LHS;\
	VMRegister RHS = ((OPAnd *)PC)->RHS;\
	Reg[Dst].bval = Reg[LHS].bval && Reg[RHS].bval;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_And 3
#define DUMP_And(OP_3) OP_3(And, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Or 10
typedef struct OPOr {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPOr;

#define OPEXEC_Or(PC) do {\
	VMRegister Dst = ((OPOr *)PC)->Dst;\
	VMRegister LHS = ((OPOr *)PC)->LHS;\
	VMRegister RHS = ((OPOr *)PC)->RHS;\
	Reg[Dst].bval = Reg[LHS].bval || Reg[RHS].bval;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Or 3
#define DUMP_Or(OP_3) OP_3(Or, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_New 11
typedef struct OPNew {
	LIRHeader Header;
	VMRegister Dst;
	TypeId Type;
} PACKED OPNew;

#define OPEXEC_New(PC) do {\
	VMRegister Dst = ((OPNew *)PC)->Dst;\
	TypeId Type = ((OPNew *)PC)->Type;\
	Reg[Dst].obj = CreateObject(kctx, Type);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_New 2
#define DUMP_New(OP_2) OP_2(New, VMRegister, Dst, TypeId, Type)

#define OPCODE_Call 12
typedef struct OPCall {
	LIRHeader Header;
	VMRegister Dst;
	uchar ParamSize;
	uchar ReturnType;
	kMethodPtr Mtd;
	uintptr_t uline;
} PACKED OPCall;

#define OPEXEC_Call(PC) do {\
	VMRegister Dst = ((OPCall *)PC)->Dst;\
	uchar ParamSize = ((OPCall *)PC)->ParamSize;\
	uchar ReturnType = ((OPCall *)PC)->ReturnType;\
	kMethodPtr Mtd = ((OPCall *)PC)->Mtd;\
	uintptr_t uline = ((OPCall *)PC)->uline;\
	Reg[Dst] = CallMethod(kctx, Mtd, ParamSize, ReturnType, uline);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Call 5
#define DUMP_Call(OP_5) OP_5(Call, VMRegister, Dst, uchar, ParamSize, uchar, ReturnType, kMethodPtr, Mtd, uintptr_t, uline)

#define OPCODE_VCall 13
typedef struct OPVCall {
	LIRHeader Header;
	VMRegister Dst;
	uint ParamSize;
	Cache CacheInfo;
	uintptr_t uline;
} PACKED OPVCall;

#define OPEXEC_VCall(PC) do {\
	VMRegister Dst = ((OPVCall *)PC)->Dst;\
	uint ParamSize = ((OPVCall *)PC)->ParamSize;\
	Cache CacheInfo = ((OPVCall *)PC)->CacheInfo;\
	uintptr_t uline = ((OPVCall *)PC)->uline;\
	Reg[Dst] = CallMethodWithCache(kctx, Mtd, ParamSize, CacheInfo, uline);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_VCall 4
#define DUMP_VCall(OP_4) OP_4(VCall, VMRegister, Dst, uint, ParamSize, Cache, CacheInfo, uintptr_t, uline)

#define OPCODE_PushI 14
typedef struct OPPushI {
	LIRHeader Header;
	VMRegister Src;
} PACKED OPPushI;

#define OPEXEC_PushI(PC) do {\
	VMRegister Src = ((OPPushI *)PC)->Src;\
	PushUnboxedValue(kctx, Reg[Src]);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_PushI 1
#define DUMP_PushI(OP_1) OP_1(PushI, VMRegister, Src)

#define OPCODE_PushO 15
typedef struct OPPushO {
	LIRHeader Header;
	VMRegister Src;
} PACKED OPPushO;

#define OPEXEC_PushO(PC) do {\
	VMRegister Src = ((OPPushO *)PC)->Src;\
	PushBoxedValue(kctx, Reg[Src]);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_PushO 1
#define DUMP_PushO(OP_1) OP_1(PushO, VMRegister, Src)

#define OPCODE_Func 16
typedef struct OPFunc {
	LIRHeader Header;
	VMRegister Dst;
	IArray StackLayout;
} PACKED OPFunc;

#define OPEXEC_Func(PC) do {\
	VMRegister Dst = ((OPFunc *)PC)->Dst;\
	IArray StackLayout = ((OPFunc *)PC)->StackLayout;\
	Reg[Dst].obj = CreateFunction(Stack, StackLayout);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Func 2
#define DUMP_Func(OP_2) OP_2(Func, VMRegister, Dst, IArray, StackLayout)

#define OPCODE_Test 17
typedef struct OPTest {
	LIRHeader Header;
	VMRegister Src;
	TestFunc Func;
	Cache CacheInfo;
} PACKED OPTest;

#define OPEXEC_Test(PC) do {\
	VMRegister Src = ((OPTest *)PC)->Src;\
	TestFunc Func = ((OPTest *)PC)->Func;\
	Cache CacheInfo = ((OPTest *)PC)->CacheInfo;\
	AppendCache(Func, Reg[Src], CacheInfo);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Test 3
#define DUMP_Test(OP_3) OP_3(Test, VMRegister, Src, TestFunc, Func, Cache, CacheInfo)

#define OPCODE_ReturnI 18
typedef struct OPReturnI {
	LIRHeader Header;
	VMRegister Src;
} PACKED OPReturnI;

#define OPEXEC_ReturnI(PC) do {\
	VMRegister Src = ((OPReturnI *)PC)->Src;\
	Stack[K_RTNIDX].intValue = Reg[Src].ival;\
	return;\
} while(0)

#define OPFIELDSIZE_ReturnI 1
#define DUMP_ReturnI(OP_1) OP_1(ReturnI, VMRegister, Src)

#define OPCODE_ReturnO 19
typedef struct OPReturnO {
	LIRHeader Header;
	VMRegister Src;
} PACKED OPReturnO;

#define OPEXEC_ReturnO(PC) do {\
	VMRegister Src = ((OPReturnO *)PC)->Src;\
	Stack[K_RTNIDX].asObject = Reg[Src].obj;\
	return;\
} while(0)

#define OPFIELDSIZE_ReturnO 1
#define DUMP_ReturnO(OP_1) OP_1(ReturnO, VMRegister, Src)

#define OPCODE_ReturnVoid 20
typedef struct OPReturnVoid {
	LIRHeader Header;
} PACKED OPReturnVoid;

#define OPEXEC_ReturnVoid(PC) do {\
	return Val;\
} while(0)

#define OPFIELDSIZE_ReturnVoid 0
#define DUMP_ReturnVoid(OP_0) OP_0(ReturnVoid)

#define OPCODE_CondBr 21
typedef struct OPCondBr {
	LIRHeader Header;
	VMRegister Src;
	Address Block;
} PACKED OPCondBr;

#define OPEXEC_CondBr(PC) do {\
	VMRegister Src = ((OPCondBr *)PC)->Src;\
	Address Block = ((OPCondBr *)PC)->Block;\
	if(Reg[Src].bval) { PC = (ByteCode *)Block; DISPATCH_JUMPTO(Block); } else {}\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_CondBr 2
#define DUMP_CondBr(OP_2) OP_2(CondBr, VMRegister, Src, Address, Block)

#define OPCODE_Jump 22
typedef struct OPJump {
	LIRHeader Header;
	Address Block;
} PACKED OPJump;

#define OPEXEC_Jump(PC) do {\
	Address Block = ((OPJump *)PC)->Block;\
	PC = (ByteCode *)Block;\
	DISPATCH_JUMPTO(Block);\
} while(0)

#define OPFIELDSIZE_Jump 1
#define DUMP_Jump(OP_1) OP_1(Jump, Address, Block)

#define OPCODE_Throw 23
typedef struct OPThrow {
	LIRHeader Header;
	VMRegister Src;
	uintptr_t uline;
} PACKED OPThrow;

#define OPEXEC_Throw(PC) do {\
	VMRegister Src = ((OPThrow *)PC)->Src;\
	uintptr_t uline = ((OPThrow *)PC)->uline;\
	Raise(kctx, Stack, (kString *) Reg[Src].obj, uline);\
} while(0)

#define OPFIELDSIZE_Throw 2
#define DUMP_Throw(OP_2) OP_2(Throw, VMRegister, Src, uintptr_t, uline)

#define OPCODE_Try 24
typedef struct OPTry {
	LIRHeader Header;
	Address Catch;
	Address Finaly;
} PACKED OPTry;

#define OPEXEC_Try(PC) do {\
	Address Catch = ((OPTry *)PC)->Catch;\
	Address Finaly = ((OPTry *)PC)->Finaly;\
	Try_(Catch, Finaly, Stack);\
} while(0)

#define OPFIELDSIZE_Try 2
#define DUMP_Try(OP_2) OP_2(Try, Address, Catch, Address, Finaly)

#define OPCODE_Yield 25
typedef struct OPYield {
	LIRHeader Header;
	VMRegister Src;
} PACKED OPYield;

#define OPEXEC_Yield(PC) do {\
	VMRegister Src = ((OPYield *)PC)->Src;\
	Yield(Reg[Src], Stack);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Yield 1
#define DUMP_Yield(OP_1) OP_1(Yield, VMRegister, Src)

#define OPCODE_Not 26
typedef struct OPNot {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister Src;
} PACKED OPNot;

#define OPEXEC_Not(PC) do {\
	VMRegister Dst = ((OPNot *)PC)->Dst;\
	VMRegister Src = ((OPNot *)PC)->Src;\
	CompileTimeAssert((TypeOf(Src) == int));\
	Reg[Dst].ival = !(Reg[Src].ival);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Not 2
#define DUMP_Not(OP_2) OP_2(Not, VMRegister, Dst, VMRegister, Src)

#define OPCODE_Neg 27
typedef struct OPNeg {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister Src;
} PACKED OPNeg;

#define OPEXEC_Neg(PC) do {\
	VMRegister Dst = ((OPNeg *)PC)->Dst;\
	VMRegister Src = ((OPNeg *)PC)->Src;\
	CompileTimeAssert((TypeOf(Src) == int));\
	Reg[Dst].ival = -Reg[Src].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Neg 2
#define DUMP_Neg(OP_2) OP_2(Neg, VMRegister, Dst, VMRegister, Src)

#define OPCODE_Add 28
typedef struct OPAdd {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPAdd;

#define OPEXEC_Add(PC) do {\
	VMRegister Dst = ((OPAdd *)PC)->Dst;\
	VMRegister LHS = ((OPAdd *)PC)->LHS;\
	VMRegister RHS = ((OPAdd *)PC)->RHS;\
	CompileTimeAssert((TypeOf(LHS) == int && TypeOf(RHS) == int));\
	Reg[Dst].ival = Reg[LHS].ival + Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Add 3
#define DUMP_Add(OP_3) OP_3(Add, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Sub 29
typedef struct OPSub {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPSub;

#define OPEXEC_Sub(PC) do {\
	VMRegister Dst = ((OPSub *)PC)->Dst;\
	VMRegister LHS = ((OPSub *)PC)->LHS;\
	VMRegister RHS = ((OPSub *)PC)->RHS;\
	CompileTimeAssert((TypeOf(LHS) == int && TypeOf(RHS) == int));\
	Reg[Dst].ival = Reg[LHS].ival - Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Sub 3
#define DUMP_Sub(OP_3) OP_3(Sub, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Mul 30
typedef struct OPMul {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPMul;

#define OPEXEC_Mul(PC) do {\
	VMRegister Dst = ((OPMul *)PC)->Dst;\
	VMRegister LHS = ((OPMul *)PC)->LHS;\
	VMRegister RHS = ((OPMul *)PC)->RHS;\
	CompileTimeAssert((TypeOf(LHS) == int && TypeOf(RHS) == int));\
	Reg[Dst].ival = Reg[LHS].ival * Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Mul 3
#define DUMP_Mul(OP_3) OP_3(Mul, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Div 31
typedef struct OPDiv {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPDiv;

#define OPEXEC_Div(PC) do {\
	VMRegister Dst = ((OPDiv *)PC)->Dst;\
	VMRegister LHS = ((OPDiv *)PC)->LHS;\
	VMRegister RHS = ((OPDiv *)PC)->RHS;\
	CompileTimeAssert((TypeOf(LHS) == int && TypeOf(RHS) == int));\
	Reg[Dst].ival = Reg[LHS].ival / Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Div 3
#define DUMP_Div(OP_3) OP_3(Div, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Mod 32
typedef struct OPMod {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPMod;

#define OPEXEC_Mod(PC) do {\
	VMRegister Dst = ((OPMod *)PC)->Dst;\
	VMRegister LHS = ((OPMod *)PC)->LHS;\
	VMRegister RHS = ((OPMod *)PC)->RHS;\
	CompileTimeAssert(TypeOf(LHS) == int && TypeOf(RHS) == int);\
	Reg[Dst].ival = Reg[LHS].ival % Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Mod 3
#define DUMP_Mod(OP_3) OP_3(Mod, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_LShift 33
typedef struct OPLShift {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPLShift;

#define OPEXEC_LShift(PC) do {\
	VMRegister Dst = ((OPLShift *)PC)->Dst;\
	VMRegister LHS = ((OPLShift *)PC)->LHS;\
	VMRegister RHS = ((OPLShift *)PC)->RHS;\
	CompileTimeAssert(TypeOf(LHS) == int && TypeOf(RHS) == int);\
	Reg[Dst].ival = Reg[LHS].ival << Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_LShift 3
#define DUMP_LShift(OP_3) OP_3(LShift, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_RShift 34
typedef struct OPRShift {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPRShift;

#define OPEXEC_RShift(PC) do {\
	VMRegister Dst = ((OPRShift *)PC)->Dst;\
	VMRegister LHS = ((OPRShift *)PC)->LHS;\
	VMRegister RHS = ((OPRShift *)PC)->RHS;\
	CompileTimeAssert(TypeOf(LHS) == int && TypeOf(RHS) == int);\
	Reg[Dst].ival = Reg[LHS].ival >> Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_RShift 3
#define DUMP_RShift(OP_3) OP_3(RShift, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_LAnd 35
typedef struct OPLAnd {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPLAnd;

#define OPEXEC_LAnd(PC) do {\
	VMRegister Dst = ((OPLAnd *)PC)->Dst;\
	VMRegister LHS = ((OPLAnd *)PC)->LHS;\
	VMRegister RHS = ((OPLAnd *)PC)->RHS;\
	CompileTimeAssert(TypeOf(LHS) == int && TypeOf(RHS) == int);\
	Reg[Dst].ival = Reg[LHS].ival & Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_LAnd 3
#define DUMP_LAnd(OP_3) OP_3(LAnd, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_LOr 36
typedef struct OPLOr {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPLOr;

#define OPEXEC_LOr(PC) do {\
	VMRegister Dst = ((OPLOr *)PC)->Dst;\
	VMRegister LHS = ((OPLOr *)PC)->LHS;\
	VMRegister RHS = ((OPLOr *)PC)->RHS;\
	CompileTimeAssert(TypeOf(LHS) == int && TypeOf(RHS) == int);\
	Reg[Dst].ival = Reg[LHS].ival | Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_LOr 3
#define DUMP_LOr(OP_3) OP_3(LOr, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Xor 37
typedef struct OPXor {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPXor;

#define OPEXEC_Xor(PC) do {\
	VMRegister Dst = ((OPXor *)PC)->Dst;\
	VMRegister LHS = ((OPXor *)PC)->LHS;\
	VMRegister RHS = ((OPXor *)PC)->RHS;\
	CompileTimeAssert(TypeOf(LHS) == int && TypeOf(RHS) == int);\
	Reg[Dst].ival = Reg[LHS].ival ^ Reg[RHS].ival;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Xor 3
#define DUMP_Xor(OP_3) OP_3(Xor, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Eq 38
typedef struct OPEq {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPEq;

#define OPEXEC_Eq(PC) do {\
	VMRegister Dst = ((OPEq *)PC)->Dst;\
	VMRegister LHS = ((OPEq *)PC)->LHS;\
	VMRegister RHS = ((OPEq *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].ival == Reg[RHS].ival);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Eq 3
#define DUMP_Eq(OP_3) OP_3(Eq, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Nq 39
typedef struct OPNq {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPNq;

#define OPEXEC_Nq(PC) do {\
	VMRegister Dst = ((OPNq *)PC)->Dst;\
	VMRegister LHS = ((OPNq *)PC)->LHS;\
	VMRegister RHS = ((OPNq *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].ival != Reg[RHS].ival);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Nq 3
#define DUMP_Nq(OP_3) OP_3(Nq, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Gt 40
typedef struct OPGt {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPGt;

#define OPEXEC_Gt(PC) do {\
	VMRegister Dst = ((OPGt *)PC)->Dst;\
	VMRegister LHS = ((OPGt *)PC)->LHS;\
	VMRegister RHS = ((OPGt *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].ival >  Reg[RHS].ival);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Gt 3
#define DUMP_Gt(OP_3) OP_3(Gt, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Ge 41
typedef struct OPGe {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPGe;

#define OPEXEC_Ge(PC) do {\
	VMRegister Dst = ((OPGe *)PC)->Dst;\
	VMRegister LHS = ((OPGe *)PC)->LHS;\
	VMRegister RHS = ((OPGe *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].ival >= Reg[RHS].ival);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Ge 3
#define DUMP_Ge(OP_3) OP_3(Ge, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Lt 42
typedef struct OPLt {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPLt;

#define OPEXEC_Lt(PC) do {\
	VMRegister Dst = ((OPLt *)PC)->Dst;\
	VMRegister LHS = ((OPLt *)PC)->LHS;\
	VMRegister RHS = ((OPLt *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].ival <  Reg[RHS].ival);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Lt 3
#define DUMP_Lt(OP_3) OP_3(Lt, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Le 43
typedef struct OPLe {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPLe;

#define OPEXEC_Le(PC) do {\
	VMRegister Dst = ((OPLe *)PC)->Dst;\
	VMRegister LHS = ((OPLe *)PC)->LHS;\
	VMRegister RHS = ((OPLe *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].ival <= Reg[RHS].ival);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Le 3
#define DUMP_Le(OP_3) OP_3(Le, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FNeg 44
typedef struct OPFNeg {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister Src;
} PACKED OPFNeg;

#define OPEXEC_FNeg(PC) do {\
	VMRegister Dst = ((OPFNeg *)PC)->Dst;\
	VMRegister Src = ((OPFNeg *)PC)->Src;\
	CompileTimeAssert((TypeOf(Src) == float));\
	Reg[Dst].fval = -Reg[Src].fval;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FNeg 2
#define DUMP_FNeg(OP_2) OP_2(FNeg, VMRegister, Dst, VMRegister, Src)

#define OPCODE_FAdd 45
typedef struct OPFAdd {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFAdd;

#define OPEXEC_FAdd(PC) do {\
	VMRegister Dst = ((OPFAdd *)PC)->Dst;\
	VMRegister LHS = ((OPFAdd *)PC)->LHS;\
	VMRegister RHS = ((OPFAdd *)PC)->RHS;\
	CompileTimeAssert((TypeOf(LHS) == float && TypeOf(RHS) == float));\
			(TypeOf(LHS) == float || TypeOf(RHS) == float));\
	Reg[Dst].fval = Reg[LHS].fval + Reg[RHS].fval;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FAdd 3
#define DUMP_FAdd(OP_3) OP_3(FAdd, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FSub 46
typedef struct OPFSub {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFSub;

#define OPEXEC_FSub(PC) do {\
	VMRegister Dst = ((OPFSub *)PC)->Dst;\
	VMRegister LHS = ((OPFSub *)PC)->LHS;\
	VMRegister RHS = ((OPFSub *)PC)->RHS;\
	CompileTimeAssert((TypeOf(LHS) == float && TypeOf(RHS) == float));\
			(TypeOf(LHS) == float || TypeOf(RHS) == float));\
	Reg[Dst].fval = Reg[LHS].fval - Reg[RHS].fval;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FSub 3
#define DUMP_FSub(OP_3) OP_3(FSub, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FMul 47
typedef struct OPFMul {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFMul;

#define OPEXEC_FMul(PC) do {\
	VMRegister Dst = ((OPFMul *)PC)->Dst;\
	VMRegister LHS = ((OPFMul *)PC)->LHS;\
	VMRegister RHS = ((OPFMul *)PC)->RHS;\
	CompileTimeAssert((TypeOf(LHS) == float && TypeOf(RHS) == float));\
	Reg[Dst].fval = Reg[LHS].fval * Reg[RHS].fval;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FMul 3
#define DUMP_FMul(OP_3) OP_3(FMul, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FDiv 48
typedef struct OPFDiv {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFDiv;

#define OPEXEC_FDiv(PC) do {\
	VMRegister Dst = ((OPFDiv *)PC)->Dst;\
	VMRegister LHS = ((OPFDiv *)PC)->LHS;\
	VMRegister RHS = ((OPFDiv *)PC)->RHS;\
	CompileTimeAssert(TypeOf(LHS) == float || TypeOf(RHS) == float);\
	Reg[Dst].fval = Reg[LHS].fval / Reg[RHS].fval;\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FDiv 3
#define DUMP_FDiv(OP_3) OP_3(FDiv, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FEq 49
typedef struct OPFEq {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFEq;

#define OPEXEC_FEq(PC) do {\
	VMRegister Dst = ((OPFEq *)PC)->Dst;\
	VMRegister LHS = ((OPFEq *)PC)->LHS;\
	VMRegister RHS = ((OPFEq *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].fval == Reg[RHS].fval);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FEq 3
#define DUMP_FEq(OP_3) OP_3(FEq, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FNq 50
typedef struct OPFNq {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFNq;

#define OPEXEC_FNq(PC) do {\
	VMRegister Dst = ((OPFNq *)PC)->Dst;\
	VMRegister LHS = ((OPFNq *)PC)->LHS;\
	VMRegister RHS = ((OPFNq *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].fval != Reg[RHS].fval);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FNq 3
#define DUMP_FNq(OP_3) OP_3(FNq, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FGt 51
typedef struct OPFGt {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFGt;

#define OPEXEC_FGt(PC) do {\
	VMRegister Dst = ((OPFGt *)PC)->Dst;\
	VMRegister LHS = ((OPFGt *)PC)->LHS;\
	VMRegister RHS = ((OPFGt *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].fval >  Reg[RHS].fval);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FGt 3
#define DUMP_FGt(OP_3) OP_3(FGt, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FGe 52
typedef struct OPFGe {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFGe;

#define OPEXEC_FGe(PC) do {\
	VMRegister Dst = ((OPFGe *)PC)->Dst;\
	VMRegister LHS = ((OPFGe *)PC)->LHS;\
	VMRegister RHS = ((OPFGe *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].fval >= Reg[RHS].fval);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FGe 3
#define DUMP_FGe(OP_3) OP_3(FGe, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FLt 53
typedef struct OPFLt {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFLt;

#define OPEXEC_FLt(PC) do {\
	VMRegister Dst = ((OPFLt *)PC)->Dst;\
	VMRegister LHS = ((OPFLt *)PC)->LHS;\
	VMRegister RHS = ((OPFLt *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].fval <  Reg[RHS].fval);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FLt 3
#define DUMP_FLt(OP_3) OP_3(FLt, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_FLe 54
typedef struct OPFLe {
	LIRHeader Header;
	VMRegister Dst;
	VMRegister LHS;
	VMRegister RHS;
} PACKED OPFLe;

#define OPEXEC_FLe(PC) do {\
	VMRegister Dst = ((OPFLe *)PC)->Dst;\
	VMRegister LHS = ((OPFLe *)PC)->LHS;\
	VMRegister RHS = ((OPFLe *)PC)->RHS;\
	Reg[Dst].bval = (Reg[LHS].ival <= Reg[RHS].ival);\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_FLe 3
#define DUMP_FLe(OP_3) OP_3(FLe, VMRegister, Dst, VMRegister, LHS, VMRegister, RHS)

#define OPCODE_Nop 55
typedef struct OPNop {
	LIRHeader Header;
} PACKED OPNop;

#define OPEXEC_Nop(PC) do {\
	DISPATCH_NEXT(PC);\
} while(0)

#define OPFIELDSIZE_Nop 0
#define DUMP_Nop(OP_0) OP_0(Nop)

#define BYTECODE_LIST(OP)\
	OP(ThreadedCode)\
	OP(Prologue)\
	OP(LoadConstant)\
	OP(LoadArgumentI)\
	OP(LoadArgumentO)\
	OP(LoadLocal)\
	OP(LoadField)\
	OP(StoreLocal)\
	OP(StoreField)\
	OP(And)\
	OP(Or)\
	OP(New)\
	OP(Call)\
	OP(VCall)\
	OP(PushI)\
	OP(PushO)\
	OP(Func)\
	OP(Test)\
	OP(ReturnI)\
	OP(ReturnO)\
	OP(ReturnVoid)\
	OP(CondBr)\
	OP(Jump)\
	OP(Throw)\
	OP(Try)\
	OP(Yield)\
	OP(Not)\
	OP(Neg)\
	OP(Add)\
	OP(Sub)\
	OP(Mul)\
	OP(Div)\
	OP(Mod)\
	OP(LShift)\
	OP(RShift)\
	OP(LAnd)\
	OP(LOr)\
	OP(Xor)\
	OP(Eq)\
	OP(Nq)\
	OP(Gt)\
	OP(Ge)\
	OP(Lt)\
	OP(Le)\
	OP(FNeg)\
	OP(FAdd)\
	OP(FSub)\
	OP(FMul)\
	OP(FDiv)\
	OP(FEq)\
	OP(FNq)\
	OP(FGt)\
	OP(FGe)\
	OP(FLt)\
	OP(FLe)\
	OP(Nop)

#endif /* end of include guard */