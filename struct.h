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

// Флаги состояния системы
typedef union stFlags
{
	struct{
		int8_t cYNcor 		:1; // внутренняя коррекция
		int8_t cIsCorExt 	:1; // внешняя коррекция (квитанция)
		int8_t cAutoSel 	:1; // признак автовыбора цепочки
		int8_t cPiData 		:1; // признак получения данных АПТ
		int8_t				:4;
	} bits;
	int8_t cData;
} stFlags;

// структура для передачи в плату интерфейса ПИ
typedef struct stPIif
{
	uint8_t				cWorkMode;		// '1'-ИФРНС разн.-дальн.,'2'-ИФРНС дальн., '3'-ФРНС разн.-дальн., '4'-ФРНС дальн.,
										// (1<<6) - флаг край зоны; (1<<5) - флаг активации меню ввода начальной точки
										// (1<<7) - флаг автоматического выбора цепочек("1" -активно)
	uint16_t			nGRI;			// номер цепочки
	int8_t				cMS;			// '0'-стационарная, '1'-мобильная
	int8_t				cASUif;			// '1'-АСУ ИФРНС подключено, '0'-АСУ ИФРНС не подключено
	int8_t				cASUf;			// '1'-АСУ ФРНС подключено, '0'-АСУ ФРНС не подключено
	int8_t				mode[5];		// режимы работы станций
	int8_t				cNum;			// количество станций в цепи
	int8_t				YN[5];			// признак станций
	float				TDif[5];		// измеренные псевдодальности по ИФРНС
	float				TDcor[5];		// скоректированные псевдодальности по ИФРНС
	stFlags   			cFlags;			// различные флаги
	int8_t				cNav;			// признак решения навигационной задачи '1'-решена, '0'-не решена
	float				fDOP;			// геометрический фактор
	double				fB;				// широта объекта в радианах по ИФРНС
	double				fL;				// долгота объекта в радианах по ИФРНС
	float				fVb;			// скорость
	float				fVl;			// скорость
	double				fBc;			// широта объекта в радианах по спутнику
	double				fLc;			// долгота объекта в радианах по спутнику
	float				fAux[5];		// дополнительные параметры для различных переменных
	uint16_t 			nCheckSum;		// контрольная сумма пакета
} stPIif;

#pragma pack(pop)

#endif  //STRUCT_H