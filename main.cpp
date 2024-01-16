#include <stdio.h>
#include <string.h>
#include "struct.h"

#include <iostream>

static double pi = 3.14159265358979323;
void SetTestPIif(stPIif* sPIif);
double ToRadians(stGrMS* sGrMS);
void swap_double(double *d);
void putDataInCanStream(uint8_t* outStream, uint8_t* data, uint16_t size_data);
void generateDebugData(stBoard* p_sBoard, stRvrIFRNS* p_sIFRNS);

void packRcvCanOutputVer2(stRcvCanOutputVer2* sRcvCanOutputVer2, stBoard* sBoard, stRvrIFRNS* sIFRNS);
void unpackRcvCanOutputVer2(stRcvCanOutputVer2* sRcvCanOutputVer2, stBoard* sBoard, stRvrIFRNS* sIFRNS);


int main()
{
    printf("Hello debug pack can\r\n");
    stRvrIFRNS sIFRNS;
    stBoard sBoard;

    memset(&sIFRNS, 0, sizeof(stRvrIFRNS));
    memset(&sBoard, 0, sizeof(stBoard));
    generateDebugData(&sBoard, &sIFRNS);    //data for check pack and unpack logic

    stRcvCanOutputVer2 sRcvCanOutputVer2;
    memset(&sRcvCanOutputVer2, 0, sizeof(stRcvCanOutputVer2));
    packRcvCanOutputVer2(&sRcvCanOutputVer2, &sBoard, &sIFRNS);

    stRvrIFRNS sIFRNS1;
    stBoard sBoard1;
    memset(&sIFRNS1, 0, sizeof(stRvrIFRNS));
    memset(&sBoard1, 0, sizeof(stBoard));

    unpackRcvCanOutputVer2(&sRcvCanOutputVer2, &sBoard1, &sIFRNS1);

    return 0;
}

void packRcvCanOutputVer2(stRcvCanOutputVer2* sRcvCanOutputVer2, stBoard* sBoard, stRvrIFRNS* sIFRNS)
{
    memset(sRcvCanOutputVer2, 0, sizeof(stRcvCanOutputVer2));
    sRcvCanOutputVer2->nWorkMode |= sIFRNS->nGRI;
    if (sIFRNS->fDOP > 63.0)
    {
        sRcvCanOutputVer2->nWorkMode |= (0x3F << 14);  
    }
    else
    {
        sRcvCanOutputVer2->nWorkMode |= (((uint8_t)(sIFRNS->fDOP) & 0x3F) << 14); 
    }
    sRcvCanOutputVer2->nWorkMode |= ((sIFRNS->cNum & 0x7) << 20);
    sRcvCanOutputVer2->nWorkMode |= ((sBoard->cWorkMode & 0x1) << 23);
    sRcvCanOutputVer2->nWorkMode |= ((sIFRNS->cNav & 0x1) << 25);
    sRcvCanOutputVer2->nWorkMode |= ((sIFRNS->cYNcor & 0x1) << 26);
    sRcvCanOutputVer2->nWorkMode |= ((sIFRNS->cIsCorExt & 0x1) << 27);
    sRcvCanOutputVer2->nWorkMode |= ((sIFRNS->cAutoGRI & 0x1) << 28);
    sRcvCanOutputVer2->nWorkMode |= ((sIFRNS->cIsGPT & 0x1) << 29);
    sRcvCanOutputVer2->nWorkMode |= ((sIFRNS->cMS & 0x1) << 30);
    sRcvCanOutputVer2->nWorkMode |= ((sBoard->cASUif & 0x1) << 31);

    sRcvCanOutputVer2->nMode |= (sIFRNS->mode[0] & 0x7);
    sRcvCanOutputVer2->nMode |= ((sIFRNS->mode[1] & 0x7) << 3);
    sRcvCanOutputVer2->nMode |= ((sIFRNS->mode[2] & 0x7) << 6);
    sRcvCanOutputVer2->nMode |= ((sIFRNS->mode[3] & 0x7) << 9);
    sRcvCanOutputVer2->nMode |= ((sIFRNS->mode[4] & 0x7) << 12); 
    sRcvCanOutputVer2->nMode |= ((sIFRNS->FlagEdgeGri & 0x1) << 15);

    sRcvCanOutputVer2->rcvB = (int32_t)(sIFRNS->curPos.rcvB * (1 << 28));
    sRcvCanOutputVer2->rcvL = (int32_t)(sIFRNS->curPos.rcvL * (1 << 28));

    sRcvCanOutputVer2->nTime = (sBoard->nTime * 1000) % 86400000;  
    sRcvCanOutputVer2->nTime |= (1 << 31);

    if (sBoard->cWorkMode == 1)
    {
        for(int i = 0; i < 5; ++i)
        {
            sRcvCanOutputVer2->TD[i] = (int16_t)(sIFRNS->TDif[i] / 0.25);
        }
    } 
    else
    {
        for(int i = 0; i < 5; ++i)
        {
            sRcvCanOutputVer2->TD[i] = (int16_t)(sIFRNS->TDif[i]);
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        sRcvCanOutputVer2->SNR[i] = ((uint8_t)sIFRNS->R[i] & 0x3F);
        sRcvCanOutputVer2->SNR[i] |= ((sIFRNS->nST[i] & 0x1) << 6);  
    }

    sRcvCanOutputVer2->hashCPU = sBoard->cpu_hash_hex;    
}

void unpackRcvCanOutputVer2(stRcvCanOutputVer2* sRcvCanOutputVer2, stBoard* p_sBoard, stRvrIFRNS* p_sIFRNS)
{
    p_sBoard->cWorkMode = (sRcvCanOutputVer2->nWorkMode >> 23) & 0x1;
    p_sBoard->cASUif = (sRcvCanOutputVer2->nWorkMode >> 31) & 0x1;
    p_sBoard->nTime = (sRcvCanOutputVer2->nTime & 0x3FFFFFFF) / 1000;
    p_sBoard->cpu_hash_hex = sRcvCanOutputVer2->hashCPU;
    p_sIFRNS->nGRI = sRcvCanOutputVer2->nWorkMode & 0x3FFF;
    p_sIFRNS->cNum = (sRcvCanOutputVer2->nWorkMode >> 20) & 0x7;
    p_sIFRNS->fDOP = (sRcvCanOutputVer2->nWorkMode >> 14) & 0x3F;
    p_sIFRNS->cNav = (sRcvCanOutputVer2->nWorkMode >> 25) & 0x1;
    p_sIFRNS->cYNcor = (sRcvCanOutputVer2->nWorkMode >> 26) & 0x1;
    p_sIFRNS->cIsCorExt = (sRcvCanOutputVer2->nWorkMode >> 27) & 0x1;
    p_sIFRNS->cAutoGRI = (sRcvCanOutputVer2->nWorkMode >> 28) & 0x1;
    p_sIFRNS->cIsGPT = (sRcvCanOutputVer2->nWorkMode >> 29) & 0x1;
    p_sIFRNS->cMS = (sRcvCanOutputVer2->nWorkMode >> 30) & 0x1;
    p_sIFRNS->mode[0] = sRcvCanOutputVer2->nMode & 0x7;
    p_sIFRNS->mode[1] = (sRcvCanOutputVer2->nMode >> 3) & 0x7;
    p_sIFRNS->mode[2] = (sRcvCanOutputVer2->nMode >> 6) & 0x7;
    p_sIFRNS->mode[3] = (sRcvCanOutputVer2->nMode >> 9) & 0x7;
    p_sIFRNS->mode[4] = (sRcvCanOutputVer2->nMode >> 12) & 0x7;
    p_sIFRNS->FlagEdgeGri = (sRcvCanOutputVer2->nMode >> 15) & 0x1;
    p_sIFRNS->curPos.rcvB = (double)sRcvCanOutputVer2->rcvB / (1 << 28);
    p_sIFRNS->curPos.rcvL = (double)sRcvCanOutputVer2->rcvL / (1 << 28);
    p_sIFRNS->TDif[0] = (float)sRcvCanOutputVer2->TD[0] * 0.25;
    p_sIFRNS->TDif[1] = (float)sRcvCanOutputVer2->TD[1] * 0.25;
    p_sIFRNS->TDif[2] = (float)sRcvCanOutputVer2->TD[2] * 0.25;
    p_sIFRNS->TDif[3] = (float)sRcvCanOutputVer2->TD[3] * 0.25;
    p_sIFRNS->TDif[4] = (float)sRcvCanOutputVer2->TD[4] * 0.25;
    p_sIFRNS->R[0] = sRcvCanOutputVer2->SNR[0] & 0x3F;
    p_sIFRNS->R[1] = sRcvCanOutputVer2->SNR[1] & 0x3F;
    p_sIFRNS->R[2] = sRcvCanOutputVer2->SNR[2] & 0x3F;
    p_sIFRNS->R[3] = sRcvCanOutputVer2->SNR[3] & 0x3F;
    p_sIFRNS->R[4] = sRcvCanOutputVer2->SNR[4] & 0x3F;
    p_sIFRNS->nST[0] = (sRcvCanOutputVer2->SNR[0] >> 6) & 0x1;
    p_sIFRNS->nST[1] = (sRcvCanOutputVer2->SNR[1] >> 6) & 0x1;
    p_sIFRNS->nST[2] = (sRcvCanOutputVer2->SNR[2] >> 6) & 0x1;
    p_sIFRNS->nST[3] = (sRcvCanOutputVer2->SNR[3] >> 6) & 0x1;
    p_sIFRNS->nST[4] = (sRcvCanOutputVer2->SNR[4] >> 6) & 0x1;
}

void putDataInCanStream(uint8_t* outStream, uint8_t* data, uint16_t size_data)
{
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

void generateDebugData(stBoard* p_sBoard, stRvrIFRNS* p_sIFRNS)
{
    p_sBoard->cWorkMode = 1;
    p_sBoard->cASUif = 1;
    p_sBoard->nTime = 3601;
    p_sBoard->cpu_hash_hex = 0xabd1;
    p_sIFRNS->nGRI = 5990;
    p_sIFRNS->cNum = 5;
    p_sIFRNS->fDOP = 4.67;
    p_sIFRNS->cNav = 1;
    p_sIFRNS->cYNcor = 1;
    p_sIFRNS->cIsCorExt = 1;
    p_sIFRNS->cAutoGRI = 1;
    p_sIFRNS->cIsGPT = 1;
    p_sIFRNS->cMS = 0;
    p_sIFRNS->mode[0] = 5;
    p_sIFRNS->mode[1] = 0;
    p_sIFRNS->mode[2] = 4;
    p_sIFRNS->mode[3] = 3;
    p_sIFRNS->mode[4] = 1;
    p_sIFRNS->FlagEdgeGri = 1;
    p_sIFRNS->curPos.rcvB = pi / 4;
    p_sIFRNS->curPos.rcvL = -pi / 8;
    p_sIFRNS->TDif[0] = 898.43;
    p_sIFRNS->TDif[1] = 1500.21;
    p_sIFRNS->TDif[2] = 1254.89;
    p_sIFRNS->TDif[3] = 2500.11;
    p_sIFRNS->TDif[4] = 2568.76;
    p_sIFRNS->R[0] = 32.45;
    p_sIFRNS->R[1] = 12.34;
    p_sIFRNS->R[2] = 15.56;
    p_sIFRNS->R[3] = 24.78;
    p_sIFRNS->R[4] = 55.66;
    p_sIFRNS->nST[0] = 1;
    p_sIFRNS->nST[1] = 1;
    p_sIFRNS->nST[2] = 1;
    p_sIFRNS->nST[3] = 1;
    p_sIFRNS->nST[4] = 1;
}




