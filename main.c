#include "uart.h"

int main()
{
    start_uart_thread();
    
    while(1)
    {
        usleep(1);
    }
}