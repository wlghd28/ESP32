#include "controller.h"
#include "custom_ui.h"
#include "queue.h"
#include "HardwareSerial.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define SOCKET_ERROR -1

Controller::Controller()
    : m_serial(&Serial1),
      m_ThreadCmd_SendData(SER_CMD_TEST1),
      m_ThreadClock_SendData(100),
      m_ThreadPeriod_SendData_Status(10),
      m_ThreadCount_SendData_Status(0)
{
    Init();
}

Controller::~Controller()
{
    if(m_queue) delete m_queue;
}

void Controller::Init()
{
    m_queue = new Queue();

    memset(&m_STC_Comm_RecvData, 0, sizeof(m_STC_Comm_RecvData));
    memset(&m_STC_Comm_SendData, 0, sizeof(m_STC_Comm_SendData));
    memset(&m_STC_MAIN_RecvBuf, 0, sizeof(m_STC_MAIN_RecvBuf));
    memset(&m_STC_MAIN_SendBuf, 0, sizeof(m_STC_MAIN_SendBuf));
}

void Controller::OnThread_SendData()
{
    switch(m_ThreadCmd_SendData)
    {
    case SER_CMD_RESET:
        SendData(SER_CMD_RESET);
        break;

    case SER_CMD_TEST1:
        SendData(SER_CMD_TEST1);
        m_ThreadCmd_SendData = SER_CMD_TEST3;
        m_ThreadCount_SendData_Status = 0;
        break;

    case SER_CMD_TEST2:
        SendData(SER_CMD_TEST2);
        m_ThreadCmd_SendData = SER_CMD_TEST3;
        m_ThreadCount_SendData_Status = 0;
        break;

    case SER_CMD_TEST3:   
        if(++m_ThreadCount_SendData_Status >= m_ThreadPeriod_SendData_Status)
        {
            SendData(SER_CMD_TEST3);
            m_ThreadCount_SendData_Status = 0;
        }     
        break;


    default:
        break;
    }
}

void Controller::SetCmd_SendData(uint16_t cmd)
{
    m_ThreadCmd_SendData = cmd;
}

void Controller::RecvData()
{
    if(m_serial->available() > 0)
    {
        m_queue->Insert(m_serial->read());
    }
}

int Controller::SendData(uint8_t cmd)
{
    int iRet = SOCKET_ERROR;

    switch (cmd)
    {
    case EN_SER_CMD::SER_CMD_RESET:
    {
        uint8_t frame[FRAME_MAXBUFSIZE] = { 0, };
        uint8_t index_frame = 0;
        uint16_t crc = 0;
        size_t size = sizeof(cmd);

        memset(&(m_STC_Comm_SendData.Frame), 0, sizeof(m_STC_Comm_SendData.Frame));
        m_STC_Comm_SendData.Frame.Start = CONTROLLER_START_BIT;
        m_STC_Comm_SendData.Frame.Message = new uint8_t[size];
        memset(m_STC_Comm_SendData.Frame.Message, 0, size);
        memcpy(m_STC_Comm_SendData.Frame.Message, &cmd, sizeof(cmd));
        crc = CalCRC((UINT8*)m_STC_Comm_SendData.Frame.Message, (UINT16)size, CRC_SEED);
        memcpy(&(m_STC_Comm_SendData.Frame.Crc), &crc, sizeof(crc));
        m_STC_Comm_SendData.Frame.Stop = CONTROLLER_STOP_BIT;
        ByteStuffing_Frame_MOSI(&m_STC_Comm_SendData, frame, &index_frame, size);

        m_serial->write(reinterpret_cast<const char*>(frame), ++index_frame);
    }
        break;

    case EN_SER_CMD::SER_CMD_TEST1:
    {
        uint8_t frame[FRAME_MAXBUFSIZE] = { 0, };
        uint8_t index_frame = 0;
        uint16_t crc = 0;
        size_t size = sizeof(cmd) + sizeof(m_STC_MAIN_SendBuf.Status.State);

        memset(&(m_STC_Comm_SendData.Frame), 0, sizeof(m_STC_Comm_SendData.Frame));
        m_STC_Comm_SendData.Frame.Start = CONTROLLER_START_BIT;
        m_STC_Comm_SendData.Frame.Message = new uint8_t[size];
        memset(m_STC_Comm_SendData.Frame.Message, 0, size);
        memcpy(m_STC_Comm_SendData.Frame.Message, &cmd, sizeof(cmd));
        memcpy(m_STC_Comm_SendData.Frame.Message + sizeof(cmd), &m_STC_MAIN_SendBuf.Status.State, sizeof(m_STC_MAIN_SendBuf.Status.State));
        crc = CalCRC((UINT8*)m_STC_Comm_SendData.Frame.Message, (UINT16)size, CRC_SEED);
        memcpy(&(m_STC_Comm_SendData.Frame.Crc), &crc, sizeof(crc));
        m_STC_Comm_SendData.Frame.Stop = CONTROLLER_STOP_BIT;
        ByteStuffing_Frame_MOSI(&m_STC_Comm_SendData, frame, &index_frame, size);

        m_serial->write(reinterpret_cast<const char*>(frame), ++index_frame);
    }
        break;

    case EN_SER_CMD::SER_CMD_TEST2:
    {
        uint8_t frame[FRAME_MAXBUFSIZE] = { 0, };
        uint8_t index_frame = 0;
        uint16_t crc = 0;
        size_t size = sizeof(cmd);

        memset(&(m_STC_Comm_SendData.Frame), 0, sizeof(m_STC_Comm_SendData.Frame));
        m_STC_Comm_SendData.Frame.Start = CONTROLLER_START_BIT;
        m_STC_Comm_SendData.Frame.Message = new uint8_t[size];
        memset(m_STC_Comm_SendData.Frame.Message, 0, size);
        memcpy(m_STC_Comm_SendData.Frame.Message, &cmd, sizeof(cmd));
        crc = CalCRC((UINT8*)m_STC_Comm_SendData.Frame.Message, (UINT16)size, CRC_SEED);
        memcpy(&(m_STC_Comm_SendData.Frame.Crc), &crc, sizeof(crc));
        m_STC_Comm_SendData.Frame.Stop = CONTROLLER_STOP_BIT;
        ByteStuffing_Frame_MOSI(&m_STC_Comm_SendData, frame, &index_frame, size);

        m_serial->write(reinterpret_cast<const char*>(frame), ++index_frame);
    }
        break;

    case EN_SER_CMD::SER_CMD_TEST3:
    {
        uint8_t frame[FRAME_MAXBUFSIZE] = { 0, };
        uint8_t index_frame = 0;
        uint16_t crc = 0;
        size_t size = sizeof(cmd);

        memset(&(m_STC_Comm_SendData.Frame), 0, sizeof(m_STC_Comm_SendData.Frame));
        m_STC_Comm_SendData.Frame.Start = CONTROLLER_START_BIT;
        m_STC_Comm_SendData.Frame.Message = new uint8_t[size];
        memset(m_STC_Comm_SendData.Frame.Message, 0, size);
        memcpy(m_STC_Comm_SendData.Frame.Message, &cmd, sizeof(cmd));
        crc = CalCRC((UINT8*)m_STC_Comm_SendData.Frame.Message, (UINT16)size, CRC_SEED);
        memcpy(&(m_STC_Comm_SendData.Frame.Crc), &crc, sizeof(crc));
        m_STC_Comm_SendData.Frame.Stop = CONTROLLER_STOP_BIT;
        ByteStuffing_Frame_MOSI(&m_STC_Comm_SendData, frame, &index_frame, size);

        m_serial->write(reinterpret_cast<const char*>(frame), ++index_frame);
    }
        break;

    default:
        break;
    }

    if(m_STC_Comm_SendData.Frame.Message)
    {
        delete[] m_STC_Comm_SendData.Frame.Message;
        m_STC_Comm_SendData.Frame.Message = nullptr;
    }

    return iRet;
}


void Controller::ProcessCommbuf(uint8_t* TempBuf, uint8_t* ByteStuffingBuf)
{
    // uint8_t TempBuf[MAXBUFSIZE]; // Temp Data array
    // uint8_t ByteStuffingBuf[MAXBUFSIZE]; // ByteStuffingBuf Data array

    uint16_t bufsize = m_queue->GetDataLength();

    // 버퍼에 데이터가 없는 경우
    if(!bufsize) return;

    uint8_t start_bit_count = 0;
    uint8_t stop_bit_count = 0;
    uint8_t start_bit_index = 0;
    uint8_t stop_bit_index = 0;
    uint16_t index = 0;
    uint16_t index_bytestuffing = 0;
    uint16_t index_message = 0;
    uint16_t index_crc = 0;
    uint16_t framesize = 0;

    // 버퍼 메모리 초기화
    memset(TempBuf, 0, bufsize * sizeof(uint8_t));
    //memset(TempBuf, 0, MAXBUFSIZE * sizeof(uint8_t));

    // 버퍼 메모리에 메모리 복사 및 Start, Stop 비트 조사
    index = m_queue->GetTail();
    for(int i = 0; i < bufsize; i++)
    {
        TempBuf[i] = m_queue->GetData(index++);
        if(TempBuf[i] == CONTROLLER_START_BIT)
        {
            start_bit_count++;
            start_bit_index = i;
            break;
        }

        m_queue->Delete(1);
        index &= (MAXBUFSIZE - 1);
    }

    // start_bit_count < 1 이면 무시
    if(start_bit_count < 1) return;

    for(int i = start_bit_index + 1; i < bufsize; i++)
    {
        TempBuf[i] = m_queue->GetData(index++);
        if(TempBuf[i] == CONTROLLER_STOP_BIT)
        {
            stop_bit_count++;
            stop_bit_index = i;
            break;
        }
        else if(TempBuf[i] == CONTROLLER_START_BIT)
        {
            m_queue->Delete((uint16_t)(i - start_bit_index));
            break;
        }

        index &= (MAXBUFSIZE - 1);
    }

    // stop_bit_count < 1이면 무시
    if(stop_bit_count < 1) return;

    // Data ByteStuffing
    memset(ByteStuffingBuf, 0, sizeof(ByteStuffingBuf));

    framesize = stop_bit_index - start_bit_index + 1;
    index = start_bit_index + 1;

    ByteStuffingBuf[index_bytestuffing++] = TempBuf[start_bit_index];
    for(int i = 0; i < framesize - 2; i++)
    {
        if(TempBuf[index + i] == 0x7D)
            ByteStuffingBuf[index_bytestuffing++] = (TempBuf[index + (++i)] ^ HDLC_BYTE_STUFFING);
        else
            ByteStuffingBuf[index_bytestuffing++] = TempBuf[index + i];
    }
    ByteStuffingBuf[index_bytestuffing++] = TempBuf[stop_bit_index];

    // Frame 구조체에 메모리 복사
    // Start
    m_STC_Comm_RecvData.Frame.Start = ByteStuffingBuf[0];

    // Message
    memset(m_STC_Comm_RecvData.Frame.Message, 0, sizeof(m_STC_Comm_RecvData.Frame.Message));
    for(uint16_t i = 1; i < index_bytestuffing - 3; i++)
    {
        m_STC_Comm_RecvData.Frame.Message[index_message++] = ByteStuffingBuf[i];
    }

    // Crc
    for(uint16_t i = index_bytestuffing - 3; i < index_bytestuffing - 1; i++)
    {
        m_STC_Comm_RecvData.Frame.Crc[index_crc++] = ByteStuffingBuf[i];
    }

    // Stop
    m_STC_Comm_RecvData.Frame.Stop = ByteStuffingBuf[index_bytestuffing - 1];

    // Crc Check
    uint16_t crc_recv = 0, crc_calc = 0;
    memcpy(&crc_recv, m_STC_Comm_RecvData.Frame.Crc, sizeof(crc_recv));
    crc_calc = CalCRC((uint8_t*)m_STC_Comm_RecvData.Frame.Message, index_message, CRC_SEED);

    if(crc_recv == crc_calc)
    {
        // 데이터 파싱
        uint8_t cmd = 0;
        memcpy(&cmd, m_STC_Comm_RecvData.Frame.Message, sizeof(cmd));

        switch(cmd)
        {
        case EN_SER_CMD::SER_CMD_RESET:
            break;

        case EN_SER_CMD::SER_CMD_TEST1:
            memcpy(&(m_STC_MAIN_RecvBuf), m_STC_Comm_RecvData.Frame.Message, sizeof(STC_MAIN));
            break;

        case EN_SER_CMD::SER_CMD_TEST2:
            memcpy(&(m_STC_MAIN_RecvBuf), m_STC_Comm_RecvData.Frame.Message, sizeof(STC_MAIN));
            break;

        case EN_SER_CMD::SER_CMD_TEST3:
            memcpy(&(m_STC_MAIN_RecvBuf), m_STC_Comm_RecvData.Frame.Message, sizeof(STC_MAIN));
            break;

        default:
            break;
        }
    }

    m_queue->Delete((uint16_t)framesize);
}

void Controller::ByteStuffing_Frame_MOSI(STC_COMM_CONTROLLER_SEND* SendData, uint8_t* frame, uint8_t* index_frame, uint16_t size)
{
    // Start
    frame[(*index_frame)++] = SendData->Frame.Start;

    // Message
    for(uint16_t i = 0; i < size; i++)
    {
        if(SendData->Frame.Message[i] == 0x7E || SendData->Frame.Message[i] == 0x7D || SendData->Frame.Message[i] == 0x7F)
        {
            frame[(*index_frame)++] = 0x7D;
            frame[(*index_frame)++] = (SendData->Frame.Message[i] ^ HDLC_BYTE_STUFFING);
        }
        else frame[(*index_frame)++] = SendData->Frame.Message[i];
    }

    // Crc
    for(uint16_t i = 0; i < sizeof(SendData->Frame.Crc); i++)
    {
        if((uint8_t)SendData->Frame.Crc[i] == 0x7E || (uint8_t)SendData->Frame.Crc[i] == 0x7D || (uint8_t)SendData->Frame.Crc[i] == 0x7F)
        {
            frame[(*index_frame)++] = 0x7D;
            frame[(*index_frame)++] = ((uint8_t)SendData->Frame.Crc[i] ^ HDLC_BYTE_STUFFING);
        }
        else frame[(*index_frame)++] = (uint8_t)SendData->Frame.Crc[i];
    }

    // Stop
    frame[(*index_frame)] = SendData->Frame.Stop;
}

uint16_t Controller::CalCRC(uint8_t *cBuf, uint16_t nLen, uint16_t nSeed)
{
    UINT16 i, nRet = nSeed;

    for (i=0; i<nLen; i++)
        nRet = (nRet << 8) ^ CRC_TABLE[((nRet >>8) & 0xff) ^ cBuf[i]];

    return nRet;
}

const STC_MAIN* Controller::GetMainRecvBuf()
{
    return (const STC_MAIN*)(&m_STC_MAIN_RecvBuf);
}

STC_MAIN* Controller::GetMainSendBuf()
{   
    return (STC_MAIN*)(&m_STC_MAIN_SendBuf);
}