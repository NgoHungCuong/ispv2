#ifndef HEX_FILE_H_
#define HEX_FILE_H_
#include <stdint.h>
#include <stdio.h>
class HexFile {
private:
	uint32_t dwTmpAddr;
	uint8_t *pReadBuff, *pWriteBuff;
	uint32_t dwMemSize;
	uint32_t dwReadSize;
	uint32_t pMem;
	uint32_t Process_Line(char *szLine);
	unsigned char StrToNum(char *szStr);
	unsigned char checksum(uint32_t iDau, uint32_t iCuoi);
public:
	void SetFlashSize(uint32_t u32FlashSize);
	uint32_t ReadFileToBuff(char *szFileName, uint8_t *buff);
	uint32_t WriteBuffToFile(char *szFileName, uint8_t *buff);
};
#endif
