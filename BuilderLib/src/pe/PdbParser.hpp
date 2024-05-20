#pragma once

struct SymFunc {
	std::string name;
	std::uint32_t offset;
	std::uint32_t size;
};

constexpr auto SymTagFunction = 5;

class PdbParser
{
	struct codeviewInfo_t
	{
		ULONG CvSignature;
		GUID Signature;
		ULONG Age;
		char PdbFileName[ANYSIZE_ARRAY];
	};


	bool init(const PE& pe);
	
	std::uint8_t* m_ModBase;

public:

	PdbParser(const PE& pe);
	~PdbParser();

	std::vector<SymFunc> enumFunctionSymbols();
};