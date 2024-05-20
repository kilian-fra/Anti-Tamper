#include <iostream>
#include "VM.hpp"
#include "VMContext.hpp"

VMContext::VMContext(std::uint8_t* pBytecode, std::size_t bytecodeSize, std::uint8_t* pBaseFunc)
	: m_PtrBytecode(pBytecode), m_BytecodeSize(bytecodeSize), m_PtrRealFunc(pBaseFunc), m_Vip(m_PtrBytecode),
		m_Gpr({}), m_Flags({})
{}