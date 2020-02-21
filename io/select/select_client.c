
//客户端程序
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234
#define SERVER_IP "127.0.0.1"
#define BUF_LEN 256


// socket编程中write、read和send、recv之间的区别 , 在于第四个参数recv可以控制是否，读取完成后是否删除缓冲
void main()
{
	int sockfd;
	//连接地址
	struct sockaddr_in addr;
	//读取数据的大小
	char buf[BUF_LEN];
	//创建根文件描述符
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
		perror("socket error!");
		exit(1);
	}
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	//创建连接
	if((connect(sockfd, &addr, sizeof(addr)))<0){
		perror("connect error!");
		exit(1);
	}

    //从根文件描述符读取数据
	recv(sockfd, buf, sizeof(buf), 0);

	printf("%s\n",buf);
    printf("please in put information:\n");
	while(1){
		bzero(buf,sizeof(buf));
		//键盘读入
		read(STDIN_FILENO, buf, sizeof(buf));
		//写入到 根文件描述符
		if(send(sockfd, buf, sizeof(buf), 0) < 0){
			perror("send error!");
			exit(1);
		}
	}
}

