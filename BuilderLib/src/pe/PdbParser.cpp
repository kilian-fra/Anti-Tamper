#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <DbgHelp.h>
#include "PE.hpp"
#include "PdbParser.hpp"

#pragma comment(lib, "dbghelp.lib")

bool PdbParser::init(const PE& pe)
{
	if (!SymInitialize(GetCurrentProcess(), nullptr, false))
		return false;

	if (!pe.GetDebugDirectory()) return false;

	for (auto currDebugDir = reinterpret_cast<PIMAGE_DEBUG_DIRECTORY>(pe.GetDebugDirectory())
		; currDebugDir && currDebugDir->SizeOfData; ++currDebugDir) {
		if (currDebugDir->Type != IMAGE_DEBUG_TYPE_CODEVIEW) continue;

		auto codeViewInfo = reinterpret_cast<codeviewInfo_t*>(pe.getRawPeBuffer() + currDebugDir->PointerToRawData);

		std::string pdbPath = codeViewInfo->PdbFileName;
		if (!std::filesystem::exists(std::filesystem::path(pdbPath))) {
			auto pdbAltPath = std::filesystem::path(pe.getPath());
			pdbAltPath.replace_extension(".pdb");
			pdbPath = pdbAltPath.string();
			if (!std::filesystem::exists(std::filesystem::path(pdbPath))) return false;
		}

		m_ModBase = reinterpret_cast<std::uint8_t*>
			(SymLoadModuleEx(GetCurrentProcess(), nullptr, pdbPath.c_str(), nullptr,
				0x10000000, static_cast<DWORD>(std::filesystem::file_size(pdbPath)), nullptr, 0));
		break;
	}

	return m_ModBase != nullptr;
}

PdbParser::PdbParser(const PE& pe)
	: m_ModBase(nullptr)
{
	init(pe);
}

PdbParser::~PdbParser()
{
	SymCleanup(GetCurrentProcess());
}

std::vector<SymFunc> PdbParser::enumFunctionSymbols()
{
	std::vector<SymFunc> symData;

	struct callback_data {
		std::uint64_t base;
		std::vector<SymFunc>& symData;
	};

	callback_data cb_data = { reinterpret_cast<std::uint64_t>(m_ModBase), symData};


	const auto enumSymbolsCallback = []
		(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext) -> BOOL {
		if (!pSymInfo || pSymInfo->Tag != SymTagFunction) return TRUE;

		SymFunc newFunc = {};
		auto pCbData = static_cast<callback_data*>(UserContext);

		if (!SymGetTypeInfo(GetCurrentProcess(), pCbData->base,
			pSymInfo->Index, TI_GET_OFFSET, &newFunc.offset))
			SymGetTypeInfo(GetCurrentProcess(), pCbData->base,
				pSymInfo->Index, TI_GET_ADDRESSOFFSET, &newFunc.offset);

		if (std::find_if(pCbData->symData.begin(), pCbData->symData.end(),
			[&](const SymFunc func) { return func.offset == newFunc.offset; }) == pCbData->symData.end()) {
			newFunc.name = pSymInfo->Name;
			newFunc.size = pSymInfo->Size;
			pCbData->symData.push_back(newFunc);
		}

		return TRUE;
	};

	SymEnumSymbols(GetCurrentProcess(), reinterpret_cast<ULONG64>(m_ModBase),
		nullptr, (PSYM_ENUMERATESYMBOLS_CALLBACK)enumSymbolsCallback, &cb_data);

	return symData;
}