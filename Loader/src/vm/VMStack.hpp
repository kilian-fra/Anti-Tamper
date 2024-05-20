#pragma once

class VMStack
{
	std::uint8_t* m_VSP; //Virtual Stack Pointer
	std::uint8_t* m_Buffer;
	static constexpr std::size_t VIRTUAL_STACK_SIZE = 1024 * 1024; //1 Mib

public:

	VMStack();
	~VMStack();

	void push(void* src, std::uint8_t sizeBytes);
	void pop(void* dst, std::uint8_t sizeBytes);
};