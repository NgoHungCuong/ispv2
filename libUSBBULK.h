#ifndef MY_USB_BULK_H
#define MY_USB_BULK_H
#include <stdint.h>
class myUSBBULK {
private:
	uint8_t u8Buff[8*1024];
public:
	uint32_t FindDevice(uint32_t u32VID, uint32_t u32PID);
	uint32_t CloseDevice(void);
	uint32_t Read(void *pData);
	uint32_t Read(uint16_t len, void *pData);
	uint32_t Write(void *pData);
	uint32_t Write(uint16_t len, void *pData);
};
#endif
