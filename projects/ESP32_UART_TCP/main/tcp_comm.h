#ifndef _HEADER_H_
#define _HEADER_H_

#define PWM_USED_LEN				3
#define ECD_USED_LEN				3
#define DAC_CH_LEN					4

#define AD_TEMP_LEN					10

#define DIV_BMS_VOLT				100
#define DIV_BMS_CURR				100
#define DIV_TEMP					10
#define DIV_RES_TIME				500	//mSec

enum
{
	CPU_MASTER, 
	CPU_SLAVE, 
	CPU_LEN
};

enum
{
    ADC_PT102,  //1st               /10
    ADC_FW301,  //Water             /100
    ADC_PT103,  //2nd               /100
    ADC_CS301,  //Conductivity      /10
    ADC_FW201,  //Air
    ADC_MFM101, //Hydrogen          /10
    ADC_REF,
    ADC_V_STACK,
    ADC_PT101,  //Tank              /10
    ADC_H501,   //Leak              /1000
    ADC_RSV_0,
    ADC_CURR,
    ADC_V_IN,
    ADC_FAN501,
    ADC12_LEN,
};

enum
{
	ADC_TEMP_INTERNAL,
	ADC_TEMP_EXTERNAL,
	ADC_TEMP_STACK_IN, 
	ADC_TEMP_STACK_OUT,
	ADC_TEMP_AIR_IN, 
	ADC_TEMP_AIR_OUT,
	ADC_TEMP_WATRE_IN, 
	ADC_TEMP_WATRE_OUT,
	ADC_TEMP_H_TANK, 
	ADC_TEMP_BOARD, 
	ADC_TEMP_LEN
};

enum
{
	ADC16_VSTACK, 
	DC16_RSV_0,
	ADC16_LEN
};


typedef union
{
	uint8_t Val;
    struct
    {
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
    } Bits;
} UNI_BYTE_VAL;

typedef union
{
    uint32_t Val;
    struct
    {
        uint8_t H501 : 1;
        uint8_t CanBMS : 1;
        uint8_t CanDcDc : 1;
        uint8_t CanBlower : 1;
        uint8_t Stack : 3;
        uint8_t FanCurr : 1;
        uint8_t Ecd0 : 1;
        uint8_t Ecd1 : 1;
        uint8_t MFM101 : 1;
        uint8_t FW201 : 1;
        uint8_t FW301 : 1;
        uint8_t PT101 : 2;
        uint8_t PT102 : 1;
        uint8_t PT103 : 2;
        uint8_t CS301 : 1;
        uint8_t Level : 1;
        uint8_t TC101 : 1;
        //uint16_t Rsv : 11;
    } b;
} UNI_ALARM;


typedef union
{
	uint16_t Val;
    struct
    {
		uint8_t EnIG : 1;
		uint8_t Load : 1;
		uint8_t V48 : 1;
		uint8_t V24 : 1;
		uint8_t V12 : 1;
		uint8_t S101 : 1;
		uint8_t S102 : 1;
		uint8_t S201 : 1;
		uint8_t S103 : 1;
		uint8_t S401 : 1;
		uint8_t P301 : 1;
		uint8_t P401 : 1;
		uint8_t Rsv0 : 1;	//Rsv Out0
		uint8_t Rsv1 : 1;	//Rsv Out1
		uint8_t Rsv : 1;
		uint8_t Buf : 1;
    } b;
} UNI_EN_MASTER;

typedef union
{
	uint8_t Val;
    struct
    {
		uint8_t Led0 : 1;
		uint8_t Led1 : 1;
		uint8_t Led2 : 1;
		uint8_t Led3 : 1;
		uint8_t Fan0 : 1;
		uint8_t Fan1 : 1;
		uint8_t Rsv : 2;
    } b;
} UNI_EN_SLAVE;

typedef union
{
	uint8_t Val;
    struct
    {
		uint8_t Fault : 1;
		uint8_t Run : 1;
		uint8_t LowV : 1;
		uint8_t HghV : 1;
		uint8_t OverCurr : 1;
		uint8_t OverTemp : 1;
		uint8_t Rsv : 2;
    } b;
} UNI_EN_DC_STATE;

typedef union
{
	uint8_t Val;
    struct
    {
		uint8_t BMS : 1;
		uint8_t Charge : 1;
		uint8_t Discharge : 1;
		uint8_t Fault : 1;
		uint8_t Rsv : 4;
    } b;
} UNI_EN_BMS_STATE;

typedef union
{
	uint16_t Val;
    struct
    {
		uint8_t Balancing : 1;
		uint8_t Discharge : 1;
		uint8_t Charge : 1;
		uint8_t FullCharge : 1;
		uint8_t FullDischarge : 1;
		uint8_t OverVolt : 1;
		uint8_t UnderVolt : 1;
		uint8_t OverVoltProtect : 1;
		uint8_t UnderVoltProtect : 1;
		uint8_t OverCargeCurrProtect : 1;
		uint8_t OverDischargeCurrProtect : 1;
		uint8_t DischargeHighTempProtect : 1;
		uint8_t ChargeHighTempProtect : 1;
		uint8_t ChargeLowTempProtect : 1;
		uint8_t Fault : 1;
		uint8_t Error : 1;
    } b;
} UNI_EN_BMS_ALARM;

#pragma pack(1)
typedef struct
{
	uint8_t IP[4], NetMask[4], Gateway[4];
} ETH_INFO;

typedef struct
{
	uint8_t Year, Month, Day;
} DATE_BUF;

typedef struct
{
    uint8_t Ver[CPU_LEN];     // /10
	DATE_BUF Date[CPU_LEN];
    uint8_t State;
    UNI_ALARM Alarm;
	UNI_BYTE_VAL Sw;
	UNI_BYTE_VAL Level;	//[3..0] : Level, [4] : IG State
    int16_t TempCPU;
    int16_t Temp[AD_TEMP_LEN];
    uint16_t Adc[ADC12_LEN];
    uint16_t Adc16[ADC16_LEN];
    uint8_t Ecd[ECD_USED_LEN];
    int16_t Blower;					//Blower RPM
	UNI_EN_DC_STATE StateDC;
    //uint16_t VoltDC[2], CurrDC[2];	//In, Out       /10
    int16_t VoltDC[2], CurrDC[2];	//In, Out       /10
    uint16_t TempDC;                 // /10
	UNI_EN_BMS_STATE StateBMS;
	UNI_EN_BMS_ALARM AlarmBMS;
    uint16_t VoltBat;            // /100
    uint16_t CurrBat[2];	//Charge, Discharge     /100
    uint8_t SocBat;
    uint16_t TempCell[2];        // /10
    uint16_t DiffCell;       // /1000
} STATUS_BUF;

typedef struct 
{
    uint8_t Mode;			//Auto : 0, Manual : 1, etc..
    uint8_t ResTime;         //Response Time
    uint8_t PurgeTime;
	UNI_EN_SLAVE EnS;
	UNI_EN_MASTER EnM;
    uint8_t Pwm[PWM_USED_LEN];
    uint8_t Dac[DAC_CH_LEN];
    int16_t Blower;		//Blower RPM
    uint16_t CurrDC;		//DC-DC Current Max 200, 0.1A Div
	ETH_INFO IP;
    uint8_t ServerIP[4];
    uint16_t Port[2];		//TCP, UDP
} CTRL_BUF;

typedef struct
{
    uint16_t Cmd;
	STATUS_BUF State;
	CTRL_BUF Ctrl;
} PMFC_BUF;
#pragma pack()

extern PMFC_BUF g_PMFC_RecvData;
extern PMFC_BUF g_PMFC_SendData;

#endif