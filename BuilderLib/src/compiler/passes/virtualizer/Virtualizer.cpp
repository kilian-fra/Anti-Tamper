#include <iostream>
#include <vector>
#include <set>
#include <memory>
#include "Virtualizer.hpp"
#include "VMDisassembler.hpp"

VM_REG_OPERAND CVirtualizer::getVirtualRegOperand(ZydisRegister reg)
{
	switch (reg)
	{
		// 8-bit registers
	case ZYDIS_REGISTER_AL:  return VM_REG_OPERAND::REGISTER_AL;
	case ZYDIS_REGISTER_AH:  return VM_REG_OPERAND::REGISTER_AH;
	case ZYDIS_REGISTER_BL:  return VM_REG_OPERAND::REGISTER_BL;
	case ZYDIS_REGISTER_BH:  return VM_REG_OPERAND::REGISTER_BH;
	case ZYDIS_REGISTER_CL:  return VM_REG_OPERAND::REGISTER_CL;
	case ZYDIS_REGISTER_CH:  return VM_REG_OPERAND::REGISTER_CH;
	case ZYDIS_REGISTER_DL:  return VM_REG_OPERAND::REGISTER_DL;
	case ZYDIS_REGISTER_DH:  return VM_REG_OPERAND::REGISTER_DH;
	case ZYDIS_REGISTER_SPL: return VM_REG_OPERAND::REGISTER_SPL;
	case ZYDIS_REGISTER_BPL: return VM_REG_OPERAND::REGISTER_BPL;
	case ZYDIS_REGISTER_SIL: return VM_REG_OPERAND::REGISTER_SIL;
	case ZYDIS_REGISTER_DIL: return VM_REG_OPERAND::REGISTER_DIL;
	case ZYDIS_REGISTER_R8B: return VM_REG_OPERAND::REGISTER_R8B;
	case ZYDIS_REGISTER_R9B: return VM_REG_OPERAND::REGISTER_R9B;
	case ZYDIS_REGISTER_R10B: return VM_REG_OPERAND::REGISTER_R10B;
	case ZYDIS_REGISTER_R11B: return VM_REG_OPERAND::REGISTER_R11B;
	case ZYDIS_REGISTER_R12B: return VM_REG_OPERAND::REGISTER_R12B;
	case ZYDIS_REGISTER_R13B: return VM_REG_OPERAND::REGISTER_R13B;
	case ZYDIS_REGISTER_R14B: return VM_REG_OPERAND::REGISTER_R14B;
	case ZYDIS_REGISTER_R15B: return VM_REG_OPERAND::REGISTER_R15B;

		// 16-bit registers
	case ZYDIS_REGISTER_AX:  return VM_REG_OPERAND::REGISTER_AX;
	case ZYDIS_REGISTER_BX:  return VM_REG_OPERAND::REGISTER_BX;
	case ZYDIS_REGISTER_CX:  return VM_REG_OPERAND::REGISTER_CX;
	case ZYDIS_REGISTER_DX:  return VM_REG_OPERAND::REGISTER_DX;
	case ZYDIS_REGISTER_SP:  return VM_REG_OPERAND::REGISTER_SP;
	case ZYDIS_REGISTER_BP:  return VM_REG_OPERAND::REGISTER_BP;
	case ZYDIS_REGISTER_SI:  return VM_REG_OPERAND::REGISTER_SI;
	case ZYDIS_REGISTER_DI:  return VM_REG_OPERAND::REGISTER_DI;
	case ZYDIS_REGISTER_R8W: return VM_REG_OPERAND::REGISTER_R8W;
	case ZYDIS_REGISTER_R9W: return VM_REG_OPERAND::REGISTER_R9W;
	case ZYDIS_REGISTER_R10W: return VM_REG_OPERAND::REGISTER_R10W;
	case ZYDIS_REGISTER_R11W: return VM_REG_OPERAND::REGISTER_R11W;
	case ZYDIS_REGISTER_R12W: return VM_REG_OPERAND::REGISTER_R12W;
	case ZYDIS_REGISTER_R13W: return VM_REG_OPERAND::REGISTER_R13W;
	case ZYDIS_REGISTER_R14W: return VM_REG_OPERAND::REGISTER_R14W;
	case ZYDIS_REGISTER_R15W: return VM_REG_OPERAND::REGISTER_R15W;

		// 32-bit registers
	case ZYDIS_REGISTER_EAX: return VM_REG_OPERAND::REGISTER_EAX;
	case ZYDIS_REGISTER_EBX: return VM_REG_OPERAND::REGISTER_EBX;
	case ZYDIS_REGISTER_ECX: return VM_REG_OPERAND::REGISTER_ECX;
	case ZYDIS_REGISTER_EDX: return VM_REG_OPERAND::REGISTER_EDX;
	case ZYDIS_REGISTER_ESP: return VM_REG_OPERAND::REGISTER_ESP;
	case ZYDIS_REGISTER_EBP: return VM_REG_OPERAND::REGISTER_EBP;
	case ZYDIS_REGISTER_ESI: return VM_REG_OPERAND::REGISTER_ESI;
	case ZYDIS_REGISTER_EDI: return VM_REG_OPERAND::REGISTER_EDI;
	case ZYDIS_REGISTER_R8D: return VM_REG_OPERAND::REGISTER_R8D;
	case ZYDIS_REGISTER_R9D: return VM_REG_OPERAND::REGISTER_R9D;
	case ZYDIS_REGISTER_R10D: return VM_REG_OPERAND::REGISTER_R10D;
	case ZYDIS_REGISTER_R11D: return VM_REG_OPERAND::REGISTER_R11D;
	case ZYDIS_REGISTER_R12D: return VM_REG_OPERAND::REGISTER_R12D;
	case ZYDIS_REGISTER_R13D: return VM_REG_OPERAND::REGISTER_R13D;
	case ZYDIS_REGISTER_R14D: return VM_REG_OPERAND::REGISTER_R14D;
	case ZYDIS_REGISTER_R15D: return VM_REG_OPERAND::REGISTER_R15D;

		// 64-bit registers
	case ZYDIS_REGISTER_RAX: return VM_REG_OPERAND::REGISTER_RAX;
	case ZYDIS_REGISTER_RBX: return VM_REG_OPERAND::REGISTER_RBX;
	case ZYDIS_REGISTER_RCX: return VM_REG_OPERAND::REGISTER_RCX;
	case ZYDIS_REGISTER_RDX: return VM_REG_OPERAND::REGISTER_RDX;
	case ZYDIS_REGISTER_RSP: return VM_REG_OPERAND::REGISTER_RSP;
	case ZYDIS_REGISTER_RBP: return VM_REG_OPERAND::REGISTER_RBP;
	case ZYDIS_REGISTER_RSI: return VM_REG_OPERAND::REGISTER_RSI;
	case ZYDIS_REGISTER_RDI: return VM_REG_OPERAND::REGISTER_RDI;
	case ZYDIS_REGISTER_R8: return VM_REG_OPERAND::REGISTER_R8;
	case ZYDIS_REGISTER_R9: return VM_REG_OPERAND::REGISTER_R9;
	case ZYDIS_REGISTER_R10: return VM_REG_OPERAND::REGISTER_R10;
	case ZYDIS_REGISTER_R11: return VM_REG_OPERAND::REGISTER_R11;
	case ZYDIS_REGISTER_R12: return VM_REG_OPERAND::REGISTER_R12;
	case ZYDIS_REGISTER_R13: return VM_REG_OPERAND::REGISTER_R13;
	case ZYDIS_REGISTER_R14: return VM_REG_OPERAND::REGISTER_R14;
	case ZYDIS_REGISTER_R15: return VM_REG_OPERAND::REGISTER_R15;
	
	//RIP-relativ-addressing
	case ZYDIS_REGISTER_RIP: return VM_REG_OPERAND::REGISTER_RIP;

	default:
		// Handle other cases if needed
		return VM_REG_OPERAND::UNKNOWN;
	}
}

VM_OPCODE CVirtualizer::getVirtualOpcode(ZydisMnemonic mnemonic)
{
	switch (mnemonic) {
		/*
		Data movement operations
		*/
		case ZYDIS_MNEMONIC_MOV: return VM_OPCODE::MOV;
		case ZYDIS_MNEMONIC_MOVBE: return VM_OPCODE::MOVBE;
		case ZYDIS_MNEMONIC_MOVSX: return VM_OPCODE::MOVSX;
		case ZYDIS_MNEMONIC_MOVZX: return VM_OPCODE::MOVZX;
		case ZYDIS_MNEMONIC_MOVD: return VM_OPCODE::MOVD;
		case ZYDIS_MNEMONIC_MOVNTI: return VM_OPCODE::MOVNTI;
		case ZYDIS_MNEMONIC_LEA: return VM_OPCODE::LEA;

		/*
		Stack operations
		*/
		case ZYDIS_MNEMONIC_POP: return VM_OPCODE::POP;
		case ZYDIS_MNEMONIC_POPA: return VM_OPCODE::POPA;
		case ZYDIS_MNEMONIC_POPAD: return VM_OPCODE::POPAD;
		case ZYDIS_MNEMONIC_PUSH: return VM_OPCODE::PUSH;
		case ZYDIS_MNEMONIC_PUSHA: return VM_OPCODE::PUSHA;
		case ZYDIS_MNEMONIC_PUSHAD: return VM_OPCODE::PUSHAD;
		case ZYDIS_MNEMONIC_POPF: return VM_OPCODE::POPF;
		case ZYDIS_MNEMONIC_POPFD: return VM_OPCODE::POPFD;
		case ZYDIS_MNEMONIC_POPFQ: return VM_OPCODE::POPFQ;
		case ZYDIS_MNEMONIC_PUSHF: return VM_OPCODE::PUSHF;
		case ZYDIS_MNEMONIC_PUSHFD: return VM_OPCODE::PUSHFD;
		case ZYDIS_MNEMONIC_PUSHFQ: return VM_OPCODE::PUSHFQ;

		/*
		General Arithmetic
		*/
		case ZYDIS_MNEMONIC_ADC: return VM_OPCODE::ADC;
		case ZYDIS_MNEMONIC_ADD: return VM_OPCODE::ADD;
		case ZYDIS_MNEMONIC_SBB: return VM_OPCODE::SBB;
		case ZYDIS_MNEMONIC_SUB: return VM_OPCODE::SUB;
		case ZYDIS_MNEMONIC_NEG: return VM_OPCODE::NEG;
		case ZYDIS_MNEMONIC_MUL: return VM_OPCODE::MUL;
		case ZYDIS_MNEMONIC_IMUL: return VM_OPCODE::IMUL;
		case ZYDIS_MNEMONIC_DIV: return VM_OPCODE::DIV;
		case ZYDIS_MNEMONIC_IDIV: return VM_OPCODE::IDIV;
		case ZYDIS_MNEMONIC_INC: return VM_OPCODE::INC;
		case ZYDIS_MNEMONIC_DEC: return VM_OPCODE::DEC;

		/*
		Rotate and Shift
		*/
		case ZYDIS_MNEMONIC_RCL: return VM_OPCODE::RCL;
		case ZYDIS_MNEMONIC_RCR: return VM_OPCODE::RCR;
		case ZYDIS_MNEMONIC_ROL: return VM_OPCODE::ROL;
		case ZYDIS_MNEMONIC_ROR: return VM_OPCODE::ROR;
		case ZYDIS_MNEMONIC_SALC: return VM_OPCODE::SALC;
		case ZYDIS_MNEMONIC_SAR: return VM_OPCODE::SAR;
		case ZYDIS_MNEMONIC_SHL: return VM_OPCODE::SHL;
		case ZYDIS_MNEMONIC_SHLD: return VM_OPCODE::SHLD;
		case ZYDIS_MNEMONIC_SHRD: return VM_OPCODE::SHRD;
		case ZYDIS_MNEMONIC_SHR: return VM_OPCODE::SHR;

		/*
		Logical Operations
		*/
		case ZYDIS_MNEMONIC_AND: return VM_OPCODE::AND;
		case ZYDIS_MNEMONIC_OR: return VM_OPCODE::OR;
		case ZYDIS_MNEMONIC_NOT: return VM_OPCODE::NOT;
		case ZYDIS_MNEMONIC_XOR: return VM_OPCODE::XOR;

		/*
		Controlflow
		*/
		case ZYDIS_MNEMONIC_JMP: return VM_OPCODE::JMP;
		case ZYDIS_MNEMONIC_CALL: return VM_OPCODE::CALL;
		case ZYDIS_MNEMONIC_RET: return VM_OPCODE::RET;

		/*
		Condtional Jumps
		*/
		case ZYDIS_MNEMONIC_JO: return VM_OPCODE::JO;
		case ZYDIS_MNEMONIC_JNO: return VM_OPCODE::JNO;
		case ZYDIS_MNEMONIC_JB: return VM_OPCODE::JB;
		case ZYDIS_MNEMONIC_JNB: return VM_OPCODE::JNB;
		case ZYDIS_MNEMONIC_JZ: return VM_OPCODE::JZ;
		case ZYDIS_MNEMONIC_JNZ: return VM_OPCODE::JNZ;
		case ZYDIS_MNEMONIC_JBE: return VM_OPCODE::JBE;
		case ZYDIS_MNEMONIC_JNBE: return VM_OPCODE::JNBE;
		case ZYDIS_MNEMONIC_JS: return VM_OPCODE::JS;
		case ZYDIS_MNEMONIC_JNS: return VM_OPCODE::JNS;
		case ZYDIS_MNEMONIC_JP: return VM_OPCODE::JP;
		case ZYDIS_MNEMONIC_JNP: return VM_OPCODE::JNP;
		case ZYDIS_MNEMONIC_JL: return VM_OPCODE::JL;
		case ZYDIS_MNEMONIC_JNL: return VM_OPCODE::JNL;
		case ZYDIS_MNEMONIC_JLE: return VM_OPCODE::JLE;
		case ZYDIS_MNEMONIC_JNLE: return VM_OPCODE::JNLE;

		/*
		Compare and Test
		*/
		case ZYDIS_MNEMONIC_CMP: return VM_OPCODE::CMP;
		case ZYDIS_MNEMONIC_TEST: return VM_OPCODE::TEST;
		case ZYDIS_MNEMONIC_BT: return VM_OPCODE::BT;
		case ZYDIS_MNEMONIC_BTC: return VM_OPCODE::BTC;
		case ZYDIS_MNEMONIC_BTR: return VM_OPCODE::BTR;
		case ZYDIS_MNEMONIC_BTS: return VM_OPCODE::BTS;


		/*
		* Handle native instructions that are not supported by the VM
		* Those instructions need to be executed under native context and the VM_CONTEXT needs to be updated afterwards.
		* This can be done by either executing the instruction in a extra handler or by a VM_EXIT
		*/
		default:
			return VM_OPCODE::UNSUPPORTED;
	}
}

int64_t CVirtualizer::handleDispRipRelative(int64_t disp, ZyanU8 instructionLength)
{
	const auto ripBaseDiff = m_realRip - m_pBaseFunc;
	return disp > 0 ? static_cast<int64_t>(disp + ripBaseDiff + instructionLength)
		: static_cast<int64_t>(disp - (ripBaseDiff + instructionLength));
}

void CVirtualizer::fixImmRipRelativeInstructions(const std::vector<std::size_t>& vmInstructionOffsets,
	const std::vector<REALLOC_IMM_RIP_REL>& nativeInstructions)
{
	//Index i maps the vm instruction to a native instruction and vice versa

	for (const auto i : m_ImmRealloc) {

		const auto& nativeInstruction = nativeInstructions.at(i);
		auto vmInstructionPtr = m_Bytecode.data() + vmInstructionOffsets.at(i);

		ZyanU64 targetRip = 0;
		if (!ZYAN_SUCCESS(ZydisCalcAbsoluteAddress(&nativeInstruction.instruction, &nativeInstruction.op1, 
			(ZyanU64)nativeInstruction.rip, &targetRip)))
			continue;

		////find target instruction by targetRip
		//auto it = std::find(nativeInstructions.begin(), 
		//	nativeInstructions.end(), [targetRip](REALLOC_IMM_RIP_REL& x) {targetRip == (ZyanU64)x.rip; });
		//if (it == nativeInstructions.end()) continue;

		std::size_t targetIndex = 0;
		for (const auto& x : nativeInstructions)
		{
			if ((ZyanU64)x.rip == targetRip)
				break;

			++targetIndex;
		}

	/*	const std::size_t targetIndex = std::distance(nativeInstructions.begin(), it);*/

		const auto vmInstructionTarget = m_Bytecode.data() + vmInstructionOffsets.at(targetIndex);
		VM_OPCODE_OP1_FLAGS flags = {};
		flags.flags = *(vmInstructionPtr + 1);

		if (flags.isImmSigned) {
			const auto realDiff = (std::int64_t)(vmInstructionTarget - vmInstructionPtr);
			switch (flags.immSize) {
				case VM_IMM_8:
					*reinterpret_cast<std::int8_t*>(vmInstructionPtr + 2) = static_cast<std::int8_t>(realDiff);
					break;
				case VM_IMM_16:
					*reinterpret_cast<std::int16_t*>(vmInstructionPtr + 2) = static_cast<std::int16_t>(realDiff);
					break;
				case VM_IMM_32:
					*reinterpret_cast<std::int32_t*>(vmInstructionPtr + 2) = static_cast<std::int32_t>(realDiff);
					break;
				case VM_IMM_64:
					*reinterpret_cast<std::int64_t*>(vmInstructionPtr + 2) = realDiff;
					break;
			}
		}
		else {
			const auto realDiff = (std::uint64_t)(vmInstructionTarget - vmInstructionPtr);
			switch (flags.immSize) {
				case VM_IMM_8:
					*reinterpret_cast<std::uint8_t*>(vmInstructionPtr + 2) = static_cast<std::uint8_t>(realDiff);
					break;
				case VM_IMM_16:
					*reinterpret_cast<std::uint16_t*>(vmInstructionPtr + 2) = static_cast<std::uint16_t>(realDiff);
					break;
				case VM_IMM_32:
					*reinterpret_cast<std::uint32_t*>(vmInstructionPtr + 2) = static_cast<std::uint32_t>(realDiff);
					break;
				case VM_IMM_64:
					*reinterpret_cast<std::uint64_t*>(vmInstructionPtr + 2) = realDiff;
					break;
			}
		}
	}
}

std::uint8_t CVirtualizer::getExplicitOperandCount(const ZydisDecodedOperand* operands)
{
	std::uint8_t count = 0;
	for (auto i = 0U; i < ZYDIS_MAX_OPERAND_COUNT; ++i)
		if (operands[i].visibility == ZYDIS_OPERAND_VISIBILITY_EXPLICIT) ++count;

	return count;
}

std::uint8_t CVirtualizer::getImmSizeFlag(ZyanU16 immSize) {
	switch (immSize) {
		case 8: return VM_IMM_8;
		case 16: return VM_IMM_16;
		case 32: return VM_IMM_32;
		case 64: return VM_IMM_64;
		default: return VM_IMM_64;
	}
};

void CVirtualizer::handleNativeInstruction(VM_OPCODE vm_opcode, const ZydisDecodedOperand& op1, 
	const ZydisDecodedOperand& op2, ZyanU8 instructionLength)
{
	
	//auto handleMemOperand = [](const ZydisDecodedOperand& opMem, const ZydisDeocdedOperand& )

	// Handle the case when both operands are registers
	if (op1.type == ZYDIS_OPERAND_TYPE_REGISTER && op2.type == ZYDIS_OPERAND_TYPE_REGISTER)
	{
		addVmInstruction(VMInstruction(vm_opcode, {}, getVirtualRegOperand(op1.reg.value), getVirtualRegOperand(op1.reg.value)));
	}
	// Handle the case when the first operand is a register and the second is memory
	else if (op1.type == ZYDIS_OPERAND_TYPE_REGISTER && op2.type == ZYDIS_OPERAND_TYPE_MEMORY)
	{
		VM_OPCODE_OP2_FLAGS flags = {};
		flags.isIndirectOp2 = 1;
		//mov reg, [base + index * scale + displacement]

		//Example: mov rax, [rbx + rdi*8]
		if (op2.mem.index != ZYDIS_REGISTER_NONE && op2.mem.scale != 0)
			flags.hasScaleIndex = 1;

		std::int64_t dispValue = (op2.mem.base == ZYDIS_REGISTER_RIP) ?
			handleDispRipRelative(op2.mem.disp.value, instructionLength) : op2.mem.disp.value;

		//Example: mov rax, [rbx + 16]
		if (op2.mem.disp.has_displacement && dispValue)
			flags.hasDisplacement = 1;

		auto instruction = VMInstruction(vm_opcode, flags, 
			getVirtualRegOperand(op1.reg.value), getVirtualRegOperand(op2.mem.base));

		instruction.handleDispScaleIndex(flags.hasDisplacement, flags.hasScaleIndex,
			dispValue, op2.mem.scale, getVirtualRegOperand(op2.mem.index));

		addVmInstruction(instruction);
	}
	// Handle the case when the first operand is memory and the second is a register
	else if (op1.type == ZYDIS_OPERAND_TYPE_MEMORY && op2.type == ZYDIS_OPERAND_TYPE_REGISTER)
	{
		VM_OPCODE_OP2_FLAGS flags = {};
		flags.isIndirectOp1 = 1;
		//mov reg, [base + index * scale + displacement]

		//Example: mov rax, [rbx + rdi*8]
		if (op1.mem.index != ZYDIS_REGISTER_NONE && op1.mem.scale != 0)
			flags.hasScaleIndex = 1;

		std::int64_t dispValue = (op1.mem.base == ZYDIS_REGISTER_RIP) ?
			handleDispRipRelative(op1.mem.disp.value, instructionLength) : op1.mem.disp.value;

		//Example: mov rax, [rbx + 16]
		if (op1.mem.disp.has_displacement && dispValue)
			flags.hasDisplacement = 1;

		auto instruction = VMInstruction(vm_opcode, flags,
			getVirtualRegOperand(op1.mem.base), getVirtualRegOperand(op2.reg.value));

		instruction.handleDispScaleIndex(flags.hasDisplacement, flags.hasScaleIndex,
			dispValue, op1.mem.scale, getVirtualRegOperand(op1.mem.index));

		addVmInstruction(instruction);
	}
	// Handle the case when the first operand is a register and the second is an immediate
	else if (op1.type == ZYDIS_OPERAND_TYPE_REGISTER && op2.type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
	{
		VM_OPCODE_OP2_FLAGS flags = {};
		flags.immSize = getImmSizeFlag(op2.size);
		flags.isImmediateUsed = 1;

		if (op2.imm.is_signed) 
			addVmInstruction(VMInstruction(vm_opcode, flags, getVirtualRegOperand(op1.reg.value), op2.imm.value.s));
		else
			addVmInstruction(VMInstruction(vm_opcode, flags, getVirtualRegOperand(op1.reg.value), op2.imm.value.u));

		fixImmSizeOp2(flags.flags);
	}
	// Handle the case when the first operand is memory and the second is an immediate
	else if (op1.type == ZYDIS_OPERAND_TYPE_MEMORY && op2.type == ZYDIS_OPERAND_TYPE_IMMEDIATE)
	{
		VM_OPCODE_OP2_FLAGS flags = {};
		flags.isIndirectOp1 = 1;
		flags.immSize = getImmSizeFlag(op2.size);
		flags.isImmSigned = op1.imm.is_signed ? 1 : 0;
		flags.isImmediateUsed = 1;

		if (op1.mem.index != ZYDIS_REGISTER_NONE && op1.mem.scale != 0)
			flags.hasScaleIndex = 1;

		std::int64_t dispValue = (op1.mem.base == ZYDIS_REGISTER_RIP) ?
			handleDispRipRelative(op1.mem.disp.value, instructionLength) : op1.mem.disp.value;

		if (op1.mem.disp.has_displacement && dispValue)
			flags.hasDisplacement = 1;

		std::unique_ptr<VMInstruction> pInstruction;

		if (op2.imm.is_signed) {
			pInstruction = std::make_unique<VMInstruction>(vm_opcode, flags,
				getVirtualRegOperand(op1.mem.base), op2.imm.value.s);
		}
		else {
			pInstruction = std::make_unique<VMInstruction>(vm_opcode, flags,
				getVirtualRegOperand(op1.mem.base), op2.imm.value.u);
		}

		pInstruction->handleDispScaleIndex(flags.hasDisplacement, flags.hasScaleIndex,
			dispValue, op1.mem.scale, getVirtualRegOperand(op1.mem.index));

		addVmInstruction(*pInstruction);
		fixImmSizeOp2(flags.flags);
	}
	else
	{
		std::cout << "[-] Instruction with two opcodes could not be handled\n";
		return;
	}
}

/*
Handle instructions without any operands (for example: ret, int3, ...)
*/
void CVirtualizer::handleNativeInstruction(VM_OPCODE vm_opcode)
{
	addVmInstruction(VMInstruction(vm_opcode));
}

void CVirtualizer::handleNativeInstruction(VM_OPCODE vm_opcode, const ZydisDecodedOperand& op1, ZyanU8 instructionLength)
{
	VM_OPCODE_OP1_FLAGS flags = {};

	switch (op1.type) {
		
		case ZYDIS_OPERAND_TYPE_REGISTER:
			// Handle single operand register instructions
			// Example: push rax, pop rcx, inc rbx, dec rdi, etc.

			addVmInstruction(VMInstruction(vm_opcode, flags, getVirtualRegOperand(op1.reg.value)));
			break;

		case ZYDIS_OPERAND_TYPE_IMMEDIATE:

			flags.isImmediateUsed = 1;
			flags.immSize = getImmSizeFlag(op1.size);
			flags.isImmRipRelative = op1.imm.is_relative ? 1 : 0;
			flags.isImmSigned = op1.imm.is_signed ? 1 : 0;

			if (op1.imm.is_signed) {
				auto imm = flags.isImmRipRelative
					? handleImmRipRelative(op1.imm.value.s, instructionLength)
					: op1.imm.value.s;
				addVmInstruction(VMInstruction(vm_opcode, flags, imm));
			}
			else {
				auto imm = flags.isImmRipRelative
					? handleImmRipRelative(op1.imm.value.u, instructionLength)
					: op1.imm.value.u;
				addVmInstruction(VMInstruction(vm_opcode, flags, imm));
			}

			fixImmSizeOp1(flags.flags);
			break;

		case ZYDIS_OPERAND_TYPE_MEMORY: {
			// Handle the case when the memory operand has base and displacement
			// For example: not byte ptr [rax + 0x10]

			flags.isIndirectOp1 = 1;
			flags.hasScaleIndex = (op1.mem.index != ZYDIS_REGISTER_NONE && op1.mem.scale != 0) ? 1 : 0;

			std::int64_t dispValue = (op1.mem.base == ZYDIS_REGISTER_RIP) ?
				handleDispRipRelative(op1.mem.disp.value, instructionLength) : op1.mem.disp.value;

			flags.hasDisplacement = op1.mem.disp.has_displacement && dispValue ? 1 : 0;

			auto instruction = VMInstruction(vm_opcode, flags, getVirtualRegOperand(op1.mem.base));
			instruction.handleDispScaleIndex(flags.hasDisplacement, flags.hasScaleIndex,
				dispValue, op1.mem.scale, getVirtualRegOperand(op1.mem.index));
			
			addVmInstruction(instruction);
			break;
		}

		default:
			std::cout << "[-] Instruction with one opcode could not be handled\n";
			break;
	}
}

CVirtualizer::CVirtualizer(CFunction* function, std::uint8_t* pBaseFunc, std::uint32_t funcSize)
	: m_TargetFunction(function), m_Bytecode(BYTECODE_EXTEND), m_BytecodeSize(0), m_pBaseFunc(pBaseFunc), m_realRip(nullptr),
	  m_currInstructionIndex(0), m_funcSize(funcSize)
{
	std::printf("Bytecode: %p\n", m_Bytecode.data());
}

/*
Transform instructions of the function according to a custom generated instruction set.
Each native instruction is converted to virtual instruction(s).
*/
void CVirtualizer::transform()
{
	/*
	Obtain disassembly for our target function.
	Disassembly is a vector that contains all disassembled instructions in the form of INSTRUCTION struct elements
	*/
	const auto disassembly = m_TargetFunction->getDisassembly();

	/*
	* Iterate over all native assembly instructions and handle/transform them
	*/
	
	std::set<ZydisMnemonic> unsupportedInst;

	//Needed for relative-RIP imm. reallocation
	std::vector<std::size_t> vmInstructionOffsets;
	std::vector<REALLOC_IMM_RIP_REL> nativeInstructions;

	//Initialize real RIP with base address of target function
	m_realRip = m_pBaseFunc;

	for (const auto&[instruction, 
			operands] : disassembly) {

		const auto vm_opcode = getVirtualOpcode(instruction.mnemonic);
		if (vm_opcode == VM_OPCODE::UNSUPPORTED) {
			unsupportedInst.insert(instruction.mnemonic);
			continue;
		}

		const auto explicitOpCount = getExplicitOperandCount(operands);
		const auto currBytecodeOffset = m_BytecodeSize;

		if (explicitOpCount == 0) {
			handleNativeInstruction(vm_opcode);
		}
		else if (explicitOpCount == 1) {
			handleNativeInstruction(vm_opcode, operands[0], instruction.length);
		}
		else if (explicitOpCount == 2) {
			handleNativeInstruction(vm_opcode, operands[0], operands[1], instruction.length);
		}
		else {
			std::cout << "[-] Instructions with more than two operands are not supported: " << ZydisMnemonicGetString(instruction.mnemonic) << "\n";
			break;
		}

		//Store current vm instruction's vip
		vmInstructionOffsets.push_back(currBytecodeOffset); //Store offset to make it work with vector reallocation
		nativeInstructions.push_back({ instruction, operands[0], m_realRip });
		++m_currInstructionIndex;

		//Increment real RIP
		m_realRip += instruction.length;
	}

	//Fix vm instructions that use RIP-relative addressing with imm. values (in range of the bytecode)
	// RIP-relative-addressed data or functions outside of the bytecode are handled by making them relative to the functions base (ptr) instead
	fixImmRipRelativeInstructions(vmInstructionOffsets, nativeInstructions);

	for (const auto& mnemonic : unsupportedInst)
		std::cout << "[-] Instruction not supported: " << ZydisMnemonicGetString(mnemonic) << "\n";

	auto dism = VMDisassembler(m_Bytecode.data(), m_BytecodeSize);
	dism.disassemble();

	std::cout << dism.getDisassembly() << "\n";
}