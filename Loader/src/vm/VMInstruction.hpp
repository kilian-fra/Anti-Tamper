#pragma once

#include "VMContext.hpp"
#include "VM.hpp"

typedef struct {
	void* pOp1;
	std::uint8_t op1SizeBytes;
}VM_INSTRUCTION_OP1;

typedef struct {
	void* pOp1;
	void* pOp2;
	std::uint8_t op1SizeBytes;
	std::uint8_t op2SizeBytes;
}VM_INSTRUCTION_OP2;

class VMInstruction
{
	VMContext* m_pVmContext;
	VM_OPCODE m_VmOpcode;

	std::uint8_t getOpCount() const;

public:

	VMInstruction(VMContext* pVmContext, VM_OPCODE vmOpcode);

	void* getRegOperandPtr(VM_REG_OPERAND regOperand, std::uint8_t* regSize);

	void decodeOp2(VM_INSTRUCTION_OP2* pInstructionInfoOut);
	void decodeOp1(VM_INSTRUCTION_OP1* pInstructionInfoOut);

	inline auto getOp1Flags() const {
		VM_OPCODE_OP1_FLAGS flags = {};
		flags.flags = *(m_pVmContext->getVip() + 1);
		return flags;
	}

	inline auto getOp2Flags() const {
		VM_OPCODE_OP2_FLAGS flags = {};
		flags.flags = *(m_pVmContext->getVip() + 1);
		return flags;
	}

	void* handleIndirectOp();

	inline auto getImmSigned(void* pImm, std::uint8_t immSize) {
		std::int64_t immSigned = 0;

		switch (immSize) {
		case VM_IMM_8:
			immSigned = *static_cast<std::int8_t*>(pImm);
			break;
		case VM_IMM_16:
			immSigned = *static_cast<std::int16_t*>(pImm);
			break;
		case VM_IMM_32:
			immSigned = *static_cast<std::int32_t*>(pImm);
			break;
		case VM_IMM_64:
			immSigned = *static_cast<std::int64_t*>(pImm);
			break;
		}

		return immSigned;
	}

	inline auto getRegOp1() const {
		return static_cast<VM_REG_OPERAND>(*(m_pVmContext->getVip() + 2));
	}

	inline auto getRegOp2() const {
		return static_cast<VM_REG_OPERAND>(*(m_pVmContext->getVip() + 3));
	}

	inline void* getImmOp1() const {
		return m_pVmContext->getVip() + 2;
	}

	inline void* getImmOp2() const {
		return m_pVmContext->getVip() + 3;
	}

	inline std::uint8_t getLength() const {
		const auto opCount = getOpCount();
		if (!opCount) return 1;

		std::uint8_t length = 2;

		if (opCount == 1) {
			auto flags = getOp1Flags();
			
			if (flags.isImmediateUsed) length += getImmSizeBytes(flags.immSize);
			else length += 1;

			if (flags.hasDisplacement)
				length += 8;

			if (flags.hasScaleIndex)
				length += 2;
		}
		else {
			auto flags = getOp2Flags();

			if (flags.isImmediateUsed) length += getImmSizeBytes(flags.immSize);
			else length += 2;

			if (flags.hasDisplacement)
				length += 8;

			if (flags.hasScaleIndex)
				length += 2;
		}

		return length;
	}

	inline std::uint8_t getImmSizeBytes(std::uint8_t immSizeFlag) const {
		switch (immSizeFlag) {
			case VM_IMM_8: return 1;
			case VM_IMM_16: return 2;
			case VM_IMM_32: return 4;
			case VM_IMM_64: return 8;
			default: return 0;
		}
	}
};