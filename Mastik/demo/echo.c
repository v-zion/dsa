#include <stdio.h>

int main(int c, char **v) {
  while (--c) 
    puts(*++v);
}
