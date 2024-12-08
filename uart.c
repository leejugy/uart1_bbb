#include "uart.h"
#include "dynamic_que.h"
dynamic_que_t rx;

static void init_uart(char *uart_route)
{
    int fd = open(uart_route, O_RDWR | O_NOCTTY, 0666);
    if(fd < 0)
    {
        perror("fail to open uart");
        return;
    }

    struct termios term;
    memset(&term, 0, sizeof(term));

    term.c_cc[VMIN] = 0;
    term.c_cc[VTIME] = 0;
    term.c_cflag = B115200 | CS8 | CREAD | CLOCAL & ~CRTSCTS;
    term.c_oflag = 0;
    term.c_iflag = IGNPAR;

    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &term);

    close(fd);
}

int uart_send(char *uart_route, void *buffer, size_t buffer_size)
{
    int ret = 0;
    int fd = 0;
    fd = open(uart_route, O_RDWR | O_NOCTTY, 0666);
    if(fd < 0)
    {
        perror("uart open send fail");
        return -1;
    }

    ret = write(fd, buffer, buffer_size);
    if(ret < 0)
    {
        perror("uart send fail");
    }

    return ret;
}

int uart_recv(char *uart_route, void *buffer, size_t buffer_size)
{
    int ret = 0;
    int fd = 0;
    fd = open(uart_route, O_RDWR | O_NOCTTY, 0666);
    if(fd < 0)
    {
        perror("uart open send fail");
        return -1;
    }

    ret = read(fd, buffer, buffer_size);
    if(ret < 0)
    {
        perror("uart send fail");
    }

    return ret;
}

void uart_echo()
{
    int ret = 0;
    char send_buffer[128] = {0, };
    char recv_buffer[128] = {0, };
    char pop_buffer[128] = {0, };
    while(1)
    {
        memset(send_buffer, 0, sizeof(send_buffer));
        memset(recv_buffer, 0, sizeof(recv_buffer));
        memset(pop_buffer, 0, sizeof(pop_buffer));

        scanf("%s", send_buffer);
        ret = uart_send(UART1_ROUTE, send_buffer, strlen(send_buffer));
        if(ret > 0)
        {
            printf("[send] : %s\n", send_buffer);
        }

        usleep(100 * 1000);

        ret = uart_recv(UART1_ROUTE, recv_buffer, sizeof(recv_buffer));
        if(ret > 0)
        {
            printf("[recv] : %s\n", recv_buffer);
        }

        push_que(&rx, recv_buffer, ret);
        ret = pop_que(&rx, pop_buffer, sizeof(pop_buffer));
        if(ret > 0)
        {
            printf("[pop] : %s\n", pop_buffer);
        }
    }
}

void start_uart_thread()
{
    pthread_t tid[UART_THREAD_NUM] = {0, };
    uint8_t  i = 0;
    int ret = 0;
    void (*function_pointer[UART_THREAD_NUM]) = {uart_echo};
    
    init_uart(UART1_ROUTE);
    init_que(&rx);

    for( i = 0 ; i < UART_THREAD_NUM; i++)
    {
        ret = pthread_create(&tid[i], NULL, (void *)function_pointer[i], NULL);
        if(ret < 0)
        {
            perror("create thread fail");
            return;
        }

        ret = pthread_detach(tid[i]);
        if(ret < 0)
        {
            perror("pthread detach");
            return;
        }
    }
}