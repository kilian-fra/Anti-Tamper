#pragma once

#include "../../Function.hpp"
#include "../../../third/Zydis/Zydis.h"
#include "VM.hpp"
#include "VMInstruction.hpp"

constexpr auto BYTECODE_EXTEND = 512U;

typedef struct {
	ZydisDecodedInstruction instruction;
	ZydisDecodedOperand op1;
	std::uint8_t* rip;
}REALLOC_IMM_RIP_REL;

class CVirtualizer
{
	CFunction* m_TargetFunction;
	std::uint8_t* m_pBaseFunc;
	std::uint8_t* m_realRip;
	std::vector<std::uint8_t> m_Bytecode;
	std::size_t m_BytecodeSize;
	std::vector<std::uint32_t> m_ImmRealloc;
	std::uint32_t m_currInstructionIndex;
	std::uint32_t m_funcSize;

	std::uint8_t getExplicitOperandCount(const ZydisDecodedOperand* operands);
	std::uint8_t getImmSizeFlag(ZyanU16 immSize);
	VM_REG_OPERAND getVirtualRegOperand(ZydisRegister reg);
	VM_OPCODE getVirtualOpcode(ZydisMnemonic mnemonic);
	int64_t handleDispRipRelative(int64_t disp, ZyanU8 instructionLength);

	void fixImmRipRelativeInstructions(const std::vector<std::size_t>& vmInstructionOffsets,
		const std::vector< REALLOC_IMM_RIP_REL>& nativeInstructions);

	template <typename T>
	T handleImmRipRelative(T imm, ZyanU8 instructionLength) {
		//Handle relative imm that is outside of the bytecode (call and jmp most likely)
		if (imm > 0) {
			if ((m_realRip + imm + instructionLength) > (m_pBaseFunc + m_funcSize)
				|| (m_realRip + imm + instructionLength) < m_pBaseFunc)
				return static_cast<T>(imm + (m_realRip - m_pBaseFunc) + instructionLength);
		}
		else {
			if ((m_realRip + imm - instructionLength) > (m_pBaseFunc + m_funcSize)
				|| (m_realRip + imm - instructionLength) < m_pBaseFunc)
				return static_cast<T>(imm - ((m_realRip - m_pBaseFunc) + instructionLength));
		}

		//Handle relative imm that is inside of the bytecode (jcc most likely)
		m_ImmRealloc.push_back(m_currInstructionIndex); //Store the index of the current instruction for realloc
		return imm;
	}

	void fixImmSizeOp1(std::uint8_t flags) {
		VM_OPCODE_OP1_FLAGS op1flags = {};
		op1flags.flags = flags;

		if (!op1flags.isImmediateUsed)
			return;

		switch (op1flags.immSize) {
			case VM_IMM_8:
				m_BytecodeSize -= 7;
				break;
			case VM_IMM_16:
				m_BytecodeSize -= 6;
				break;
			case VM_IMM_32:
				m_BytecodeSize -= 4;
				break;
			case VM_IMM_64: break;
		}
	}

	void fixImmSizeOp2(std::uint8_t flags) {
		VM_OPCODE_OP2_FLAGS op2flags = {};
		op2flags.flags = flags;

		if (!op2flags.isImmediateUsed)
			return;

		//Opcode (1) + Flags (1) + op1 (1) = 3 bytes

		const auto oldBytecodeSize = m_BytecodeSize;
		auto p = m_Bytecode.data() + m_BytecodeSize;

		if (op2flags.hasDisplacement && op2flags.hasScaleIndex)
			p -= 10;
		else if (op2flags.hasDisplacement && !op2flags.hasScaleIndex)
			p -= 8;
		else if (!op2flags.hasDisplacement && op2flags.hasScaleIndex)
			p -= 2;

		switch (op2flags.immSize) {
			case VM_IMM_8:
				m_BytecodeSize -= 7;
				break;
			case VM_IMM_16:
				m_BytecodeSize -= 6;
				break;
			case VM_IMM_32:
				m_BytecodeSize -= 4;
				break;
			case VM_IMM_64: return;
		}

		if (op2flags.hasDisplacement && op2flags.hasScaleIndex)
			std::memcpy(p - (oldBytecodeSize - m_BytecodeSize), p, 10);
		else if (op2flags.hasDisplacement && !op2flags.hasScaleIndex)
			std::memcpy(p - (oldBytecodeSize - m_BytecodeSize), p, 8);
		else if (!op2flags.hasDisplacement && op2flags.hasScaleIndex)
			std::memcpy(p - (oldBytecodeSize - m_BytecodeSize), p, 2);
	}

	void addVmInstruction(const VMInstruction& vmInstruction) {
		if (m_BytecodeSize + vmInstruction.getSize() > m_Bytecode.capacity())
			m_Bytecode.resize(m_BytecodeSize + BYTECODE_EXTEND);

		std::memcpy(m_Bytecode.data() + m_BytecodeSize, vmInstruction.getBytes().data(), vmInstruction.getSize());
		m_BytecodeSize += vmInstruction.getSize();
	}

	//template <typename T>
	//void addVmInstruction(VM_OPCODE opcode, std::uint8_t flags, T op1) {
	//	std::vector<std::uint8_t> bytes(2 + sizeof(T));
	//	*bytes.data() = (std::uint8_t)opcode;
	//	*(bytes.data() + 1) = flags;
	//	std::memcpy(bytes.data() + 2, &op1, sizeof(T));
	//	addVmInstruction(bytes);
	//}

	//template <typename T, typename U>
	//void addVmInstruction(VM_OPCODE opcode, std::uint8_t flags, T op1, U op2) {
	//	addVmInstruction(opcode, flags, op1);
	//	std::vector<std::uint8_t> bytes(sizeof(U));
	//	std::memcpy(bytes.data(), &op2, sizeof(U));
	//	addVmInstruction(bytes);
	//}

	//template <typename T, typename U>
	//void addVmInstruction(VM_OPCODE opcode, std::uint8_t flags, T op1, U op2, int64_t displacement) {
	//	addVmInstruction(opcode, flags, op1, op2);
	//	std::vector<std::uint8_t> bytes(sizeof(int64_t));
	//	std::memcpy(bytes.data(), &displacement, sizeof(int64_t));
	//	addVmInstruction(bytes);
	//}

	//template <typename T, typename U>
	//void addVmInstruction(VM_OPCODE opcode, std::uint8_t flags, T op1, U op2, int64_t displacement,
	//	std::uint8_t scale, VM_REG_OPERAND index) {
	//	addVmInstruction(opcode, flags, op1, op2, displacement);
	//	addVmInstruction({ scale, (std::uint8_t)index });
	//}

	//template <typename T, typename U>
	//void addVmInstruction(VM_OPCODE opcode, std::uint8_t flags, T op1, U op2, std::uint8_t scale, VM_REG_OPERAND index) {
	//	addVmInstruction(opcode, flags, op1, op2);
	//	addVmInstruction({ scale, (std::uint8_t)index });
	//}

	//template <typename T>
	//void addVmInstruction(VM_OPCODE opcode, std::uint8_t flags, T op1, int64_t displacement) {
	//	addVmInstruction(opcode, flags, op1);
	//	std::vector<std::uint8_t> bytes(sizeof(int64_t));
	//	std::memcpy(bytes.data(), &displacement, sizeof(int64_t));
	//	addVmInstruction(bytes);
	//}

	//template <typename T>
	//void addVmInstruction(VM_OPCODE opcode, std::uint8_t flags, T op1, int64_t displacement,
	//	std::uint8_t scale, VM_REG_OPERAND index) {
	//	addVmInstruction(opcode, flags, op1, displacement);
	//	addVmInstruction({ scale, (std::uint8_t)index });
	//}

	//template <typename T>
	//void addVmInstruction(VM_OPCODE opcode, std::uint8_t flags, T op1, std::uint8_t scale, VM_REG_OPERAND index) {
	//	addVmInstruction(opcode, flags, op1);
	//	addVmInstruction({ scale, (std::uint8_t)index });
	//}

	void handleNativeInstruction(VM_OPCODE vm_opcode, const ZydisDecodedOperand& op1, const ZydisDecodedOperand& op2, ZyanU8 instructionLength);
	void handleNativeInstruction(VM_OPCODE vm_opcode, const ZydisDecodedOperand& op1, ZyanU8 instructionLength);
	void handleNativeInstruction(VM_OPCODE vm_opcode);

	/*void handleCall(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);
	void handleUncondJmp(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);
	void handleCondJmp(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);
	void handleDataTransfer(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);
	void handleStackOperation(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);
	void handleGenArithmetic(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);
	void handleRotShift(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);
	void handleLogicalOperation(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);
	void handleCmpTest(const ZydisDecodedInstruction& instruction, ZydisDecodedOperand operands[ZYDIS_MAX_OPERAND_COUNT]);*/

public:

	CVirtualizer(CFunction* function, std::uint8_t* pBaseFunc, std::uint32_t funcSize);
	void transform();

};