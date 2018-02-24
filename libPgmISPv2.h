#ifndef PGM_ISPV2_H_
#define PGM_ISPV2_H_
#include <stdint.h>
#include <unistd.h>

#include "libProgrammer.h"

#include "libUSBBULK.h"

#define ISPV2_VID 0x1986
#define ISPV2_PID 0x0002

#define INIT_CMD 0x60
#define EXIT_CMD 0x61
#define SPI_CMD 0x63
#define READ_FLASH_CMD 0x83
#define WRITE_FLASH_CMD 0x84

class PgmISPv2: public myProgrammer {
private:
	myUSBBULK myUsb;
public:
	virtual uint32_t Init();
	virtual uint32_t Exit();
	virtual uint32_t Spi(uint8_t u8Len, uint8_t *pu8Buff);
	virtual uint32_t ReadFlashAT89S(uint16_t u16Addr, uint8_t u8Len, uint8_t *pu8Buff);
	virtual uint32_t WriteFlashAT89S(uint16_t u16Addr, uint8_t u8Len, uint8_t *pu8Buff);
};

#endif
