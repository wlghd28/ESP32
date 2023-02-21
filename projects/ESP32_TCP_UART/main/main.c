#include <stdio.h>
#include "tcp_client.h"
#include "uart_async_rxtxtasks.h"

void app_main(void)
{
    uart_start();
    tcp_client_start();
}
