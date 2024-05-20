
///*Exported routines*/
//__declspec(dllexport) void Init();

//BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
//	switch (fdwReason) {
//	case DLL_PROCESS_ATTACH:
//		DisableThreadLibraryCalls(hinstDLL);
//		break;
//	}
//
//	return TRUE;
//}

#include <stdio.h>
#include <inttypes.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include "pe\PE.hpp"
#include "pe\PdbParser.hpp"
#include "third/Zydis/Zydis.h"
#include "compiler/Function.hpp"
#include "compiler/passes/virtualizer/Virtualizer.hpp"

int main() {

	auto pe = PE("C:\\Users\\Kilian\\source\\repos\\Test\\x64\\Release\\Test.exe");

	auto pdbparser = PdbParser(pe);

	auto v = pdbparser.enumFunctionSymbols();

    const auto symFn = v.at(14);

	auto pBaseFunc = pe.getMappedPeBuffer() + 0x1000 + symFn.offset;

    auto fn = CFunction(pBaseFunc, symFn.size);

	std::cout << fn << std::endl;

	auto vlizer = CVirtualizer(&fn, pBaseFunc, symFn.size);
	vlizer.transform();
	

	std::system("pause");
}