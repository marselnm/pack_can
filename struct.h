#ifndef STRUCT_H
#define STRUCT_H

#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
	int32_t   			nHemS;			// hemisphere, N or E - positive, S or W - negative
	uint32_t			nDeg;			// degrees, 0..+90 for latitude, 0..+180 for longitude
	uint32_t			nMin;			// minutes, 0..59
	float        		fSec;  			// seconds, 0..59.999999
}stGrMS;

// ����� ��������� �������
typedef union stFlags
{
	struct{
		int8_t cYNcor 		:1; // ���������� ���������
		int8_t cIsCorExt 	:1; // ������� ��������� (���������)
		int8_t cAutoSel 	:1; // ������� ���������� �������
		int8_t cPiData 		:1; // ������� ��������� ������ ���
		int8_t				:4;
	} bits;
	int8_t cData;
} stFlags;

// ��������� ��� �������� � ����� ���������� ��
typedef struct stPIif
{
	uint8_t				cWorkMode;		// '1'-����� ����.-�����.,'2'-����� �����., '3'-���� ����.-�����., '4'-���� �����.,
										// (1<<6) - ���� ���� ����; (1<<5) - ���� ��������� ���� ����� ��������� �����
										// (1<<7) - ���� ��������������� ������ �������("1" -�������)
	uint16_t			nGRI;			// ����� �������
	int8_t				cMS;			// '0'-������������, '1'-���������
	int8_t				cASUif;			// '1'-��� ����� ����������, '0'-��� ����� �� ����������
	int8_t				cASUf;			// '1'-��� ���� ����������, '0'-��� ���� �� ����������
	int8_t				mode[5];		// ������ ������ �������
	int8_t				cNum;			// ���������� ������� � ����
	int8_t				YN[5];			// ������� �������
	float				TDif[5];		// ���������� ��������������� �� �����
	float				TDcor[5];		// ���������������� ��������������� �� �����
	stFlags   			cFlags;			// ��������� �����
	int8_t				cNav;			// ������� ������� ������������� ������ '1'-������, '0'-�� ������
	float				fDOP;			// �������������� ������
	double				fB;				// ������ ������� � �������� �� �����
	double				fL;				// ������� ������� � �������� �� �����
	float				fVb;			// ��������
	float				fVl;			// ��������
	double				fBc;			// ������ ������� � �������� �� ��������
	double				fLc;			// ������� ������� � �������� �� ��������
	float				fAux[5];		// �������������� ��������� ��� ��������� ����������
	uint16_t 			nCheckSum;		// ����������� ����� ������
} stPIif;

#pragma pack(pop)

#endif  //STRUCT_H