#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "libAT89S.h"
#include "libHexFile.h"
#include "gui.h"

extern MyGUI myGui;

AT89S_Part partList[]={
	{"AT89S51", {0x1E, 0x51, 0x06}, 4*1024}, \
	{"AT89S52", {0x1E, 0x52, 0x06}, 8*1024}
};

void myAT89S::ReadFlash(char *szPartName, char *szFile) {
	//cap phat bo nho
	uint16_t i, partIndex, k;
	uint16_t n;
	uint16_t u16FlashSize, u16NumOfPack;
	uint16_t j;
	
	printf("*------------------------------------------------*\n");
	printf("Part: %s\n", szPartName);
	printf("*------------------------------------------------*\n");
	
	//return;
	
	n=sizeof(partList)/sizeof(AT89S_Part);

	for(i=0; i<n; ++i) {
		if(strcmp(szPartName, partList[i].szName)==0) {
			u16FlashSize=partList[i].u16FlashSize;
			partIndex=i;
			break;
		}
	}
	//return;
	if(i==n) {
		myGui.SetProgress("Error part\n");
		return;
	}

	//cap phat bo nho
	uint8_t *buff;
	buff=(uint8_t*)malloc(u16FlashSize);

	//khoi tao programmer
	if(prog->Init()) {
		myGui.SetProgress("Init Programmer: Error\n");
		free(buff);
		return;
	}

	if(ChipEnable()) {
		printf("Chip Enable: ERROR\n");
		free(buff);
		prog->Exit();
		return;
	}


	//kiem tra ID
	uint8_t u8ID[3];

	if(ReadID(u8ID)) {
		printf("Read ID: ERROR\n");
		free(buff);
		prog->Exit();
		return;
	}

	for(i=0; i<3; ++i) {
		if(partList[partIndex].ID[i]!=u8ID[i]) {
			printf("Check ID: ERROR\n");
			free(buff);
			prog->Exit();
			return;
		}
	}

	printf("Check ID: %02X %02X %02X: OKAY\n", u8ID[0], u8ID[1], u8ID[2]);
	printf("*------------------------------------------------*\n");

	//doc flash tu MCU
	u16NumOfPack=u16FlashSize/64;
	printf("Read Flash:\n");

	//them phan hien thi tien trinh doc
	i=1;
	for(k=0; k<50; ++k) {
		printf(".");
	}
	fflush(stdout);
	for(j=0; j<u16NumOfPack; ++j) {
		if(prog->ReadFlashAT89S(j*64, 64, &buff[j*64])) {
			printf("Read Flash at: %04X ERROR\n", j*64);
			free(buff);
			prog->Exit();
			return;
		}
		if((i*u16NumOfPack/50)==j) {
			++i;
			printf("\r");
			for(k=0; k<i; ++k) {
				printf("#");
			}
			for(k=i; k<50; ++k) {
				printf(".");
			}
			fflush(stdout);
		}
	}
	printf("\n");
	printf("*------------------------------------------------*\n");

	//ghi du lieu ra file
	//kiem tra xem file hex hay file BIN
	uint16_t u16Len;
	u16Len=strlen(szFile);

	if(strcmp(&szFile[u16Len-4], ".hex")==0) {
		//HEX file

		//printf("File Type: hex file\n");
		HexFile hex;
		hex.SetFlashSize(u16FlashSize);
		hex.WriteBuffToFile(szFile, buff);
	} else {
		//bin file
		//printf("File Type: bin file\n");
		FILE *f;
		f=fopen(szFile, "wb");
		if(f==NULL) {
			printf("Create file ERROR\n");
			free(buff);
			prog->Exit();
			return;
		}
		fwrite(buff, 1, u16FlashSize, f);
		fclose(f);
	}
	printf("OpenProg by Ngo Hung Cuong\n");
	printf("http://ngohungcuong.com\n");
	printf("ngohungcuong@gmail.com\n");
	printf("0989696971 - 0915696971\n");
	//printf("Write Flash to file: %s\n", szFile);
	free(buff);
	prog->Exit();
}
void myAT89S::WriteFlash(char *szPartName, char *szFile) {
	//cap phat bo nho
	uint16_t i, partIndex, k;
	uint16_t n;
	uint16_t u16FlashSize, u16NumOfPack;
	uint16_t j;
	
	//printf("*------------------------------------------------*\n");
	//printf("Part: %s\n", szPartName);
	//printf("*------------------------------------------------*\n");

	n=sizeof(partList)/sizeof(AT89S_Part);

	for(i=0; i<n; ++i) {
		if(strcmp(szPartName, partList[i].szName)==0) {
			u16FlashSize=partList[i].u16FlashSize;
			partIndex=i;
			break;
		}
	}
	if(i==n) {
		myGui.SetProgress("Error part");
		return;
	}

	//cap phat bo nho
	uint8_t *buff, *buffread;
	buff=(uint8_t*)malloc(u16FlashSize);
	buffread=(uint8_t*)malloc(u16FlashSize);
	memset(buff, 0xFF, u16FlashSize);
	memset(buffread, 0xFF, u16FlashSize);
	//khoi tao programmer
	if(prog->Init()) {
		myGui.SetProgress("Init Programmer: Error");
		free(buff);
		free(buffread);
		return;
	}

	if(ChipEnable()) {
		myGui.SetProgress("Chip Enable: ERROR");
		free(buff);
		free(buffread);
		prog->Exit();
		return;
	}


	//kiem tra ID
	uint8_t u8ID[3];

	if(ReadID(u8ID)) {
		myGui.SetProgress("Read ID: ERROR");
		free(buff);
		free(buffread);
		prog->Exit();
		return;
	}

	for(i=0; i<3; ++i) {
		if(partList[partIndex].ID[i]!=u8ID[i]) {
			myGui.SetProgress("Check ID: ERROR");
			free(buff);
			free(buffread);
			prog->Exit();
			return;
		}
	}

	myGui.SetProgress("Check ID: OKAY");
	//printf("*------------------------------------------------*\n");

	//xoa chip
	myGui.SetProgress("Erase");
	fflush(stdout);
	ChipErase();
	//printf("*------------------------------------------------*\n");
	fflush(stdout);

	uint16_t u16Len;
	u16Len=strlen(szFile);

	if(strcmp(&szFile[u16Len-4], ".hex")==0) {
		HexFile hex;
		hex.SetFlashSize(u16FlashSize);
		if(hex.ReadFileToBuff(szFile, buff)) {
			myGui.SetProgress("Read file ERROR");
			free(buff);
			free(buffread);
			prog->Exit();
			return;
		}
		//hex.WriteBuffToFile("bb.hex", buff);
	} else {
		FILE *f;
		f=fopen(szFile, "rb");
		if(f==NULL) {
			myGui.SetProgress("Open file ERROR");
			free(buff);
			free(buffread);
			prog->Exit();
			return;
		}

		fread(buff, 1, u16FlashSize, f);
		fclose(f);
	}

	

	//doc flash tu MCU
	u16NumOfPack=u16FlashSize/32;
	myGui.SetProgress("Write Flash ...");

	//them phan hien thi tien trinh doc

	for(j=0; j<u16NumOfPack; ++j) {
		if(prog->WriteFlashAT89S(j*32, 32, &buff[j*32])) {
			myGui.SetProgress("Write Flash: ERROR");
			free(buff);
			free(buffread);
			prog->Exit();
			return;
		}
		myGui.SetProgress((float)j/u16NumOfPack);
	}
	//printf("\n");
	//printf("*------------------------------------------------*\n");
	
	myGui.SetProgress("Write Flash: OKAY");

	//printf("*------------------------------------------------*\n");

	//doc lai flash de kiem tra
	//doc flash tu MCU
	u16NumOfPack=u16FlashSize/64;
	myGui.SetProgress("Read Flash ...");

	//them phan hien thi tien trinh doc
	
	for(j=0; j<u16NumOfPack; ++j) {
		if(prog->ReadFlashAT89S(j*64, 64, &buffread[j*64])) {
			myGui.SetProgress("Read Flash: ERROR");
			free(buff);
			free(buffread);
			prog->Exit();
			return;
		}
		myGui.SetProgress((float)j/u16NumOfPack);
	}
	//printf("\n");
	myGui.SetProgress("Read Flash: OKAY");
	//printf("*------------------------------------------------*\n");

	if(memcmp(buff, buffread, u16FlashSize)) {
		myGui.SetProgress("Verify: ERROR");
	} else {
		myGui.SetProgress("Verify: OKAY");
	}
	
	//printf("*------------------------------------------------*\n");
	//printf("OpenProg by Ngo Hung Cuong\n");
	//printf("http://ngohungcuong.com\n");
	//printf("ngohungcuong@gmail.com\n");
	//printf("0989696971 - 0915696971\n");
	
	free(buff);
	free(buffread);
	prog->Exit();
}
void myAT89S::ReadLock(char *szPartName, char *szFile) {

}
void myAT89S::WriteLock(char *szPartName, char *szFile) {

}

uint32_t myAT89S::ChipEnable() {
	//goi toi ham SPI
	uint8_t buff[64];
	buff[0]=0xAC;
	buff[1]=0x53;
	buff[2]=0x00;
	buff[3]=0x00;
	if(prog->Spi(4, buff)) {
		return 1;
	}
	if(buff[3]==0x69) {
		return 0;
	}
	return 1;
}

uint32_t myAT89S::ReadID(uint8_t *pu8ID) {
	uint8_t buff[64];

	buff[0]=0x28;
	buff[1]=0x00;
	buff[2]=0x00;
	buff[3]=0x00;
	if(prog->Spi(4, buff)) {
		return 1;
	}
	pu8ID[0]=buff[3];

	buff[0]=0x28;
	buff[1]=0x01;
	buff[2]=0x00;
	buff[3]=0x00;
	if(prog->Spi(4, buff)) {
		return 1;
	}
	pu8ID[1]=buff[3];

	buff[0]=0x28;
	buff[1]=0x02;
	buff[2]=0x00;
	buff[3]=0x00;
	if(prog->Spi(4, buff)) {
		return 1;
	}
	pu8ID[2]=buff[3];

	return 0;
}

void myAT89S::ChipErase() {
	uint8_t buff[64];
	buff[0]=0xAC;
	buff[1]=0x80;
	buff[2]=0x00;
	buff[3]=0x00;
	prog->Spi(4, buff);
	usleep(500*1000);
}
