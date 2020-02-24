
//服务端程序
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>//hotn
#include <sys/select.h>
#include <unistd.h>

#define PORT 1234
//最大的描述符
#define MAXSOCKFD 12
#define BUF_LEN 256

//文件描述符(file descriptor)是内核为文件所创建的索引
//Linux刚启动的时候会自动设置0是标准输入，1是标准输出，2是标准错误。
int main(){
	//is_connected 数组的index为文件描述符
	int sockfd, newsockfd, is_connected[MAXSOCKFD], fd;
	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr_in);
	fd_set readfds;
	char buf[BUF_LEN];
	char msg[] = "Welcome to server!";

    //开启socket文件描述符，用户新连接的接收 （根描述符）
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket error!");
		exit(1);
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //将文件描述符绑定端口和地址
	if((bind(sockfd, &addr, sizeof(addr))) < 0){
		perror("bind error");
		exit(1);
	}
	//开启监听,设置最大
	if(listen(sockfd, 3) < 0){
		perror("connect error");
		exit(1);
	}

    //初始文件描述符'标记'集合
	for(fd = 0; fd < MAXSOCKFD; fd ++)
		is_connected[fd] = 0;

    //超时时间
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 1000;

	while(1){
		//将读操作集合重置
		FD_ZERO(&readfds);
		//将根描述符，设置为可读操作
		FD_SET(sockfd, &readfds);

		//将以开启的文件描述，设置为可读操作
	    for(fd = 0; fd < MAXSOCKFD; fd++)
	    	if(is_connected[fd])
	    		FD_SET(fd, &readfds);

	    if(!select(MAXSOCKFD, &readfds, NULL, NULL, &timeout))
	        //如果超时那么跳过循环
	    	continue;
        //循环所有监听的描述符
	    for(fd = 0; fd < MAXSOCKFD; fd++){
	    	//如果文件描述符是可读的
	    	if(FD_ISSET(fd, &readfds)){
	    		//如果可读的文件描述符为 '根文件描述符' ，那么说明是-----新的连接
	    		if(sockfd == fd){
	    			if((newsockfd = accept(sockfd, &addr, &addr_len)) < 0)
	    				perror("accept error!");

	    			//向新文件描述符（连接）写入数据
					write(newsockfd, msg, sizeof(msg));
                    //将新文件描述符设置为 可读
					is_connected[newsockfd] = 1;
					//打印客户端地址
					printf("Connect from %s\n", inet_ntoa(addr.sin_addr));
	    		}
	    		//否则为 久文件描述符，那么是可读事件
	    		else{
	    			bzero(buf, sizeof(buf));
	    			//从文件描述符，读取信息， 如果没有读到数据说明，连接断了
	    			if(read(fd, buf , sizeof(buf)) <= 0){
	    				printf("Connection closed.\n");
	    				//取消文件描述符的可读状态
	    				is_connected[fd] = 0;
	    				//关闭文件描述符
	    				close(fd);
	    			}
	    			else{
	    				//打印客户端输入的内容
	    				printf("%s", buf);
	    			}
	    		}
	    	}
	    }
	}
}

