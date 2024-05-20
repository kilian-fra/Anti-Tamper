#pragma once

class VMDisassembler
{
	//std::vector<std::uint8_t> m_Bytecode;
	std::uint8_t* m_PtrBytecode;
	std::size_t m_Size;
	std::string m_Disassembly;

	std::string fetchInstruction(VM_OPCODE vm_opcode);
	std::string fetchFirstOperand(std::uint8_t* vip);
	std::string fetchSecondOperand(std::uint8_t* vip);
	std::size_t getInstructionLength(std::uint8_t* vip);
	std::uint8_t getInstructionOpCount(VM_OPCODE vm_opcode);

public:

	VMDisassembler(std::uint8_t* p, std::size_t size);

	void disassemble();
	auto getDisassembly() const { return m_Disassembly; }
};