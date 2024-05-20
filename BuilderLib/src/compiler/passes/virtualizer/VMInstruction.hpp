#pragma once

#include <iostream>
#include <vector>
#include "VM.hpp"

class VMInstruction
{
	std::vector<std::uint8_t> m_Bytecode;
	std::size_t m_BytecodeSize;

public:

	VMInstruction(VM_OPCODE vm_opcode) : m_Bytecode({ (std::uint8_t)vm_opcode }), m_BytecodeSize(1) {}

	VMInstruction(VM_OPCODE vm_opcode, VM_OPCODE_OP1_FLAGS flags, VM_REG_OPERAND op1) 
		: m_Bytecode({(std::uint8_t)vm_opcode, flags.flags, (std::uint8_t)op1}), m_BytecodeSize(3) {}

	VMInstruction(VM_OPCODE vm_opcode, VM_OPCODE_OP2_FLAGS flags, VM_REG_OPERAND op1, VM_REG_OPERAND op2)
		: m_Bytecode({ (std::uint8_t)vm_opcode, flags.flags, (std::uint8_t)op1, (std::uint8_t)op2 }), m_BytecodeSize(4) {}

	template <typename T>
	VMInstruction(VM_OPCODE vm_opcode, VM_OPCODE_OP2_FLAGS flags, VM_REG_OPERAND op1, T op2Imm) 
		: m_Bytecode({ (std::uint8_t)vm_opcode, flags.flags, (std::uint8_t)op1 }), m_BytecodeSize(3)
	{
		m_Bytecode.resize(m_BytecodeSize + sizeof(T));
		std::memcpy(m_Bytecode.data() + m_BytecodeSize, &op2Imm, sizeof(T));
		m_BytecodeSize += sizeof(T);
	}

	template <typename T>
	VMInstruction(VM_OPCODE vm_opcode, VM_OPCODE_OP1_FLAGS flags, T op1Imm) 
		: m_Bytecode({ (std::uint8_t)vm_opcode, flags.flags }), m_BytecodeSize(2)
	{
		m_Bytecode.resize(m_BytecodeSize + sizeof(T));
		std::memcpy(m_Bytecode.data() + m_BytecodeSize, &op1Imm, sizeof(T));
		m_BytecodeSize += sizeof(T);
	}

	void addDisplacement(std::int64_t dispValue) {
		if (!dispValue) return;
		m_Bytecode.resize(m_BytecodeSize + sizeof(std::int64_t));
		std::memcpy(m_Bytecode.data() + m_BytecodeSize, &dispValue, sizeof(std::int64_t));
		m_BytecodeSize += sizeof(std::int64_t);
	}

	void addScaleIndex(std::uint8_t scale, VM_REG_OPERAND index) {
		m_Bytecode.push_back(scale);
		m_Bytecode.push_back((std::uint8_t)index);
		m_BytecodeSize += 2;
	}

	void handleDispScaleIndex(std::uint8_t hasDisplacement, std::uint8_t hasScaleIndex, 
		std::int64_t disp, std::uint8_t scale, VM_REG_OPERAND index) {
		
		if (hasDisplacement && hasScaleIndex) //disp and scale + index
		{
			addDisplacement(disp);
			addScaleIndex(scale, index);
		}
		else if (hasDisplacement && !hasScaleIndex) //Disp only
		{
			addDisplacement(disp);
		}
		else if (!hasDisplacement && hasScaleIndex) //scale + index
		{
			addScaleIndex(scale, index);
		}
	}

	auto getBytes() const { return m_Bytecode; }
	auto getSize() const { return m_BytecodeSize;  }
};