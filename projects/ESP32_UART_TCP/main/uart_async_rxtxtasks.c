#include "uart_async_rxtxtasks.h"
#include "tcp_comm.h"

static const int RX_BUF_SIZE = 1024;
//static char g_cCount = 0;
static char g_cTestValue = 0;

//#define TXD_PIN (GPIO_NUM_4)
//#define RXD_PIN (GPIO_NUM_5)

#define TXD_PIN (GPIO_NUM_17)
#define RXD_PIN (GPIO_NUM_5)

void uart_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char* logName, const char* data, int len)
{
    //const int len = sizeof(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

static void tx_task(void *arg)
{
    static const char *TX_TASK_TAG = "TX_TASK";
    char *send_memory = (char*)malloc(sizeof(char) * 8);
    //char *send_memory = (char*)malloc(sizeof(char) * 15);
    send_memory[0] = 0x5A;
    send_memory[1] = 0xA5;
    send_memory[2] = 0x05;
    send_memory[3] = 0x82;
    send_memory[4] = 0x50;
    send_memory[5] = 0x12;
    send_memory[6] = 0x00;
    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
    while (1) {
        //g_cCount += 10;
        //if(g_cCount > 100) g_cCount = 0;
        //memcpy(send_memory + 7, (char*)&g_cCount, sizeof(char));
        g_cTestValue = (char)((int)g_PMFC_RecvData.State.Temp[ADC_TEMP_INTERNAL] / (int)10);
        memcpy(send_memory + 7, & g_cTestValue, sizeof(char));


        ESP_LOGI("RecvData", "Data : %d %d",  (int)g_cTestValue, g_PMFC_RecvData.State.Temp[ADC_TEMP_INTERNAL]);

        ESP_LOGI("Send_Memory", "Send : %2x %2x %2x %2x %2x %2x %2x %2x", 
        send_memory[0], send_memory[1], send_memory[2], send_memory[3], send_memory[4], send_memory[5], send_memory[6], send_memory[7]);

        //sendData(TX_TASK_TAG, "Hello world");
        sendData(TX_TASK_TAG, send_memory, sizeof(char) * 8);
        //sendData(TX_TASK_TAG, send_memory, sizeof(char) * 15);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    free(send_memory);
    send_memory = NULL;
}

static void rx_task(void *arg)
{
    static const char *RX_TASK_TAG = "RX_TASK";
    esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%d'", rxBytes, (*data));
            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
        }
    }
    free(data);
}

void uart_start()
{
    uart_init();
    xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
    xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);
}
