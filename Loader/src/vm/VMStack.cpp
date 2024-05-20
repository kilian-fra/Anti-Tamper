#include <iostream>
#include "VMStack.hpp"

VMStack::VMStack()
{
	//Alloc memory for vm stack
	m_Buffer = new std::uint8_t[VMStack::VIRTUAL_STACK_SIZE];

	//Init VIP
	m_VSP = m_Buffer + VMStack::VIRTUAL_STACK_SIZE;
}

VMStack::~VMStack()
{
	if (m_Buffer) delete[] m_Buffer;
}

void VMStack::push(void* src, std::uint8_t sizeBytes)
{
	//Check stack overflow
	if (m_VSP - sizeBytes < m_Buffer)
		return;

	//Copy from src
	std::memcpy(m_VSP, src, sizeBytes);

	//Decrement VIP to point to the new top of the stack
	m_VSP -= sizeBytes;
}

void VMStack::pop(void* dst, std::uint8_t sizeBytes)
{
	//Check stack underflow
	if (m_VSP + sizeBytes > m_Buffer + VMStack::VIRTUAL_STACK_SIZE)
		return;

	//Copy to dst
	std::memcpy(dst, m_VSP - sizeBytes, sizeBytes);

	//Increment vip
	m_VSP += sizeBytes;
}