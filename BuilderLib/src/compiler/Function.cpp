#include <iostream>
#include <inttypes.h>
#include <vector>
#include <string>
#include "Function.hpp"
#include "../third/Zydis/Zydis.h"

CFunction::CFunction(std::uint8_t* functionBuffer, std::size_t functionSize)
{
	m_FunctionBytes.resize(functionSize);
	std::memcpy(m_FunctionBytes.data(), functionBuffer, functionSize);
}

CFunction::CFunction(const std::vector<std::uint8_t>& functionBytes)
{
	m_FunctionBytes = functionBytes;
}

std::string CFunction::toString() const
{
    std::string result = "";

    // Initialize formatter. Only required when you actually plan to do instruction
    // formatting ("disassembling"), like we do here
    ZydisFormatter formatter;
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

    const auto disassembly = getDisassembly();
    ZyanU64 runtimeAddr = reinterpret_cast<ZyanU64>(m_FunctionBytes.data());

    for (const auto&[instruction, operands] : disassembly) {

        char buffer[256];
        ZydisFormatterFormatInstruction(&formatter, &instruction, operands,
            instruction.operand_count_visible, buffer, sizeof(buffer), runtimeAddr, ZYAN_NULL);

        result += buffer;
        result += "\n";
        runtimeAddr += instruction.length;
    }

    return result;
}

std::vector<INSTRUCTION> CFunction::getDisassembly() const
{
    std::vector<INSTRUCTION> result = {};

    // Initialize decoder context
    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

    INSTRUCTION currInstruction = {};
    ZyanUSize offset = 0;
    ZyanU64 runtimeAddr = reinterpret_cast<ZyanU64>(m_FunctionBytes.data());
    const ZyanUSize length = m_FunctionBytes.size();

    while (ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, m_FunctionBytes.data() + offset,
        length - offset, &currInstruction.instruction, currInstruction.operands))) {
        result.push_back(currInstruction);

        offset += currInstruction.instruction.length;
        runtimeAddr += currInstruction.instruction.length;
    }

    return result;
}

std::ostream& operator<<(std::ostream& stream, const CFunction& fn)
{
	return stream << fn.toString();
}