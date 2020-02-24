
//服务端程序
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>//hotn
#include <unistd.h>
#include <sys/select.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


//文件描述符(file descriptor)是内核为文件所创建的索引
//Linux刚启动的时候会自动设置0是标准输入，1是标准输出，2是标准错误。
int main(int argc, char const *argv[])
{
    //判断入参
	if (argc<2){
		printf("eg: ./a.out prot\n");
		exit(1);
	}
    //atoi 类型转换
    int port = atoi(argv[1]);
    //开启服务端 socket文件描述符，用户标记新连接的接收
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	//初始化服务器 sockaddr_in
	struct sockaddr_in serverAddr;
	socklen_t serverAddrLen= sizeof(serverAddr);
    memset(&serverAddr,0,serverAddrLen);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(port);
    //将文件描述符绑定端口和地址
	bind(lfd, (struct sockaddr *)&serverAddr, serverAddrLen);
	//开启监听,设置最大
	listen(lfd, 3);
    printf("select io:Start accept .....\n");
    //客户端地址 声明
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen= sizeof(clientAddr);

    //-------------- select 相关代码----------------------------------------
    int MAX_SOCK_FD_INDEX = 12;
    //超时时间
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 1000;
    //可读取的文件描述符集合
	fd_set readFds;
    //初始文件描述符'标记'集合 is_connected 数组的index为文件描述符‘索引’
    int isConnected[MAX_SOCK_FD_INDEX];
	for(int i = 0; i < MAX_SOCK_FD_INDEX; i++)
		isConnected[i] = 0;

	while(1){
		//将读操作集合重置
		FD_ZERO(&readFds);
		//将服务端描述符，设置为可读操作
		FD_SET(lfd, &readFds);
		//将准备就绪的，文件描述，设置为可读操作
	    for(int i= 0; i < MAX_SOCK_FD_INDEX; i++)
	    	if(isConnected[i])
	    		FD_SET(i, &readFds);

        //只有当缓存区可读的数据量(接收低水位标记)到达一定程度（默认 1）的时候，我们才能读到数据，不然不就读不到数据了吗。
        //只有当缓存区剩余一定空间(发送低水位标记)（默认 2048）,你才能写数据进去，不然可能导致空间不够。
	    if(!select(MAX_SOCK_FD_INDEX, &readFds, NULL, NULL, &timeout))
	        //如果超时那么跳过循环
	    	continue;
        //循环所有监听的描述符
	    for(int i = 0; i < MAX_SOCK_FD_INDEX; i++){
	    	//如果文件描述符是可读的
	    	if(FD_ISSET(i, &readFds)){
	    		int fd = i;
	    		//如果可读的文件描述符为 '根文件描述符' ，那么说明是-----新的连接
	    		if(lfd == fd){
	    			int cfd = accept(lfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	    			if(cfd ==-1){
	        			perror("accpet error");
	        			exit(0);
	        		}
	    			//向新文件描述符（连接）写入数据
					//write(cfd, "hello world", sizeof("hello world"));
                    //将新文件描述符设置为 可读
					isConnected[cfd] = 1;
					//打印客户端地址
					char ip[64] = {0};
					printf("new Clinet ip %s, Port %d \n",
	                	inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,ip,sizeof(ip)),
	                	ntohs(clientAddr.sin_port)
                    );
	    		}
	    		//否则为 旧文件描述符，那么是可读事件
	    		else{
	    			char buf[256];
	    			bzero(buf, sizeof(buf));
	    			//从文件描述符，读取信息， 如果没有读到数据说明，连接断了
	    			if(read(fd, buf , sizeof(buf)) <= 0){
	    				printf("Connection closed.\n");
	    				//取消文件描述符的可读状态
	    				isConnected[fd] = 0;
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

