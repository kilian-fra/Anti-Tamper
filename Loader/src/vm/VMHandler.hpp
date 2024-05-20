#pragma once

#include <functional>
#include "VMContext.hpp"
#include "VM.hpp"

class VMHandler
{

	static bool isVipInBytecode(VMContext* pVmContext, std::uint8_t* vip);
	static void performOperationUnsigned(std::uint64_t* opSrc, std::uint64_t* opDst, std::uint8_t srcSize,
		std::uint8_t dstSize, std::function<std::uint64_t(std::uint64_t, std::uint64_t)> operation);

	static void performOperationSigned(std::int64_t* opSrc, std::int64_t* opDst, std::uint8_t srcSize,
		std::uint8_t dstSize, std::function<std::int64_t(std::int64_t, std::int64_t)> operation);

public:

	static void mov(VMContext* pVmContext);
	static void lea(VMContext* pVmContext);

	static void jcc(VMContext* pVmContext);
	static void jmp(VMContext* pVmContext);

	static void AND(VMContext* pVmContext);
	static void OR(VMContext* pVmContext);
	static void NOT(VMContext* pVmContext);
	static void XOR(VMContext* pVmContext);

	//Stack operations
	static void push(VMContext* pVmContext);
	static void pop(VMContext* pVmContext);

	//Arithmetic
	static void add(VMContext* pVmContext);
	static void sub(VMContext* pVmContext);

	//VMExit
	static void exit(VMContext* pVmContext);
};