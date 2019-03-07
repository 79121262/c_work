#include<stdio.h>
#include <iostream>
#include<cmath>
using std::cout;

int f(){
 printf("%s\n","adsssaas");
 return 0;
}

struct ref{
	unsigned char a,b,c,d;
	int (*f)();
};

void f1(ref *d){
 //printf("------%s\n",(*d).a);
  cout <<"------"<<(*d).a<<"\n";
}


int main(){
	int i=1,t=1;

	double pi,s;

	while(fabs(s)<1e-8)

	s=double(1)/(i+2)*t;

	t=-t;

	pi+=s;

	cout<<"pi的值是：";

	return 0;
}