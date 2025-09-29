#include "custom_task.h"
#include "custom_ui.h"
#include "controller.h"
#include <Arduino.h>

Custom_Task::Custom_Task(Custom_UI *custom_ui, Controller* controller)
    : m_custom_ui(custom_ui),
      m_controller(controller)
{
    Init();
}

Custom_Task::~Custom_Task()
{

}

void Custom_Task::Init()
{
    xTaskCreatePinnedToCore(Custom_Task_ScalingData, "Custom_Task_ScalingData", 1024, m_custom_ui, 1, NULL, 1);
    delay(500);
    xTaskCreatePinnedToCore(Custom_Task_RecvData, "Custom_Task_RecvData", 2048, m_controller, 1, NULL,  0); 
    delay(500); 
    xTaskCreatePinnedToCore(Custom_Task_SendData, "Custom_Task_SendData", 2048, m_controller, 2, NULL,  0); 
    delay(500); 
    xTaskCreatePinnedToCore(Custom_Task_ProcessCommbuf, "Custom_Task_ProcessCommbuf", 4096, m_controller, 3, NULL,  0);  
    delay(500); 
}

void Custom_Task::Custom_Task_ScalingData(void *param)
{
    while(1)
    {
        ((Custom_UI *)param)->ScalingData();
        delay(500);
    }
}

void Custom_Task::Custom_Task_RecvData(void *param)
{
    while(1)
    {
        ((Controller *)param)->RecvData();
        delay(1);
    }
}

void Custom_Task::Custom_Task_SendData(void *param)
{
    while(1)
    {
        ((Controller *)param)->OnThread_SendData();
        delay(100);
    }
}

void Custom_Task::Custom_Task_ProcessCommbuf(void *param)
{
    uint8_t TempBuf[1024]; // Temp Data array
    uint8_t ByteStuffingBuf[1024]; // ByteStuffingBuf Data array

    while(1)
    {
        ((Controller *)param)->ProcessCommbuf(TempBuf, ByteStuffingBuf);
        delay(10);
    }
}