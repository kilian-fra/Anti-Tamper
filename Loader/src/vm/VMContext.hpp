#pragma once

#include "VMStack.hpp"

class VMContext
{
	std::uint8_t* m_PtrBytecode;
	std::size_t m_BytecodeSize;
	std::uint8_t* m_PtrRealFunc;
	std::uint8_t* m_Vip; //virtual instruction pointer
	VMStack m_Stack;

public:

	VMContext(std::uint8_t* pBytecode, std::size_t bytecodeSize, std::uint8_t* pBaseFunc);
	VGPR m_Gpr; //general purpose registers
	RFLAGS m_Flags;

	inline auto incVip(std::uint64_t offset) {
		m_Vip += offset;
	}

	inline auto& getStack() { return m_Stack; }

	inline auto incVip(std::int64_t offset) {
		m_Vip += offset;
	}

	inline auto setVip(std::uint8_t* newVip) {
		m_Vip = newVip;
	}

	inline auto getVip() const {
		return m_Vip;
	}

	inline auto getBytecodePtr() const {
		return m_PtrBytecode;
	}

	inline auto getPtrRealFunc() const {
		return m_PtrRealFunc;
	}

	inline auto getBytecodeSize() const {
		return m_BytecodeSize;
	}
};