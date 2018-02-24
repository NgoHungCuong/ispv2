#ifndef LIB_AT89S_H_
#define LIB_AT89S_H_
#include <stdint.h>
#include "libProgrammer.h"

struct AT89S_Part
{
	char szName[260];
	uint8_t ID[3];
	uint16_t u16FlashSize;
};

class myAT89S {
public:
	void ReadFlash(char *szPartName, char *szFile);
	void WriteFlash(char *szPartName, char *szFile);
	void ReadLock(char *szPartName, char *szFile);
	void WriteLock(char *szPartName, char *szFile);
	uint32_t ChipEnable();
	void ChipErase();
	uint32_t ReadID(uint8_t *pu8ID);
	uint32_t Init();
	void Exit();

	myProgrammer *prog;
};

#endif