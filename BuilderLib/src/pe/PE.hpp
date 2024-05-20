#pragma once

#define GetImgDirEntryRVA( pNTHdr, IDE ) \
	(pNTHdr->OptionalHeader.DataDirectory[IDE].VirtualAddress)

#define GetImgDirEntrySize( pNTHdr, IDE ) \
	(pNTHdr->OptionalHeader.DataDirectory[IDE].Size)

class PE
{
	PIMAGE_DOS_HEADER m_DosHeader;
	PIMAGE_NT_HEADERS m_NtHeader;
	PIMAGE_OPTIONAL_HEADER m_OptHeader;
	PIMAGE_FILE_HEADER m_FileHeader;
	mutable std::vector<std::uint8_t> m_PeRawBuffer;
	mutable std::vector<std::uint8_t> m_PeMappedBuffer;
	std::string m_PePath;

public:

	PE(std::uint8_t* pe_buffer);
	PE(const std::string& filePath);

	std::vector<IMAGE_SECTION_HEADER> getPeSections();
	std::vector<std::uint8_t> ReadPeFromDisk(const std::string& filePath);
	void MapViewOffPe();

	inline std::uint8_t* getRawPeBuffer() const { return m_PeRawBuffer.data(); }
	inline std::uint8_t* getMappedPeBuffer() const { return m_PeMappedBuffer.data(); }

	inline void* GetPtrFromRVA(std::uint64_t RVA) const
	{
		auto pSecHeader = IMAGE_FIRST_SECTION(m_NtHeader);
		for (std::uint8_t i = 0; i < m_NtHeader->FileHeader.NumberOfSections; i++, pSecHeader++)
		{
			if (RVA >= pSecHeader->VirtualAddress && RVA <=
				(pSecHeader->VirtualAddress + pSecHeader->Misc.VirtualSize))
			{
				const auto delta = pSecHeader->VirtualAddress - pSecHeader->PointerToRawData;
				return (void*)(getRawPeBuffer() + RVA - delta);
			}
		}

		return nullptr;
	}

	inline auto GetDosHeader() const { return m_DosHeader; }
	inline auto GetNtHeader() const { return m_NtHeader; }
	inline auto GetOptHeader() const { return m_OptHeader; }
	inline auto GetFileHeader() const { return m_FileHeader; }
	inline auto Isx64() const { return m_FileHeader->Machine; }
	inline auto GetSubsystem() const { return m_OptHeader->Subsystem; }
	inline void SetSubsystem(WORD s) { m_OptHeader->Subsystem = s; }
	inline auto GetImageDirSize(DWORD ImageDir) const { return m_OptHeader->DataDirectory[ImageDir].Size; }

	inline auto GetRawTextPtr() const { return getRawPeBuffer() + m_OptHeader->BaseOfCode; }


	inline auto GetTextSize() const { return m_OptHeader->SizeOfCode; }
	inline auto getPath() const { return m_PePath; }

	inline auto GetDebugDirectory() const { 
		return !m_OptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress ? nullptr
			: GetPtrFromRVA(m_OptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress);
	}

	inline auto GetImageDirPtr(DWORD ImageDir) const
	{
		const auto RVA = GetImgDirEntryRVA(m_NtHeader, ImageDir);
		return GetPtrFromRVA(RVA);
	}
};