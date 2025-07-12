#include <Windows.h>
#include "RedoBlHooks.hpp"
#include "torque.hpp"
#include <exception>
#include <iostream>
#include <fstream>

typedef unsigned int U32;
typedef signed int S32;
typedef float F32;

struct HashTableData
{
	struct Entry
	{
		enum
		{
			TypeInternalInt = -3,
			TypeInternalFloat = -2,
			TypeInternalString = -1,
		};

		const char* name; /*StringTableEntry*/
		Entry* nextEntry;

		//didnt check if these vars are aligned
		S32 type;
		char* sval;
		U32 ival;  // doubles as strlen when typeName = -1 (MISLEADING / LIE ? / DEPRECATED)
		F32 fval;
		U32 bufferLen;
		void* dataPtr;
	};

	S32 size;
	S32 count;
	Entry** data;
};

int exportGlobalVariableData(ADDR obj, int argc, const char* argv[])
{
	HashTableData globalVars = *(HashTableData*)tsf_gEvalState_globalVars;

	std::ofstream file;
	file.open("./config/globalVarTable.tsv");

	U32 numEntries = 0;
	U32 walkedEntries = 0;
	for (S32 i = 0; i < globalVars.size; i++)
	{
		HashTableData::Entry* walk = globalVars.data[i];
		U32 lastWalkCount = 0;
		while (walk)
		{
			switch (walk->type)
			{
			case HashTableData::Entry::TypeInternalInt:
				file << walk->name << "\tInteger\t" << (int)walk->ival << "\n";
				break;
			case HashTableData::Entry::TypeInternalFloat:
				file << walk->name << "\tInteger\t" << walk->fval << "\n";
				break;
			case HashTableData::Entry::TypeInternalString:
				file << walk->name << "\tString\t" << walk->bufferLen << "\n";
				break;
			}

			walk = walk->nextEntry;
			numEntries++;
			lastWalkCount++;
		}

		walkedEntries += lastWalkCount - 1;
	}

	file << "hashtable size: " << globalVars.size << "\n";
	file << "real variable count: " << numEntries << "\n";
	file << "walked Entries: " << walkedEntries << "\n";
	file.close();

	BlPrintf("Exported %i variables to: config/globalVarTable.tsv", numEntries);

	return 1;
}

bool init()
{
	BlInit;
	if (!tsf_InitInternal())
		return false;

	BlPrintf("exportGlobalVariableData Initialized!");
	tsf_AddConsoleFunc(NULL, NULL, "exportGlobalVariableData", exportGlobalVariableData, "()", 1, 1);

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		return init();
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}