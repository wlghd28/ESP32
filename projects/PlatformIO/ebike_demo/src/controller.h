#ifndef _CONTROLLER
#define _CONTROLLER

#include "comm_protocol.h"

class Queue;
class HardwareSerial;
class Controller
{
public:
    Controller();
    ~Controller();

private: // member
    Queue* m_queue;
    HardwareSerial* m_serial;

    uint16_t  m_ThreadCmd_SendData;
    uint16_t  m_ThreadClock_SendData;          // 100ms 고정
    uint16_t  m_ThreadPeriod_SendData_Status;  // Status Command 보내는 주기 (10 고정)
    uint16_t  m_ThreadCount_SendData_Status;   // Status Command 보내는 주기에 대한 카운팅

    // protocol data
    STC_COMM_CONTROLLER_RECV m_STC_Comm_RecvData;
    STC_COMM_CONTROLLER_SEND m_STC_Comm_SendData;

    // serial_protocol
    STC_MAIN m_STC_MAIN_RecvBuf;
    STC_MAIN m_STC_MAIN_SendBuf;
    // STC_TCP_CMD_PWM m_TCP_CMD_PWM;
    // STC_TCP_CMD_DAC m_TCP_CMD_DAC;

public: // methods
    void Init();
    void SetCmd_SendData(uint16_t cmd);
    void OnThread_SendData();
    void RecvData();
    int SendData(uint8_t cmd);
    void ProcessCommbuf(uint8_t* TempBuf, uint8_t* ByteStuffingBuf);
    void ByteStuffing_Frame_MOSI(STC_COMM_CONTROLLER_SEND* SendData, uint8_t* frame, uint8_t* index_frame, uint16_t size);
    uint16_t CalCRC(uint8_t *cBuf, uint16_t nLen, uint16_t nSeed);
    
    const STC_MAIN* GetMainRecvBuf();
    STC_MAIN* GetMainSendBuf();
};

extern Controller *g_controller;
#endif