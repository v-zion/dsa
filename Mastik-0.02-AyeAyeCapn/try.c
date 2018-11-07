#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "helper.c"

// int add(int x,int y){
// 	return x+y;
// }
// int mul2(int x,int y,int n){
// 	return x*y%n;
// }
// // int div(int x,int y){
// // 	return (int)(x/(float)y);
// // }
// int mod(int x,int y){
// 	return x%y;
// }
int main(){
	int n=5,e=0xFFF,x=2,b=1;
	scanf("%d",&e);
	for(int i=0;i<30;i++){
		div_t q=div(x,x);
		x=q.rem;
		if((e & 1<<i)>>i){
			b=b*x;
			div(5,7);
			b=mod(b,n);
			printf("ss\n");
		}
	}
	return b;
}
