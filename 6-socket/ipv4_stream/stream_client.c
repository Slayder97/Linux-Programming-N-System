#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>     //  Chứa cấu trúc cần thiết cho socket. 
#include <netinet/in.h>     //  Thư viện chứa các hằng số, cấu trúc khi sử dụng địa chỉ trên internet
#include <arpa/inet.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
		
/* Chức năng chat */
void chat_func(int server_fd)
{
    int numb_write, numb_read;
    char recvbuff[BUFF_SIZE];
    char sendbuff[BUFF_SIZE];
    while (1) {
        memset(sendbuff, '0', BUFF_SIZE);
	   				     memset(recvbuff, '0', BUFF_SIZE);
        printf("Please enter the message : ");
        fgets(sendbuff, BUFF_SIZE, stdin);

        /* Gửi thông điệp tới server bằng hàm write */
        numb_write = write(server_fd, sendbuff, sizeof(sendbuff));
        if (numb_write == -1)     
            handle_error("write()");
        if (strncmp("exit", sendbuff, 4) == 0) {
            printf("Client exit ...\n");
            break;
        }
		
        /* Nhận thông điệp từ server bằng hàm read */
        numb_read = read(server_fd, recvbuff, sizeof(recvbuff));
        if (numb_read < 0) 
            handle_error("read()");
        if (strncmp("exit", recvbuff, 4) == 0) {
            printf("Server exit ...\n");
            break;
        }

        printf("\nMessage from Server: %s\n",recvbuff);   
    }
    close(server_fd); /*close*/ 
}

int main(int argc, char *argv[])
{
    int portno;
    int server_fd;
    struct sockaddr_in serv_addr;
	memset(&serv_addr, '0',sizeof(serv_addr));
	
    /* Đọc portnumber từ command line */
    if (argc < 3) {
        printf("command : ./client <server address> <port number>\n");
        exit(1);
    }
    portno = atoi(argv[2]);
	
    /* Khởi tạo địa chỉ server */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(portno);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) == -1) 
        handle_error("inet_pton()");
	
    /* Tạo socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
        handle_error("socket()");
	
    /* Kết nối tới server*/
    if (connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        handle_error("connect()");
	
    chat_func(server_fd);

    return 0;
}

