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

struct stRcvCanOutputVer2
{
	uint32_t        nWorkMode;  // 0..13 - GRI number 
								// 14..19 - gdop
								// 20..22 - num station in chain
                                // 23 - system mode, '0'-ifrns, '1'-frns
								// 24 - '1'-dalnomer mode, '0'-otherwise
								// 25 - flag NAV, '1'- NAV, '0'-otherwise
								// 26 - internal correction, '1'-on, '0'-off
								// 27 - external correction, '1'-on, '0'-off
								// 28 - auto gri, '1'-auto mode for choosing gri, '0'-otherwise
								// 29 - gpt, '1'-we have gpt, '0'-otherwise
								// 30 - type of gri, '0'-stationary, '1'-mobile
								// 31 - check asu, '1'-asu is good, '0'-otherwise
								
	uint16_t        nMode;      //  0.. 2 - station Lead   mode
								//  3.. 5 - sttaion Slave1 mode
								//  6.. 8 - station Slave2 mode
								//  9..11 - station Slave3 mode
								// 12..14 - station Slave4 mode
								// 15 - zone, '1'-bad zone, '0'-othersiwe
								
    int32_t			rcvB;		// latitude Ifrns/Frns,  scale factor is *(1<<28) radian
	int32_t			rcvL;		// longitude Ifrns/Frns, scale factor is *(1<<28) radian
	
	uint32_t        nTime;      // 0..29 - time im ms, module one day [0..86399999] or module one week [0..604799999] 
								// 30 - '1' - time sync with gnss time, '0' - internal rcv time
                                // 31 - '0' module one day, '1' - module one week (by default module one day)     
       	   
    int16_t         TD[5];      // pseudorange, scale factor is 0.32 musec for ifrns, 1 musec for frns

    uint8_t         SNR[5];     // 0..5 - snr in db, scale factor is 1
                                // 6 - use in NT or not, '1'-use, '0'-not used
                                // 7 - staion ban or not '1'-on, '0'-off
    uint16_t 		hashCPU;    // firmware version, use printf("%x", hashCPU) and you we'll have last 4 symbol from git hash commit
    uint8_t         padding;    // reserved field                                                                                  
};

// структура описывающая параметры ячейки
typedef struct
{
	int8_t				cWorkMode;		// '1'-ИФРНС разн.-дальн., '2'-ИФРНС дальн., '3'-ФРНС разн.-дальн., '4'-ФРНС дальн.
	uint32_t			nTime;			// время работы изделия
	uint32_t			nLD;			// число ретаймингов LD
	int8_t				cASUif;			// '1'-АСУ ИФРНС подключено, '0'-АСУ ИФРНС не подключено
	int8_t				cASUf;			// '1'-АСУ ФРНС подключено, '0'-АСУ ФРНС не подключено
	uint8_t				cPCB;			// версия платы
	uint16_t			nCPU;			// версия программы для процессора
	uint16_t			nFPGA;			// версия программы для ПЛИСа
	uint32_t			nTimeCorrOff;	// время для отключения коррекции
    const char*         hardware_ver;   // version board hardware
    const char*         cpu_hash;       // git hash version
    int                 cpu_diff;       // if we don't have changes 0, otherwise 1
    uint16_t            cpu_hash_hex;   // this is small part of hash which putted in uint16_t as a hex data
}stBoard;

//initial state for iteration NT algorithm
typedef enum 
{
    eCenterArea = 1,
    eFromFlash = 2,
    eFromUser = 3,
    eFromGNSS = 4
}eStartNT;

typedef struct rcvPos
{
    eStartNT initPos;
    unsigned char valid;
    unsigned char wasCalc;
    double rcvB;
    double rcvL;
}rcvPos; 

// main receiver struct
typedef struct
{
    uint16_t			nGRI;			// group repetion interval for current chain
    float				fDOP;			// геометрический фактор
    uint8_t				cNum;			// number of station in current chain
    uint8_t				cNav;			// признак решения навигационной задачи '1'-решена, '0'-не решена
    uint8_t  			cYNcor;			// внутренний признак коррекции '1'- включена, '0'- выключена. Переключается относительно внешнего и состояния приема спутника
    uint8_t  			cIsCorExt;      // external params, '1'- correction is on, '0'-correction is off (receiving from parent board or from monitor).
    uint8_t				cAutoGRI;		// '1'-auto select GRI, '0'-otherwise
    uint8_t 			cIsGPT;         // external param, '1'-we have GTP (and GTP is valid), '0'-otherwise
	uint8_t				cMS;			// '0'-стационарная, '1'-мобильная, '2'- взятая из структуры
	uint8_t				mode[5];		// station mode
	uint8_t 			FlagEdgeGri;
    rcvPos              curPos;         // current receiver position
    double				TDif[5];		// pseudorange, TDif=delay3-ED
    float				R[5];			// SNR
    int32_t 			nST[5];			// '1'-use for POS, '0'-otherwise
}stRvrIFRNS;

#pragma pack(pop)

#endif  //STRUCT_H