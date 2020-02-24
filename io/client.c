
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
#include <stdlib.h>
#include <ctype.h>
// socket编程中write、read和send、recv之间的区别 , 在于第四个参数recv可以控制是否，读取完成后是否删除缓冲
void main(int argc, char const *argv[]){
	//判断入参
	if (argc<2 || argc>3){
		printf("eg: ./a.out [ip] prot\n");
		exit(1);
	}
    const char *ip="127.0.0.1";
    int port;
	if (argc==3){
		ip = argv[1];
		port = atoi(argv[2]);
	}else{
		port = atoi(argv[1]);
	}

	//连接地址
	struct sockaddr_in addr;
	//创建根文件描述符
	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	//创建连接，此处三次握手，如果socket 创建时，参数为非阻塞，那么此处不等三次握手完成，立即返回。
	if((connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)))<0){
		printf("tcp 三次握手未完成，或错误无!");
		exit(1);
	}
	//读取数据的大小
	char buf[256];
    //从根文件描述符读取数据，最后一个参数可设置，是否阻塞模式
    //注意协议接收到的数据可能大于buf的长度，
    //所以在这种情况下要调用几次recv函数才能把s的接收缓冲中的数据copy完。recv函数仅仅是copy数据，真正的接收数据是协议来完成的
	recv(sockfd, buf, sizeof(buf), 10);
	printf("%s\n",buf);
    printf("please in put information:\n");
	while(1){
		bzero(buf,sizeof(buf));
/*      printf("%s\n","非阻塞 读取");
        int flags;
		//使用非阻塞io
		if(flags = fcntl(STDIN_FILENO, F_GETFL, 0) < 0)
		{
		    perror("fcntl");
		    return -1;
		}
		flags |= O_NONBLOCK;
		if(fcntl(STDIN_FILENO, F_SETFL, flags) < 0)
		{
		    perror("fcntl");
		    return -1;
		}*/

		//键盘读入.默认为阻塞
		read(STDIN_FILENO, buf, sizeof(buf));
		//写入到 根文件描述符
		if(send(sockfd, buf, sizeof(buf), 0) < 0){
			perror("send error!");
			exit(1);
		}
	}
}

