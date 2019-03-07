
//#include<iostream>
#include<stdio.h>
 //namespace std;

struct abc{
	unsigned :8;

	unsigned   :8;
	unsigned   :8;
	unsigned   :8;
	unsigned isa:3;
	//unsigned isb:3;
	//unsigned isc:3; 
	
	//unsigned isd:3;
	//unsigned ise:3;
	//unsigned isf:3; 
	//long a,b,c;
	//char b[1000];
	//char a;	
};

struct ref{
	unsigned char a,b,c,d;
	//unsigned isa:3;
	//unsigned isb:3;
	//unsigned isc:3; 
	
	//unsigned isd:3;
	//unsigned ise:3;
	//unsigned isf:3; 
	//long a,b,c;
	//char b[1000];
	//char a;	
};

struct tc{
	unsigned char name;
	int age;
	//unsigned isa:3;
	//unsigned isb:3;
	//unsigned isc:3; 
	
	//unsigned isd:3;
	//unsigned ise:3;
	//unsigned isf:3; 
	//long a,b,c;
	//char b[1000];
	//char a;	
};


 


int main(){
	
	abc a;
	tc t;
	tc* b;
	tc *n;

	printf("struct %d\n",sizeof(b));
	printf("struct %d\n",(n==b));
	
	t.name='a';
	t.age=28;

    //取地址拿到 具体数值
	printf("age %d\n",(&t)->age);
	printf("name %c\n",(&t)->name);
	//用引用那值
	printf("age %d\n",t.age);
	printf("name %c\n",t.name);
	//取地址拿到 具体地址
	printf("age %d\n",&t.age);
	printf("name %c\n",&t.name);
    //取地址后转换成指针 拿到具体数值
	printf("age %d\n",(*(&t)).age);
	printf("name %c\n",(*(&t)).name);


	((ref*)(&a))->a=1;
	((ref*)(&a))->b=2;
	((ref*)(&a))->c=3;
	((ref*)(&a))->d=4;
	
	printf("%d",((ref*)(&a))->a);

	printf("%d",((ref*)(&a))->b);

	printf("%d",((ref*)(&a))->c);

	printf("%d",((ref*)(&a))->d);
	//*((int*)&a)=3;
	//*((int*)&a)=*((int*)&a)+2;
	//printf("%d",*((int*)&a));
	
	
	//a.z='a';
	//printf("%d",sizeof(a ));
	//printf("%h",a.z);
	//a.int;
	//printf("%d\n",(char*)&a.a-(char*)(&a));
	//printf("%d\n",(char*)&a.c-(char*)(&a.a));
	//printf("%d\n", &a.c- (&a.a));

	printf("struct %d\n",sizeof(abc));
	
	printf("char %d\n",sizeof(char));

	printf("int %d\n",sizeof(int));
	printf("short %d\n",sizeof(short));
	printf("long %d\n",sizeof(long));
	printf("double %d\n",sizeof(double));
	printf("float %d\n",sizeof(float));
	printf("long long %d\n",sizeof(long long));
	//std::cout<<"helloworld";
	return 0;
}

//运行 用 gcc 加参数，和g++ 可编译c++