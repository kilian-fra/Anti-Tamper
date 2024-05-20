#include <iostream>
#include <functional>
#include "VMHandler.hpp"
#include "VMInstruction.hpp"
#include "VMStack.hpp"

bool VMHandler::isVipInBytecode(VMContext* pVmContext, std::uint8_t* vip)
{
    return !(vip > (pVmContext->getBytecodePtr() + pVmContext->getBytecodeSize())
        || vip > (pVmContext->getBytecodePtr() + pVmContext->getBytecodeSize()));
}

void VMHandler::performOperationUnsigned(std::uint64_t* opSrc, std::uint64_t* opDst, std::uint8_t srcSize,
    std::uint8_t dstSize, std::function<std::uint64_t(std::uint64_t, std::uint64_t)> operation)
{
    switch (dstSize) {
    case 1: {
        switch (srcSize) {
        case 1: *(std::uint8_t*)opDst = (std::uint8_t)operation(*(std::uint8_t*)opDst, *(std::uint8_t*)opSrc); break;
        case 2: *(std::uint8_t*)opDst = (std::uint8_t)operation(*(std::uint8_t*)opDst, *(std::uint16_t*)opSrc); break;
        case 4: *(std::uint8_t*)opDst = (std::uint8_t)operation(*(std::uint8_t*)opDst, *(std::uint32_t*)opSrc); break;
        case 8: *(std::uint8_t*)opDst = (std::uint8_t)operation(*(std::uint8_t*)opDst, *opSrc); break;
        }
        break;
    }
    case 2: {
        switch (srcSize) {
        case 1: *(std::uint16_t*)opDst = (std::uint16_t)operation(*(std::uint16_t*)opDst, *(std::uint8_t*)opSrc); break;
        case 2: *(std::uint16_t*)opDst = (std::uint16_t)operation(*(std::uint16_t*)opDst, *(std::uint16_t*)opSrc); break;
        case 4: *(std::uint16_t*)opDst = (std::uint16_t)operation(*(std::uint16_t*)opDst, *(std::uint32_t*)opSrc); break;
        case 8: *(std::uint16_t*)opDst = (std::uint16_t)operation(*(std::uint16_t*)opDst, *opSrc); break;
        }
        break;
    }
    case 4: {
        switch (srcSize) {
        case 1: *(std::uint32_t*)opDst = (std::uint32_t)operation(*(std::uint32_t*)opDst, *(std::uint8_t*)opSrc); break;
        case 2: *(std::uint32_t*)opDst = (std::uint32_t)operation(*(std::uint32_t*)opDst, *(std::uint16_t*)opSrc); break;
        case 4: *(std::uint32_t*)opDst = (std::uint32_t)operation(*(std::uint32_t*)opDst, *(std::uint32_t*)opSrc); break;
        case 8: *(std::uint32_t*)opDst = (std::uint32_t)operation(*(std::uint32_t*)opDst, *opSrc); break;
        }
        break;
    }
    case 8: {
        switch (srcSize) {
        case 1: *opDst = operation(*opDst, *(std::uint8_t*)opSrc); break;
        case 2: *opDst = operation(*opDst, *(std::uint16_t*)opSrc); break;
        case 4: *opDst = operation(*opDst, *(std::uint32_t*)opSrc); break;
        case 8: *opDst = operation(*opDst, *opSrc); break;
        }
        break;
    }
    }
}

void VMHandler::performOperationSigned(std::int64_t* opSrc, std::int64_t* opDst, std::uint8_t srcSize, std::uint8_t dstSize, std::function<std::int64_t(std::int64_t, std::int64_t)> operation)
{
    switch (dstSize) {
    case 1: {
        switch (srcSize) {
        case 1: *(std::int8_t*)opDst = (std::int8_t)operation(*(std::int8_t*)opDst, *(std::int8_t*)opSrc); break;
        case 2: *(std::int8_t*)opDst = (std::int8_t)operation(*(std::int8_t*)opDst, *(std::int16_t*)opSrc); break;
        case 4: *(std::int8_t*)opDst = (std::int8_t)operation(*(std::int8_t*)opDst, *(std::int32_t*)opSrc); break;
        case 8: *(std::int8_t*)opDst = (std::int8_t)operation(*(std::int8_t*)opDst, *opSrc); break;
        }
        break;
    }
    case 2: {
        switch (srcSize) {
        case 1: *(std::int16_t*)opDst = (std::int16_t)operation(*(std::int16_t*)opDst, *(std::int8_t*)opSrc); break;
        case 2: *(std::int16_t*)opDst = (std::int16_t)operation(*(std::int16_t*)opDst, *(std::int16_t*)opSrc); break;
        case 4: *(std::int16_t*)opDst = (std::int16_t)operation(*(std::int16_t*)opDst, *(std::int32_t*)opSrc); break;
        case 8: *(std::int16_t*)opDst = (std::int16_t)operation(*(std::int16_t*)opDst, *opSrc); break;
        }
        break;
    }
    case 4: {
        switch (srcSize) {
        case 1: *(std::int32_t*)opDst = (std::int32_t)operation(*(std::int32_t*)opDst, *(std::int8_t*)opSrc); break;
        case 2: *(std::int32_t*)opDst = (std::int32_t)operation(*(std::int32_t*)opDst, *(std::int16_t*)opSrc); break;
        case 4: *(std::int32_t*)opDst = (std::int32_t)operation(*(std::int32_t*)opDst, *(std::int32_t*)opSrc); break;
        case 8: *(std::int32_t*)opDst = (std::int32_t)operation(*(std::int32_t*)opDst, *opSrc); break;
        }
        break;
    }
    case 8: {
        switch (srcSize) {
        case 1: *opDst = operation(*opDst, *(std::int8_t*)opSrc); break;
        case 2: *opDst = operation(*opDst, *(std::int16_t*)opSrc); break;
        case 4: *opDst = operation(*opDst, *(std::int32_t*)opSrc); break;
        case 8: *opDst = operation(*opDst, *opSrc); break;
        }
        break;
    }
    }
}

void VMHandler::mov(VMContext* pVmContext)
{
	//Fetch & decode
	auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::MOV);
	VM_INSTRUCTION_OP2 instructionData = {};
	vmInstruction.decodeOp2(&instructionData);

	//Execute
	std::memcpy(instructionData.pOp1, instructionData.pOp2, instructionData.op2SizeBytes);
    pVmContext->incVip(static_cast<std::uint64_t>(vmInstruction.getLength()));
}

// lea rax, [base + disp + scale * index]
// lea eax, [base + disp + scale * index]
void VMHandler::lea(VMContext* pVmContext)
{
	auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::MOV);
	VM_INSTRUCTION_OP2 instructionData = {};
	vmInstruction.decodeOp2(&instructionData);

	std::memcpy(instructionData.pOp1, &instructionData.pOp2, instructionData.op2SizeBytes);
    pVmContext->incVip(static_cast<std::uint64_t>(vmInstruction.getLength()));
}

void VMHandler::jcc(VMContext* pVmContext)
{
	auto vmOpcode = static_cast<VM_OPCODE>(*pVmContext->getVip());
	bool bTakeJump = false;
	auto vmInstruction = VMInstruction(pVmContext, vmOpcode);

    switch (vmOpcode) {
    case VM_OPCODE::JO:
        bTakeJump = pVmContext->m_Flags.overflow; // OF (Overflow Flag)
        break;
    case VM_OPCODE::JNO:
        bTakeJump = !pVmContext->m_Flags.overflow; // Not OF (Overflow Flag)
        break;
    case VM_OPCODE::JB: // JNAE, JC
        bTakeJump = pVmContext->m_Flags.carry; // CF (Carry Flag)
        break;
    case VM_OPCODE::JNB: // JAE, JNC
        bTakeJump = !pVmContext->m_Flags.carry; // Not CF (Carry Flag)
        break;
    case VM_OPCODE::JZ: // JE
        bTakeJump = pVmContext->m_Flags.zero; // ZF (Zero Flag)
        break;
    case VM_OPCODE::JNZ: // JNE
        bTakeJump = !pVmContext->m_Flags.zero; // Not ZF (Zero Flag)
        break;
    case VM_OPCODE::JBE: // JNA
        bTakeJump = pVmContext->m_Flags.carry || pVmContext->m_Flags.zero; // CF or ZF
        break;
    case VM_OPCODE::JNBE: // JA
        bTakeJump = !pVmContext->m_Flags.carry && !pVmContext->m_Flags.zero; // Not CF and Not ZF
        break;
    case VM_OPCODE::JS:
        bTakeJump = pVmContext->m_Flags.sign; // SF (Sign Flag)
        break;
    case VM_OPCODE::JNS:
        bTakeJump = !pVmContext->m_Flags.sign; // Not SF (Sign Flag)
        break;
    case VM_OPCODE::JP: // JPE
        bTakeJump = pVmContext->m_Flags.parity; // PF (Parity Flag)
        break;
    case VM_OPCODE::JNP: // JPO
        bTakeJump = !pVmContext->m_Flags.parity; // Not PF (Parity Flag)
        break;
    case VM_OPCODE::JL: // JNGE
        bTakeJump = pVmContext->m_Flags.sign != pVmContext->m_Flags.overflow; // SF xor OF
        break;
    case VM_OPCODE::JNL: // JGE
        bTakeJump = pVmContext->m_Flags.sign == pVmContext->m_Flags.overflow; // SF equals OF
        break;
    case VM_OPCODE::JLE: // JNG
        bTakeJump = pVmContext->m_Flags.zero || (pVmContext->m_Flags.sign != pVmContext->m_Flags.overflow); // ZF or (SF xor OF)
        break;
    case VM_OPCODE::JNLE: // JG
        bTakeJump = !pVmContext->m_Flags.zero && (pVmContext->m_Flags.sign == pVmContext->m_Flags.overflow); // Not ZF and (SF equals OF)
        break;
        // Handle other jump conditions if needed

    default:
        // Handle unknown jump conditions
        break;
    }

    VM_INSTRUCTION_OP1 instructionData = {};
    vmInstruction.decodeOp1(&instructionData);
    const auto flags = vmInstruction.getOp1Flags();

    if (bTakeJump) {
        auto pImm = instructionData.pOp1;
        std::int64_t immSigned = vmInstruction.getImmSigned(pImm, flags.immSize);

        //Check if vip would lead to vm_exit (location outisde of vm code)
        auto newVip = pVmContext->getVip() + immSigned;

        if (!isVipInBytecode(pVmContext, newVip)) {
            //Maybe check if the target is also virtualized (no VM_EXIT needed)

            //Trigger VM_EXIT
            exit(pVmContext);
        }

        //Update VIP
        pVmContext->incVip(immSigned);
    }

    pVmContext->incVip(static_cast<std::uint64_t>(vmInstruction.getLength()));
}

void VMHandler::jmp(VMContext* pVmContext)
{
    auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::JMP);
    VM_INSTRUCTION_OP1 instructionData = {};
    vmInstruction.decodeOp1(&instructionData);
    const auto flags = vmInstruction.getOp1Flags();

    if (!flags.isImmediateUsed && !flags.isIndirectOp1) { //Direct jmp -> jmp rax
        auto target = *reinterpret_cast<std::uint64_t*>(instructionData.pOp1) + vmInstruction.getLength();
        pVmContext->setVip((std::uint8_t*)target);
    }
    else if (!flags.isImmediateUsed && flags.isIndirectOp1) { //Indirect jmp -> jmp [rax]
        pVmContext->setVip((std::uint8_t*)(*reinterpret_cast<std::uint64_t*>(instructionData.pOp1)));
    }
    else { //Imm. jump (absolute or relative)
        std::int64_t immSigned = vmInstruction.getImmSigned(instructionData.pOp1, flags.immSize);
        auto newVip = pVmContext->getVip() + immSigned;
    }
}

void VMHandler::AND(VMContext* pVmContext)
{
    auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::AND);
    VM_INSTRUCTION_OP2 instructionData = {};
    vmInstruction.decodeOp2(&instructionData);

    switch (instructionData.op1SizeBytes) {
        case 1:
            *static_cast<std::uint8_t*>(instructionData.pOp1) &= *static_cast<std::uint8_t*>(instructionData.pOp2);
            break;
        case 2:
            *static_cast<std::uint16_t*>(instructionData.pOp1) &= *static_cast<std::uint16_t*>(instructionData.pOp2);
            break;
        case 4:
            *static_cast<std::uint32_t*>(instructionData.pOp1) &= *static_cast<std::uint32_t*>(instructionData.pOp2);
            break;
        case 8:
            *static_cast<std::uint64_t*>(instructionData.pOp1) &= *static_cast<std::uint64_t*>(instructionData.pOp2);
            break;
    }
}

void VMHandler::OR(VMContext* pVmContext)
{
    auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::AND);
    VM_INSTRUCTION_OP2 instructionData = {};
    vmInstruction.decodeOp2(&instructionData);

    switch (instructionData.op1SizeBytes) {
    case 1:
        *static_cast<std::uint8_t*>(instructionData.pOp1) |= *static_cast<std::uint8_t*>(instructionData.pOp2);
        break;
    case 2:
        *static_cast<std::uint16_t*>(instructionData.pOp1) |= *static_cast<std::uint16_t*>(instructionData.pOp2);
        break;
    case 4:
        *static_cast<std::uint32_t*>(instructionData.pOp1) |= *static_cast<std::uint32_t*>(instructionData.pOp2);
        break;
    case 8:
        *static_cast<std::uint64_t*>(instructionData.pOp1) |= *static_cast<std::uint64_t*>(instructionData.pOp2);
        break;
    }
}

void VMHandler::NOT(VMContext* pVmContext)
{
    auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::NOT);
    VM_INSTRUCTION_OP1 instructionData = {};
    vmInstruction.decodeOp1(&instructionData);

    switch (instructionData.op1SizeBytes) {
    case 1:
        *static_cast<std::uint8_t*>(instructionData.pOp1) = ~(*static_cast<std::uint8_t*>(instructionData.pOp1));
        break;
    case 2:
        *static_cast<std::uint16_t*>(instructionData.pOp1) = ~(*static_cast<std::uint16_t*>(instructionData.pOp1));
        break;
    case 4:
        *static_cast<std::uint32_t*>(instructionData.pOp1) = ~(*static_cast<std::uint32_t*>(instructionData.pOp1));
        break;
    case 8:
        *static_cast<std::uint64_t*>(instructionData.pOp1) = ~(*static_cast<std::uint64_t*>(instructionData.pOp1));
        break;
    }
}

void VMHandler::XOR(VMContext* pVmContext)
{
    auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::AND);
    VM_INSTRUCTION_OP2 instructionData = {};
    vmInstruction.decodeOp2(&instructionData);

    switch (instructionData.op1SizeBytes) {
    case 1:
        *static_cast<std::uint8_t*>(instructionData.pOp1) ^= *static_cast<std::uint8_t*>(instructionData.pOp2);
        break;
    case 2:
        *static_cast<std::uint16_t*>(instructionData.pOp1) ^= *static_cast<std::uint16_t*>(instructionData.pOp2);
        break;
    case 4:
        *static_cast<std::uint32_t*>(instructionData.pOp1) ^= *static_cast<std::uint32_t*>(instructionData.pOp2);
        break;
    case 8:
        *static_cast<std::uint64_t*>(instructionData.pOp1) ^= *static_cast<std::uint64_t*>(instructionData.pOp2);
        break;
    }
}

void VMHandler::push(VMContext* pVmContext)
{
    auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::PUSH);
    VM_INSTRUCTION_OP1 op1 = {};
    vmInstruction.decodeOp1(&op1);

    pVmContext->getStack().push(op1.pOp1, op1.op1SizeBytes);
}

void VMHandler::pop(VMContext* pVmContext)
{
    auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::POP);
    VM_INSTRUCTION_OP1 op1 = {};
    vmInstruction.decodeOp1(&op1);

    pVmContext->getStack().pop(op1.pOp1, op1.op1SizeBytes);
}

void VMHandler::add(VMContext* pVmContext)
{
    auto vmInstruction = VMInstruction(pVmContext, VM_OPCODE::ADD);

    VM_INSTRUCTION_OP2 op2 = {};
    vmInstruction.decodeOp2(&op2);

    //Add src and dst together and store result in dst
    performOperationSigned((std::int64_t*)op2.pOp1, (std::int64_t*)op2.pOp2, op2.op1SizeBytes, op2.op2SizeBytes,
        [&](std::int64_t p1, std::int64_t p2) { return p1 + p2; });
    

    //Update flags acording to the result of add
}

void VMHandler::sub(VMContext* pVmContext)
{

}