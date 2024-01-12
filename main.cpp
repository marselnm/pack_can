#include <stdio.h>
#include <string.h>
#include "struct.h"

#include <iostream>

static double pi = 3.14159265358979323;
void SetTestPIif(stPIif* sPIif);
double ToRadians(stGrMS* sGrMS);
void swap_double(double *d);
void putDataInCanStream(uint8_t* outStream, uint8_t* data, uint16_t size_data);



int main()
{
    printf("Hello pack can\r\n");
    uint8_t streamCanOutputVer1[176];

    stPIif sRcvCanOutputVer1;
    SetTestPIif(&sRcvCanOutputVer1);
    memset(streamCanOutputVer1, 0, 176);

    for(int i = 0; i < sizeof(stPIif); ++i)
    {
        printf("%X ", *((uint8_t*)(&sRcvCanOutputVer1) + i));    
    }
    printf("\n\r");

    putDataInCanStream(streamCanOutputVer1, (uint8_t*)(&sRcvCanOutputVer1), sizeof(stPIif));


    for(int i = 0; i < 22; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            printf("%X ", streamCanOutputVer1[8*i + j]);

            if (j == 7)
            {
                printf("\n\r");
            }
        }
    }


    std::cout << sizeof(stPIif) << std::endl;



    return 0;
}

void putDataInCanStream(uint8_t* outStream, uint8_t* data, uint16_t size_data)
{
    uint8_t outStream1[176];
    memset(outStream1, 0, 176);

    outStream[2] = (size_data & 0xFF);
    outStream[3] = (size_data >> 8) & 0xFF;

    uint32_t crc = 0;
    	
	for (int i = 0; i < size_data; ++i)
    {
		crc += *(data + i);
	}

    for(int i = 0; i < 4; ++i)
    {
        outStream[4 + i] = (crc >> 8*i) & 0xFF; 
    }

    int numPackets = (size_data + 6 - 1) / 6;

    for(int i = 1; i < numPackets + 1; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            if (j == 0)
            {
                outStream[8*i + j] = i;
                continue;
            }

            if (j == 1)
            {
                outStream[8*i + j] = 0;
                continue;
            }

            int indx = 6*(i - 1) + j - 2;  

            if (indx < size_data)
            {
                outStream[8*i + j] = data[indx];
            }      
        }
    }
}

// 23:52:20.294 1 0x240 STD Rx 8 00 00 7D 00 08 27 00 00
// 23:52:20.294 1 0x240 STD Rx 8 01 00 01 40 1F 00 01 01
// 23:52:20.294 1 0x240 STD Rx 8 02 00 04 04 04 04 04 05
// 23:52:20.294 1 0x240 STD Rx 8 03 00 01 01 01 01 01 00
// 23:52:20.294 1 0x240 STD Rx 8 04 00 80 AC 43 00 00 AD
// 23:52:20.294 1 0x240 STD Rx 8 05 00 43 00 80 AD 43 00
// 23:52:20.294 1 0x240 STD Rx 8 06 00 00 AE 43 00 80 AE
// 23:52:20.294 1 0x240 STD Rx 8 07 00 43 00 00 6F 44 00
// 23:52:20.294 1 0x240 STD Rx 8 08 00 40 6F 44 00 80 6F
// 23:52:20.294 1 0x240 STD Rx 8 09 00 44 00 C0 6F 44 00
// 23:52:20.294 1 0x240 STD Rx 8 0A 00 00 70 44 06 01 85
// 23:52:20.294 1 0x240 STD Rx 8 0B 00 EB 51 40 CC A7 E7
// 23:52:20.294 1 0x240 STD Rx 8 0C 00 3F 6C E5 F8 0C CD
// 23:52:20.294 1 0x240 STD Rx 8 0D 00 D1 FF BF 7F 10 4E
// 23:52:20.294 1 0x240 STD Rx 8 0E 00 68 A4 70 45 41 5C
// 23:52:20.294 1 0x240 STD Rx 8 0F 00 8F 0B 42 DA 13 F0
// 23:52:20.294 1 0x240 STD Rx 8 10 00 3F 3B 8D 80 F2 4F
// 23:52:20.294 1 0x240 STD Rx 8 11 00 FD FC BF C6 3B FE
// 23:52:20.294 1 0x240 STD Rx 8 12 00 35 00 00 00 00 00
// 23:52:20.294 1 0x240 STD Rx 8 13 00 00 80 3F 00 00 00
// 23:52:20.294 1 0x240 STD Rx 8 14 00 40 00 00 40 40 00
// 23:52:20.294 1 0x240 STD Rx 8 15 00 00 80 40 F9 25 01

void SetTestPIif(stPIif* sPIif)
{
    memset(sPIif, 0, sizeof(stPIif));

	sPIif->cWorkMode = 1;
	sPIif->nGRI = 8000;
	sPIif->cMS = 0;
	sPIif->cASUif = 1;
	sPIif->cASUf = 1;
	sPIif->cNum = 5;

	for(uint8_t i = 0; i < 5; ++i)
	{
		sPIif->mode[i] = 4;
		sPIif->YN[i] = 1;
		sPIif->TDif[i] = 345.00 + i;
		sPIif->TDcor[i] = 956 + i;
		sPIif->fAux[i] = i;
	}

	sPIif->cFlags.cData = 6;
	sPIif->cNav = 1;
    
    stGrMS stTestB;
    stTestB.nHemS = 1;
    stTestB.nDeg = 42;
    stTestB.nMin = 21;
    stTestB.fSec = 17.76;

    stGrMS stTestL;
    stTestL.nHemS = -1;
    stTestL.nDeg = 113;
    stTestL.nMin = 56;
    stTestL.fSec = 43.21;
    
    double tempB = ToRadians(&stTestB); 
    swap_double(&tempB);

    double tempL = ToRadians(&stTestL); 
    swap_double(&tempL);

    sPIif->fB = tempB;
	sPIif->fL = tempL;
    
	sPIif->fDOP = 3.28;
	sPIif->fVb = 12.34;
	sPIif->fVl = 34.89;

    stGrMS stTestBsat;
    stTestBsat.nHemS = 1;
    stTestBsat.nDeg = 57;
    stTestBsat.nMin = 34;
    stTestBsat.fSec = 24.67;

    stGrMS stTestLsat;
    stTestLsat.nHemS = -1;
    stTestLsat.nDeg = 103;
    stTestLsat.nMin = 48;
    stTestLsat.fSec = 39.47;

    double tempBsat = ToRadians(&stTestBsat); 
    swap_double(&tempBsat);

    double tempLsat = ToRadians(&stTestLsat); 
    swap_double(&tempLsat);

	sPIif->fBc = tempBsat;
	sPIif->fLc = tempLsat;

	uint16_t chSum = 15;

	uint8_t *p = (uint8_t*)sPIif;

	for (int i = 0; i < sizeof(struct stPIif) - 2; i++)
    {
		chSum += *(p++);
	}
	sPIif->nCheckSum = chSum;
}

double ToRadians(stGrMS* sGrMS)
{
	return ((double)(sGrMS->nDeg * pi / 180) + (double)(sGrMS->nMin * pi / (180 * 60)) + (double)(sGrMS->fSec * pi / (180 * 3600)))*(double)sGrMS->nHemS;
}

void swap_double(double *d)
{
	union {
		uint8_t bytes[sizeof(double)];
		double val;
	} p;
	p.val = *d;
	uint8_t tmp[sizeof(double) / 2];
	memcpy(tmp, p.bytes, sizeof(tmp));
	memcpy(p.bytes, p.bytes + sizeof(double) / 2, sizeof(tmp));
	memcpy(p.bytes + sizeof(double) / 2, tmp, sizeof(tmp));
	*d = p.val;
}





