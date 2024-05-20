#pragma once
#include "../third/Zydis/Zydis.h"

typedef struct {
	ZydisDecodedInstruction instruction;
	ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT];
}INSTRUCTION, *PINSTRUCTION;

class CFunction
{
	std::vector<std::uint8_t> m_FunctionBytes;

public:

	CFunction(std::uint8_t* functionBuffer, std::size_t functionSize);
	CFunction(const std::vector<std::uint8_t>& functionBytes);

	//Return disassembly of the function
	std::string toString() const;
	std::vector<INSTRUCTION> getDisassembly() const;
};

std::ostream& operator<< (std::ostream& stream, const CFunction& fn);