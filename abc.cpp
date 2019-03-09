#include <string.h>  
#include <stdio.h>  
#include <iostream>
#include <ctime>
using namespace std;
  
struct demo
{
	char a;

};



int main()  
{  
	int a,b;
	//cin>>a>>b;
	cout<<a+b<<endl;
	cout<<sizeof(demo);
	
	demo *d;
	demo de;

	d=&de;

	d->a='a';

	cout<<(*d).a;


	string str1 = "Hello ";
    string str2 = "World";

    cout<<str1+str2;



}  
