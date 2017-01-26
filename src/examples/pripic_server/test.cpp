/*
 *  test.cpp
 *    
 *
 *  by:  Roger Bermudez-Chacon
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int  main_2(){
    printf("number %d is %08lX\n", (uint32_t)3412, (uint32_t)3412);
    printf("number %d is %08lX\n", (uint32_t)15, (uint32_t)15);
    printf("number %d is %08lX\n", (uint32_t)312, (uint32_t)312);
    printf("number %d is %08lX\n", (uint32_t)313412, (uint32_t)313412);
    printf("number %d is %08lX\n", (uint32_t)2, (uint32_t)2);
    printf("number %d is %08lX\n", (uint32_t)3413, (uint32_t)3412);
    return 0;
}

int main(){
    uint32_t* thenums = (uint32_t*) malloc(sizeof(uint32_t) * 5);
    sscanf("0000000F", "%08lX", &thenums[0]);
    sscanf("0000100F", "%08lX", &thenums[1]);
    sscanf("00000002", "%08lX", &thenums[2]);
    sscanf("0000023A", "%08lX", &thenums[3]);
    printf("num was %d\n", thenums[0]);
    printf("num was %d\n", thenums[1]);
    printf("num was %d\n", thenums[2]);
    printf("num was %d\n", thenums[3]);
    return 0;
}
