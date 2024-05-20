#include <Windows.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include "PE.hpp"

PE::PE(std::uint8_t* pe_buffer)
{
	//m_BufferPE = pe_buffer;
	m_DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(pe_buffer);
	m_NtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(pe_buffer + m_DosHeader->e_lfanew);
	m_OptHeader = &m_NtHeader->OptionalHeader;
	m_FileHeader = &m_NtHeader->FileHeader;
}

PE::PE(const std::string& filePath)
{
	m_PePath = filePath;
	m_PeRawBuffer = ReadPeFromDisk(filePath);
	m_DosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(getRawPeBuffer());
	m_NtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(getRawPeBuffer() + m_DosHeader->e_lfanew);
	m_OptHeader = &m_NtHeader->OptionalHeader;
	m_FileHeader = &m_NtHeader->FileHeader;

	MapViewOffPe();
}

std::vector<IMAGE_SECTION_HEADER> PE::getPeSections()
{
	std::vector<IMAGE_SECTION_HEADER> sections;
	auto currSection = IMAGE_FIRST_SECTION(m_NtHeader);

	for (int i = 0; i < m_FileHeader->NumberOfSections; ++i, ++currSection)
		sections.push_back(*currSection);

	return sections;
}

std::vector<std::uint8_t> PE::ReadPeFromDisk(const std::string& filePath)
{
	const auto fPath = std::filesystem::path(filePath);

	if (!std::filesystem::exists(fPath))
		return std::vector<std::uint8_t>();

	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) return std::vector<std::uint8_t>();

	//Determine size
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<std::uint8_t> fileData(fileSize);
	if (!file.read(reinterpret_cast<char*>(fileData.data()), fileSize))
		return std::vector<std::uint8_t>();

	file.close();
	return fileData;
}

void PE::MapViewOffPe()
{
	m_PeMappedBuffer.resize(m_OptHeader->SizeOfImage);

	//Copy all headers first
	std::memcpy(m_PeMappedBuffer.data(), m_PeRawBuffer.data(), 0x1000);

	//Map all sections at VA
	auto sections = getPeSections();

	for (const auto& currSec : sections) {
		if (!currSec.VirtualAddress || !currSec.SizeOfRawData) continue;
		std::memcpy(m_PeMappedBuffer.data() + currSec.VirtualAddress,
			m_PeRawBuffer.data() + currSec.PointerToRawData, currSec.SizeOfRawData);
	}
}