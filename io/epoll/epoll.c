
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>//hotn
#include <unistd.h>
#include <sys/epoll.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	if (argc<2) {
		printf("eg: ./a.out prot\n");
		exit(1);
	}
    //atoi 类型转换
    int port = atoi(argv[1]);

    //创建套接字，文件描述符，接受连接的节点
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    //服务端地址
	struct sockaddr_in serverAddr;
    socklen_t serverAddrlen= sizeof(serverAddr);
    //初始化服务器 sockaddr_in
    memset(&serverAddr,0,serv_len);
    //地址族
    serverAddr.sin_family=AF_INET;
    //设置监听本机ip
    serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    //设置监听端口
    serverAddr.sin_port=htons(port);
    //绑定ip
    bind(lfd,(struct sockaddr *)&serverAddr,serverAddrlen);
    
    //监听 最大值128
    //在Linux内核2.2之后，socket backlog参数(listen的最后一个参数)的形为改变了，
    //现在它指等待accept的完全建立(ESTABLISHED 状态)的套接字的队列长度，而不是不完全(SYN RECEIVED 状态)连接请求的数量。 
    //不完全连接的长度可以使用/proc/sys/net/ipv4/tcp_max_syn_backlog设置。
    listen(lfd,128);
    printf("epoll io:Start accept .....\n");
    //客户端地址 声明
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen= sizeof(clientAddr);

    //-------------- epoll 相关代码----------------------------------------
    //创建epoll树, 初始创建2000个子节点， 可扩容
    int epfd = epoll_create(2000);
    //初始化，根节点 关注的事件
    struct epoll_event ev;
    //读写事件
    ev.events=EPOLLIN;
    ev.data.fd=lfd;
    //初始化epoll数根节点
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
    //声明 内核返回的，检测到的事件数组
    struct epoll_event all[2000];
    while(1){
    	//使用epoll 通知内核 文件io检测,第3参数为数组大小,最后一个参数-1代表阻塞
    	//返回值为，有多少个元素发生了，io事件
        int ret = epoll_wait(epfd,all,sizeof(all)/sizeof(0),-1);
        for (int i = 0; i < ret; ++i) {
        	int fd=all[i].data.fd;
        	if(fd == lfd){//新连接
        		//接受连接请求
        		int cfd=accept(lfd,(struct sockaddr *)&clientAddr, &clientAddrLen);
        		if(cfd ==-1){
        			perror("accpet error");
        			exit(0);
        		}
        		//初始化，普通节点 关注的事件
			    struct epoll_event tmp;
			    //读写事件
			    tmp.events=EPOLLIN;
			    tmp.data.fd=cfd;
        		//将新得到的cfd挂在树上
        		epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&tmp);
                //打印客户端信息
                char ip[64] = {0};
                printf("new Clinet ip %s, Port %d \n",
                	inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,ip,sizeof(ip)),
                	ntohs(clientAddr.sin_port)
                );
        	}else {
        		//处理已连接的客户端发来的数据
                char buf[1024] ={0};
        		int len = recv(fd, buf, sizeof(buf), 0);
        		if(len == -1){
        			perror("recv error");
        		}else if(len == 0){
        			printf("clent close");
        			close(fd);
        			//将fd从树上删除
    				epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
        		}else{
        			printf("recv buf %s\n", buf);
        			//将 buf 回写客户端
        			write(fd,buf,len);
        		}
        	}
        }
    }
    close(lfd);
	return 0;
}