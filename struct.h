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

typedef union stFlags
{
	struct{
		int8_t cYNcor 		:1; 
		int8_t cIsCorExt 	:1; 
		int8_t cAutoSel 	:1; 
		int8_t cPiData 		:1; 
		int8_t				:4;
	} bits;
	int8_t cData;
} stFlags;

typedef struct stPIif
{
	uint8_t				cWorkMode;														
	uint16_t			nGRI;			
	int8_t				cMS;			
	int8_t				cASUif;			
	int8_t				cASUf;			
	int8_t				mode[5];		
	int8_t				cNum;			
	int8_t				YN[5];			
	float				TDif[5];		
	float				TDcor[5];		
	stFlags   			cFlags;			
	int8_t				cNav;			
	float				fDOP;			
	double				fB;				
	double				fL;				
	float				fVb;			
	float				fVl;			
	double				fBc;			
	double				fLc;			
	float				fAux[5];		
	uint16_t 			nCheckSum;		
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

// структура описывающа€ параметры €чейки
typedef struct
{
	int8_t				cWorkMode;		
	uint32_t			nTime;					
	int8_t				cASUif;						
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
    float				fDOP;			
    uint8_t				cNum;			// number of station in current chain
    uint8_t				cNav;			
    uint8_t  			cYNcor;			
    uint8_t  			cIsCorExt;      // external params, '1'- correction is on, '0'-correction is off (receiving from parent board or from monitor).
    uint8_t				cAutoGRI;		// '1'-auto select GRI, '0'-otherwise
    uint8_t 			cIsGPT;         // external param, '1'-we have GTP (and GTP is valid), '0'-otherwise
	uint8_t				cMS;			
	uint8_t				mode[5];		// station mode
	uint8_t 			FlagEdgeGri;
    rcvPos              curPos;         // current receiver position
    double				TDif[5];		// pseudorange, TDif=delay3-ED
    float				R[5];			// SNR
    int32_t 			nST[5];			// '1'-use for POS, '0'-otherwise
}stRvrIFRNS;

#pragma pack(pop)

#endif  //STRUCT_H