#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int add(int x,int y){
	return x+y;
}

int mul2(int x,int y,int n){
	return x*y%n;
}

int mod(int x,int y){
	return x%y;
}

