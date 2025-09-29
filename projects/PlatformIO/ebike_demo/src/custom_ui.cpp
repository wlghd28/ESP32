#include "custom_ui.h"
#include "controller.h"
#include "comm_protocol.h"
#include "ui.h"
#include <Arduino.h>
#include <stdlib.h>
#include <Arduino_GFX_Library.h>

Custom_UI::Custom_UI(Controller *controller)
    : m_controller(controller)
    , m_LCD_Duty(255)
    , m_Debug_Status(false)
{
    Init();
}

Custom_UI::~Custom_UI()
{

}

// 멤버함수
void Custom_UI::Init()
{
    CreateTable_Liion_48V_Battery_SOC();
    //ledcWrite(0, m_LCD_Duty);
}

void Custom_UI::CreateTable_Liion_48V_Battery_SOC()
{
    memset(m_Table_Liion_48V_Battery_SOC, 0, sizeof(m_Table_Liion_48V_Battery_SOC));

    int volt_row[10] = {390, 462, 478, 486, 491, 497, 502, 510, 520, 530};
    int volt_high[10] = {462, 478, 486, 491, 497, 502, 510, 520, 530, 533};
    double m = 0, n = 0;    // y = mx + n

    for(int i = 0; i < 10; i++)
    {
        m = (double)10 / (double)((double)volt_high[i] - (double)volt_row[i]);
        n = (double)(10 * i) - (double)((double)volt_row[i] * m);

        for(int j = volt_row[i]; j <= volt_high[i]; j++)
        {
            m_Table_Liion_48V_Battery_SOC[j] = m * (double)j + n;
        }
    }

    for(int i = 534; i <= 1000; i++)
    {
        m_Table_Liion_48V_Battery_SOC[i] = 100;
    }
}

void Custom_UI::ScalingData()
{ 
    const STC_MAIN *STC_MAIN_RecvBuf = m_controller->GetMainRecvBuf();

    m_ArrScaledData[static_cast<uint16_t>(SCALEDDATA::TEST1)] = (double)STC_MAIN_RecvBuf->Status.State / (double)10;
    m_ArrScaledData[static_cast<uint16_t>(SCALEDDATA::TEST2)] = (double)STC_MAIN_RecvBuf->Status.State / (double)100;
    m_ArrScaledData[static_cast<uint16_t>(SCALEDDATA::TEST3)] = (double)STC_MAIN_RecvBuf->Status.State / (double)1000;
}

void Custom_UI::DisplayData()
{
    const STC_MAIN *STC_MAIN_RecvBuf = m_controller->GetMainRecvBuf();

    String strSpeedNumber1 = String(m_ArrScaledData[static_cast<uint16_t>(SCALEDDATA::TEST1)]);
    String strLabelAVGSpeedNumber = String(m_ArrScaledData[static_cast<uint16_t>(SCALEDDATA::TEST2)]);
    String strLabelMaxSpeedNumber = String(m_ArrScaledData[static_cast<uint16_t>(SCALEDDATA::TEST3)]);

    lv_label_set_text(ui_Speed_Number_1, strSpeedNumber1.c_str());
    lv_label_set_text(ui_Label_AVG_Speed_Number, strLabelAVGSpeedNumber.c_str());
    lv_label_set_text(ui_Label_Max_Speed_Number, strLabelMaxSpeedNumber.c_str());
}

uint8_t Custom_UI::GetLCDDuty()
{
    return m_LCD_Duty;
}

void Custom_UI::SetLCDDuty(uint8_t val)
{
    m_LCD_Duty = val;
}

bool Custom_UI::GetDebugStatus()
{
    return m_Debug_Status;
}

void Custom_UI::SetDebugStatus(bool stat)
{
    m_Debug_Status = stat;
}

// ui_evnets.c >> UI Event Override
