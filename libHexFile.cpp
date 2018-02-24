#include <string.h>
#include "libHexFile.h"

unsigned char checksum2(unsigned char *pData, uint32_t iStart, uint32_t iFinish);

void HexFile::SetFlashSize(uint32_t u32FlashSize) {
	dwMemSize=u32FlashSize;
}

uint32_t HexFile::ReadFileToBuff(char *szFileName, uint8_t *buff) {
	
	pReadBuff=buff;

	FILE *fRead;

	fRead=fopen(szFileName,"rt");

	dwReadSize=0;

	if(!fRead) {
		//strcpy(szError,"File not found");
		return 1;
	}
	//lan luot doc vao tung dong va xu ly
	char *c;
	char szLine[200];
	//memset(mem_use,0,sizeof(mem_use));
	//memset(pReadBuff,0xff,sizeof(uData));//chu y cho nay ta se dien day bang 0xff
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	pMem=0;
	dwTmpAddr=0;
	while(1) {
		c=fgets(szLine,150,fRead);
		if(!c)
			break;
		//neu khong doc duoc thi thoat
		if(!strncmp(szLine,":00000001FF",11)) {
			break;
		}
		if(Process_Line(szLine)) {
			fclose(fRead);
			return 1;
		}
	}
	fclose(fRead);
	return 0;
}

uint32_t HexFile::Process_Line(char *szLine) {
	int n;
	char szpLine[200];
	strcpy(szpLine,szLine);
	n=strlen(szpLine);
	
	while((szpLine[n-1]==0x0A)||(szpLine[n-1]==0x0D)) {
		szpLine[n-1]=0;
		--n;
	}
	/*
	if(szpLine[n-1]=='\n') {
		szpLine[n-1]=0;
		--n;
	}
	*/
	//printf("%s\n", szLine);

	//lay do dai cua xau

	int i;
	//ky tu dau tien la ':'
	if(szpLine[0]!=':') {
		//strcpy(szError,"Hex file error");
		return 1;
	}
	//cac ky tu tiep theo chi la 0-9 A-F
	for(i=1;i<n;++i) {
		if(szpLine[i]>='0'&&szpLine[i]<='9')
			continue;
		if(szpLine[i]>='A'&&szpLine[i]<='F')
			continue;
		//strcpy(szError,"Hex file error");
		return 1;
	}
	//do dai
	//kiem tra cai thu 7
	//them phan kiem tra xem no thuoc type nao
	//neu la type 5 thi chu y
	uint32_t dwLo, dwHi;

	//char szTg[100];

	if(StrToNum(&szpLine[7])) {
		if(StrToNum(&szpLine[7])==0x04) {
			//================================================
			//chuyen con tro
			dwHi=StrToNum(&szpLine[9]);
			dwLo=StrToNum(&szpLine[11]);
			dwTmpAddr=(dwHi*0x100+dwLo)*0x10000;//(StrToNum(&szpLine[11]))*64*1024;

			//pMem=(dwHi*16+dwLo)*0x10000;//(StrToNum(&szpLine[11]))*64*1024;
			//sprintf(szTg, "T_Mem %08X", dwTmpAddr);
			//MessageBox(NULL, szTg, NULL, MB_OK);
		}
		return 0;
	}
	n=(int)StrToNum(&szpLine[1]);
	dwReadSize+=n;
	uint32_t dwLowAdd;
	uint32_t dwHiAdd;
	//dia chi
	dwLowAdd=StrToNum(&szpLine[5]);
	dwHiAdd=StrToNum(&szpLine[3]);
	pMem=dwTmpAddr+dwHiAdd*0x100;
	//pMem<<=8;
	pMem|=dwLowAdd;
	if(pMem>=0x08000000) {
		pMem-=0x08000000;
	}
	if(pMem>=dwMemSize) {
		//strcpy(szError,"File too big");
		return 1;
	}
	int iPos;
	iPos=9;
	for(i=0;i<n;++i) {
		//mem_use[pMem]=1;
		pReadBuff[pMem]=StrToNum(&szpLine[iPos]);
		iPos+=2;
		++pMem;
		if(pMem>dwMemSize) {
			//strcpy(szError,"File too big");
			return 1;
		}
	}
	return 0;
}

unsigned char HexFile::StrToNum(char *szStr) {
	unsigned char ret;
	char szPr[300];
	strncpy(szPr,szStr, 2);
	szPr[2]=0;
	if(szPr[0]>'9') {
		szPr[0]-='A';
		szPr[0]+=10;
	}
	else
		szPr[0]-='0';

	if(szPr[1]>'9') {
		szPr[1]-='A';
		szPr[1]+=10;
	}
	else
		szPr[1]-='0';

	ret=szPr[0];
	ret<<=4;
	ret|=szPr[1];
	return ret;
}

uint32_t HexFile::WriteBuffToFile(char *szFileName, uint8_t *buff) {
	uint32_t dwNum,i,j, addr;
	unsigned addrh, addrl;
	dwNum=dwMemSize/16;

	FILE *fWrite;
	pWriteBuff=buff;

	fWrite=fopen(szFileName,"wt");
	if(!fWrite)
		return 1;
	unsigned char uTg;

	uint32_t dwTg;

	unsigned char szRecTg[100];

	for(i=0;i<dwNum;++i) {
		//dau tien ta ghi ra so luong byte
		//dau tien can kiem tra xem co phai byte bat dau cua 64KB hay khong
		if(((i*0x10)%0x10000)==0) {
			//dua gia tri voi bat dau 0x04 ra
			//dua ra gia tri
			fprintf(fWrite,":02000004");
			//:02000004
			//tinh toan dia chi
			dwTg=i*0x10;
			dwTg>>=24;
			uTg=(unsigned char)dwTg;
			szRecTg[0]=uTg;
			fprintf(fWrite,"%02X",uTg);
			dwTg=i*0x10;
			dwTg>>=16;
			uTg=(unsigned char)dwTg;
			szRecTg[1]=uTg;
			fprintf(fWrite,"%02X",uTg);
			uTg=checksum2(szRecTg, 0, 1);
			fprintf(fWrite,"%02X\n", uTg);
		}
		fprintf(fWrite,":10");
		//2 byte dia chi
		addr=i*16;
		addrl=(unsigned char)addr;
		addr>>=8;
		addrh=(unsigned char)addr;
		fprintf(fWrite,"%02X%02X00",addrh,addrl);
		for(j=0;j<16;++j) {
			fprintf(fWrite,"%02X",pWriteBuff[i*16+j]);
		}
		//them checksum tai day
		uTg=checksum(i*16,i*16+15);
		fprintf(fWrite,"%02X\n", uTg);
	}

	fprintf(fWrite,":00000001FF\n");
	fprintf(fWrite,";Open Prog - Ngo Hung Cuong Electronics\n");
	fprintf(fWrite,";(C) 2017 by Ngo Hung Cuong\n");
	fprintf(fWrite,";http://ngohungcuong.com\n");
	fprintf(fWrite,";http://vnshop.org\n");
	fprintf(fWrite,";ngohungcuong@gmail.com\n");
	fprintf(fWrite,";0989696971 - 0915696971\n");
	fclose(fWrite);
	return 0;
}
unsigned char checksum2(unsigned char *pData, uint32_t iStart, uint32_t iFinish) {
	unsigned char uRes;
	unsigned char uCount;
	uint32_t dwCount=iFinish-iStart+1;
	uCount=(unsigned char)dwCount;
	uRes=uCount;
	uCount=(unsigned char)iStart;
	//uCount
	uRes+=uCount;
	uCount=(unsigned char)(iStart>>8);
	uRes+=uCount;
	uRes+=0x04;
	uint32_t i;
	for(i=iStart;i<=iFinish;++i) {
		uRes+=pData[i];
	}
	uRes*=-1;
	return uRes;
}
unsigned char HexFile::checksum(uint32_t iDau, uint32_t iCuoi) {
	unsigned char uRes;
	unsigned char uCount;
	uint32_t dwCount=iCuoi-iDau+1;
	uCount=(unsigned char)dwCount;
	uRes=uCount;
	uCount=(unsigned char)iDau;
	//uCount
	uRes+=uCount;
	uCount=(unsigned char)(iDau>>8);
	uRes+=uCount;
	uint32_t i;
	for(i=iDau;i<=iCuoi;++i) {
		uRes+=pWriteBuff[i];
	}
	uRes*=-1;
	return uRes;
}
