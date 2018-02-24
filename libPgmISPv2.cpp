#include "libPgmISPv2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t PgmISPv2::Init() {
	if(myUsb.FindDevice(ISPV2_VID, ISPV2_PID)) {
		//found programmer
		buff[0]=0x6D;//RST pin
		buff[1]=1;//RST = 1
		myUsb.Write(buff);
		
		usleep(10*1000);
		
		buff[0]=0x6D;//RST pin
		buff[1]=0;//RST = 0
		myUsb.Write(buff);
		
		usleep(10*1000);
		
		buff[0]=0x53;//clock speed
		buff[1]=9;//25khz
		myUsb.Write(buff);
		myUsb.Read(buff);
		
		buff[0]=0x60;//SPI init
		buff[1]=0;
		myUsb.Write(buff);
		
		buff[0]=0x6D;//RST pin
		buff[1]=1;//RST = 1
		myUsb.Write(buff);
		
		usleep(10*1000);
		
		return 0;
	}

	return 2;//not found
}
uint32_t PgmISPv2::Exit() {
	buff[0]=EXIT_CMD;

	if(myUsb.Write(buff)) {
		
		buff[0]=0x6D;//RST pin
		buff[1]=0;//RST = 0
		myUsb.Write(buff);
		
		usleep(10*1000);
		
		buff[0]=0x6D;//RST pin
		buff[1]=1;//RST = 0
		myUsb.Write(buff);
		
		usleep(10*1000);
		
		buff[0]=0x6D;//RST pin
		buff[1]=0;//RST = 0
		myUsb.Write(buff);
		
		myUsb.CloseDevice();
		return 0;
	}
	
	myUsb.CloseDevice();
	return 1;
}
uint32_t PgmISPv2::Spi(uint8_t u8Len, uint8_t *pu8Buff) {
	buff[0]=SPI_CMD;
	buff[1]=u8Len;
	uint8_t i;
	for(i=0; i<u8Len; ++i) {
		buff[2+i]=pu8Buff[i];
	}
	if(myUsb.Write(buff)) {
		if(myUsb.Read(buff)) {
			for(i=0; i<u8Len; ++i) {
				pu8Buff[i]=buff[i];
				//printf("%02X", buff[i]);
			}
			//printf("\n");
			return 0;
		} else {
			return 1;//error
		}
	}
	return 1;
}
uint32_t PgmISPv2::ReadFlashAT89S(uint16_t u16Addr, uint8_t u8Len, uint8_t *pu8Buff) {
	//kiem tra 256 byte 1 lan doc / ghi
	uint8_t tmp[100];
	
	if((u16Addr%256)==0) {
		tmp[0]=0x30;
		tmp[1]=(uint8_t)(u16Addr>>8);
		tmp[2]=(uint8_t)(u16Addr);
		Spi(2, tmp);
		Spi(32, &pu8Buff[0]);
		return Spi(32, &pu8Buff[32]);
	} else {
		Spi(32, &pu8Buff[0]);
		return Spi(32, &pu8Buff[32]);
	}

	return 1;
}
uint32_t PgmISPv2::WriteFlashAT89S(uint16_t u16Addr, uint8_t u8Len, uint8_t *pu8Buff) {
	uint8_t tmp[100];
	
	if((u16Addr%256)==0) {
		tmp[0]=0x50;
		tmp[1]=(uint8_t)(u16Addr>>8);
		tmp[2]=(uint8_t)(u16Addr);
		Spi(2, tmp);
		memmove(tmp, pu8Buff, 32);
		return Spi(32, tmp);
	} else {
		memmove(tmp, pu8Buff, 32);
		Spi(32, tmp);
		if((u16Addr%256)==224) {
			usleep(10*1000);
		}
		return 0;
	}
	return 1;
}
