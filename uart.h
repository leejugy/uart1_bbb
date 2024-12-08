#ifndef __UART__
#define __UART__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdint.h>

#define UART1_ROUTE "/dev/ttyS1"
#define UART2_ROUTE "/dev/ttyS2"
#define UART3_ROUTE "/dev/ttyS3"
#define UART4_ROUTE "/dev/ttyS4"
#define UART5_ROUTE "/dev/ttyS5"

#define UART_THREAD_NUM 1

int uart_send(char *uart_route, void *buffer, size_t buffer_size);
int uart_recv(char *uart_route, void *buffer, size_t buffer_size);
void start_uart_thread();
#endif