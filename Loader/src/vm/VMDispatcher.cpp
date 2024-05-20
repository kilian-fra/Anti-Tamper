#include <iostream>
#include <vector>
#include "VM.hpp"
#include "VMDispatcher.hpp"

VMDispatcher::VMDispatcher(const VMContext& vmContext)
	: m_VmContext(vmContext)
{
}

void VMDispatcher::dispatch()
{
	const auto bytecodeSize = m_VmContext.getBytecodeSize();
	const auto pRealFunc = m_VmContext.getPtrRealFunc();

	bool bIsVmExit = false;

	//Dispatch until VM_EXIT is reached
	while (!bIsVmExit) {

		//Fetch current opcode
		auto opcode = static_cast<VM_OPCODE>(*m_VmContext.getVip());

		//Decode operands



		//Execute VM_INSTRUCTION

		//Update vm context
		
	}
}