#ifndef CUSTOM_TASK_H
#define CUSTOM_TASK_H

class Custom_UI;
class Controller;
class Custom_Task
{
public:
    Custom_Task(Custom_UI *custom_ui, Controller *controller);
    ~Custom_Task();

private:
    Custom_UI *m_custom_ui;
    Controller *m_controller;

public:
    void Init();
    static void Custom_Task_ScalingData(void *param);        // Scaling Data (CPU1)
    static void Custom_Task_RecvData(void *param);           // 시리얼 수신 (CPU0)
    static void Custom_Task_SendData(void *param);           // 시리얼 송신 (CPU0)
    static void Custom_Task_ProcessCommbuf(void *param);     // 수신데이터 처리 (CPU0)
};

extern Custom_Task *g_custom_task;
#endif