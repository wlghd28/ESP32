#ifndef CUSTOM_UI_H
#define CUSTOM_UI_H

class Controller;
class Custom_UI
{
public:
    Custom_UI(Controller *controller);
    ~Custom_UI();

public:
    enum class SCALEDDATA
    {
        TEST1,
        TEST2,
        TEST3,
        LEN
    };

private:
    Controller *m_controller;
    double m_ArrScaledData[static_cast<unsigned short>(SCALEDDATA::LEN)];
    double m_Table_Liion_48V_Battery_SOC[1001];
    unsigned char m_LCD_Duty;
    bool m_Debug_Status;    // 0:user, 1:debug

public:
    void Init();
    void CreateTable_Liion_48V_Battery_SOC();
    void ScalingData();
    void DisplayData();

    // Getter Setter
    unsigned char GetLCDDuty();
    void SetLCDDuty(unsigned char val);
    bool GetDebugStatus();
    void SetDebugStatus(bool stat);
};

extern Custom_UI *g_custom_ui;
#endif