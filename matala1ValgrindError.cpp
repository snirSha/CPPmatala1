#include<iostream>
#include<stdlib.h>

int main(){
int* ptr;
ptr = (int*) malloc(100 * sizeof(int));

std::cout<<"hello world!"<<std::endl;
return 0;
}
