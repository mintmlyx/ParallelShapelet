//
//  parallel_compute.c
//  ComputeGap
//
//  Created by Mushroom on 4/12/13.
//  Copyright (c) 2013 Self. All rights reserved.
//

#include <stdio.h>
#include<string.h>
#include <errno.h>
#include "parallel_compute.h"



int computeSquare8(double *input, double *output)
{

	  __asm ( 	"mov  %0, %%rax;"
	                "mov  %1, %%rdx;"
                	"movupd 0x00(%%rax), %%xmm0;"
                	"movupd 0x10(%%rax), %%xmm1;"
                	"movupd 0x20(%%rax), %%xmm2;"
                	"movupd 0x30(%%rax), %%xmm3;"
                	"mulpd 0x00(%%rax), %%xmm0;"
                	"mulpd 0x10(%%rax), %%xmm1;"
                	"mulpd 0x20(%%rax), %%xmm2;"
                	"mulpd 0x30(%%rax), %%xmm3;"
                	"movupd %%xmm0, 0x00(%%rdx);"
                	"movupd %%xmm1, 0x10(%%rdx);"
                	"movupd %%xmm2, 0x20(%%rdx);"
                	"movupd %%xmm3, 0x30(%%rdx);"
                :
                :"r"(input), "r"(output)
                : "%rax","%rdx"
        );

}

int computeSquare16(double *input, double *output)
{

	  __asm ( 	"mov  %0, %%rax;"
	                "mov  %1, %%rdx;"
                	"movupd 0x00(%%rax), %%xmm0;"
                	"movupd 0x10(%%rax), %%xmm1;"
                	"movupd 0x20(%%rax), %%xmm2;"
                	"movupd 0x30(%%rax), %%xmm3;"
                	"movupd 0x40(%%rax), %%xmm4;"
                	"movupd 0x50(%%rax), %%xmm5;"
                	"movupd 0x60(%%rax), %%xmm6;"
                	"movupd 0x70(%%rax), %%xmm7;"
                	"mulpd 0x00(%%rax), %%xmm0;"
                	"mulpd 0x10(%%rax), %%xmm1;"
                	"mulpd 0x20(%%rax), %%xmm2;"
                	"mulpd 0x30(%%rax), %%xmm3;"
                	"mulpd 0x40(%%rax), %%xmm4;"
                	"mulpd 0x50(%%rax), %%xmm5;"
                	"mulpd 0x60(%%rax), %%xmm6;"
                	"mulpd 0x70(%%rax), %%xmm7;"
                	"movupd %%xmm0, 0x00(%%rdx);"
                	"movupd %%xmm1, 0x10(%%rdx);"
                	"movupd %%xmm2, 0x20(%%rdx);"
                	"movupd %%xmm3, 0x30(%%rdx);"
                	"movupd %%xmm4, 0x40(%%rdx);"
                	"movupd %%xmm5, 0x50(%%rdx);"
                	"movupd %%xmm6, 0x60(%%rdx);"
                	"movupd %%xmm7, 0x70(%%rdx);"
		:
                :"r"(input), "r"(output)
                : "%rax","%rdx"
        );
}

int computeSquare32(double *input, double *output)
{

	  __asm ( 	"mov  %0, %%rax;"
	                "mov  %1, %%rdx;"
                	"movupd 0x00(%%rax), %%xmm0;"
                	"movupd 0x10(%%rax), %%xmm1;"
                	"movupd 0x20(%%rax), %%xmm2;"
                	"movupd 0x30(%%rax), %%xmm3;"
                	"movupd 0x40(%%rax), %%xmm4;"
                	"movupd 0x50(%%rax), %%xmm5;"
                	"movupd 0x60(%%rax), %%xmm6;"
                	"movupd 0x70(%%rax), %%xmm7;"
                	"movupd 0x80(%%rax), %%xmm8;"
                	"movupd 0x90(%%rax), %%xmm9;"
                	"movupd 0xa0(%%rax), %%xmm10;"
                	"movupd 0xb0(%%rax), %%xmm11;"
                	"movupd 0xc0(%%rax), %%xmm12;"
                	"movupd 0xd0(%%rax), %%xmm13;"
                	"movupd 0xe0(%%rax), %%xmm14;"
                	"movupd 0xf0(%%rax), %%xmm15;"
                	"mulpd 0x00(%%rax), %%xmm0;"
                	"mulpd 0x10(%%rax), %%xmm1;"
                	"mulpd 0x20(%%rax), %%xmm2;"
                	"mulpd 0x30(%%rax), %%xmm3;"
                	"mulpd 0x40(%%rax), %%xmm4;"
                	"mulpd 0x50(%%rax), %%xmm5;"
                	"mulpd 0x60(%%rax), %%xmm6;"
                	"mulpd 0x70(%%rax), %%xmm7;"
                	"mulpd 0x80(%%rax), %%xmm8;"
                	"mulpd 0x90(%%rax), %%xmm9;"
                	"mulpd 0xa0(%%rax), %%xmm10;"
                	"mulpd 0xb0(%%rax), %%xmm11;"
                	"mulpd 0xc0(%%rax), %%xmm12;"
                	"mulpd 0xd0(%%rax), %%xmm13;"
                	"mulpd 0xe0(%%rax), %%xmm14;"
                	"mulpd 0xf0(%%rax), %%xmm15;"
                	"movupd %%xmm0, 0x00(%%rdx);"
                	"movupd %%xmm1, 0x10(%%rdx);"
                	"movupd %%xmm2, 0x20(%%rdx);"
                	"movupd %%xmm3, 0x30(%%rdx);"
                	"movupd %%xmm4, 0x40(%%rdx);"
                	"movupd %%xmm5, 0x50(%%rdx);"
                	"movupd %%xmm6, 0x60(%%rdx);"
                	"movupd %%xmm7, 0x70(%%rdx);"
                	"movupd %%xmm8, 0x80(%%rdx);"
                	"movupd %%xmm9, 0x90(%%rdx);"
                	"movupd %%xmm10, 0xa0(%%rdx);"
                	"movupd %%xmm11, 0xb0(%%rdx);"
                	"movupd %%xmm12, 0xc0(%%rdx);"
                	"movupd %%xmm13, 0xd0(%%rdx);"
                	"movupd %%xmm14, 0xe0(%%rdx);"
                	"movupd %%xmm15, 0xf0(%%rdx);"
		:
                :"r"(input), "r"(output)
                : "%rax","%rdx"
        );
}
int computeDiff8(double *input1, double *input2, double *output)
{

          __asm (       "mov  %0, %%rax;"
                        "mov  %1, %%rcx;"
                        "mov  %2, %%rdx;"
                        "movupd 0x00(%%rax), %%xmm0;"
                        "movupd 0x10(%%rax), %%xmm1;"
                        "movupd 0x20(%%rax), %%xmm2;"
                        "movupd 0x30(%%rax), %%xmm3;"
                        "subpd 0x00(%%rcx), %%xmm0;"
                        "subpd 0x10(%%rcx), %%xmm1;"
                        "subpd 0x20(%%rcx), %%xmm2;"
                        "subpd 0x30(%%rcx), %%xmm3;"
                        "movupd %%xmm0, 0x00(%%rdx);"
                        "movupd %%xmm1, 0x10(%%rdx);"
                        "movupd %%xmm2, 0x20(%%rdx);"
                        "movupd %%xmm3, 0x30(%%rdx);"
                :
                :"r"(input1), "r"(input2), "r"(output)
                : "%rax","%rcx", "%rdx"
        );

}
int computeDiff16(double *input1, double *input2, double *output)
{

          __asm (       "mov  %0, %%rax;"
                        "mov  %1, %%rcx;"
                        "mov  %2, %%rdx;"
                        "movupd 0x00(%%rax), %%xmm0;"
                        "movupd 0x10(%%rax), %%xmm1;"
                        "movupd 0x20(%%rax), %%xmm2;"
                        "movupd 0x30(%%rax), %%xmm3;"
                        "movupd 0x40(%%rax), %%xmm4;"
                        "movupd 0x50(%%rax), %%xmm5;"
                        "movupd 0x60(%%rax), %%xmm6;"
                        "movupd 0x70(%%rax), %%xmm7;"
                        "subpd 0x00(%%rcx), %%xmm0;"
                        "subpd 0x10(%%rcx), %%xmm1;"
                        "subpd 0x20(%%rcx), %%xmm2;"
                        "subpd 0x30(%%rcx), %%xmm3;"
                        "subpd 0x40(%%rcx), %%xmm4;"
                        "subpd 0x50(%%rcx), %%xmm5;"
                        "subpd 0x60(%%rcx), %%xmm6;"
                        "subpd 0x70(%%rcx), %%xmm7;"
                        "movupd %%xmm0, 0x00(%%rdx);"
                        "movupd %%xmm1, 0x10(%%rdx);"
                        "movupd %%xmm2, 0x20(%%rdx);"
                        "movupd %%xmm3, 0x30(%%rdx);"
                        "movupd %%xmm4, 0x40(%%rdx);"
                        "movupd %%xmm5, 0x50(%%rdx);"
                        "movupd %%xmm6, 0x60(%%rdx);"
                        "movupd %%xmm7, 0x70(%%rdx);"
                :
                :"r"(input1), "r"(input2), "r"(output)
                : "%rax", "%rcx", "%rdx"
        );
}

int computeDiff32(double *input1, double* input2, double *output)
{

	  __asm ( 	"mov  %0, %%rax;"
	                "mov  %1, %%rcx;"
	                "mov  %2, %%rdx;"
                	"movupd 0x00(%%rax), %%xmm0;"
                	"movupd 0x10(%%rax), %%xmm1;"
                	"movupd 0x20(%%rax), %%xmm2;"
                	"movupd 0x30(%%rax), %%xmm3;"
                	"movupd 0x40(%%rax), %%xmm4;"
                	"movupd 0x50(%%rax), %%xmm5;"
                	"movupd 0x60(%%rax), %%xmm6;"
                	"movupd 0x70(%%rax), %%xmm7;"
                	"movupd 0x80(%%rax), %%xmm8;"
                	"movupd 0x90(%%rax), %%xmm9;"
                	"movupd 0xa0(%%rax), %%xmm10;"
                	"movupd 0xb0(%%rax), %%xmm11;"
                	"movupd 0xc0(%%rax), %%xmm12;"
                	"movupd 0xd0(%%rax), %%xmm13;"
                	"movupd 0xe0(%%rax), %%xmm14;"
                	"movupd 0xf0(%%rax), %%xmm15;"
                	"subpd 0x00(%%rcx), %%xmm0;"
                	"subpd 0x10(%%rcx), %%xmm1;"
                	"subpd 0x20(%%rcx), %%xmm2;"
                	"subpd 0x30(%%rcx), %%xmm3;"
                	"subpd 0x40(%%rcx), %%xmm4;"
                	"subpd 0x50(%%rcx), %%xmm5;"
                	"subpd 0x60(%%rcx), %%xmm6;"
                	"subpd 0x70(%%rcx), %%xmm7;"
                	"subpd 0x80(%%rcx), %%xmm8;"
                	"subpd 0x90(%%rcx), %%xmm9;"
                	"subpd 0xa0(%%rcx), %%xmm10;"
                	"subpd 0xb0(%%rcx), %%xmm11;"
                	"subpd 0xc0(%%rcx), %%xmm12;"
                	"subpd 0xd0(%%rcx), %%xmm13;"
                	"subpd 0xe0(%%rcx), %%xmm14;"
                	"subpd 0xf0(%%rcx), %%xmm15;"
                	"movupd %%xmm0, 0x00(%%rdx);"
                	"movupd %%xmm1, 0x10(%%rdx);"
                	"movupd %%xmm2, 0x20(%%rdx);"
                	"movupd %%xmm3, 0x30(%%rdx);"
                	"movupd %%xmm4, 0x40(%%rdx);"
                	"movupd %%xmm5, 0x50(%%rdx);"
                	"movupd %%xmm6, 0x60(%%rdx);"
                	"movupd %%xmm7, 0x70(%%rdx);"
                	"movupd %%xmm8, 0x80(%%rdx);"
                	"movupd %%xmm9, 0x90(%%rdx);"
                	"movupd %%xmm10, 0xa0(%%rdx);"
                	"movupd %%xmm11, 0xb0(%%rdx);"
                	"movupd %%xmm12, 0xc0(%%rdx);"
                	"movupd %%xmm13, 0xd0(%%rdx);"
                	"movupd %%xmm14, 0xe0(%%rdx);"
                	"movupd %%xmm15, 0xf0(%%rdx);"
		:
                :"r"(input1), "r"(input2), "r"(output)
                : "%rax", "%rcx", "%rdx"
        );
}

double scalarSum8(double *input, double *out)
{

          __asm (       "mov  %0, %%rax;"
                        "mov %1, %%rbx;"
                        "movupd 0x00(%%rax), %%xmm0;"
                        "movupd 0x10(%%rax), %%xmm1;"
                        "addpd 0x20(%%rax), %%xmm0;"
                        "addpd 0x30(%%rax), %%xmm1;"
                        "addpd %%xmm1, %%xmm0;"
                        "movhlps %%xmm0, %%xmm1;"
                        "addpd %%xmm1, %%xmm0;"
                        "movlps %%xmm0, 0x00(%%rbx);"
                :
                :"r"(input), "r"(out)

                : "%rax", "rbx"
        );

        return *out;
}

double scalarSum16(double *input, double *out)
{

          __asm (       "mov  %0, %%rax;"
                        "mov %1, %%rbx;"
                        "movupd 0x00(%%rax), %%xmm0;"
                        "movupd 0x10(%%rax), %%xmm1;"
                        "movupd 0x20(%%rax), %%xmm2;"
                        "movupd 0x30(%%rax), %%xmm3;"
                        "addpd 0x40(%%rax), %%xmm0;"
                        "addpd 0x50(%%rax), %%xmm1;"
                        "addpd 0x60(%%rax), %%xmm2;"
                        "addpd 0x70(%%rax), %%xmm3;"
                        "addpd %%xmm1, %%xmm0;"
                        "addpd %%xmm3, %%xmm2;"
                        "addpd %%xmm2, %%xmm0;"
                        "movhlps %%xmm0, %%xmm1;"
                        "addpd %%xmm1, %%xmm0;"
                        "movlps %%xmm0, 0x00(%%rbx);"
                :
                :"r"(input), "r"(out)

                : "%rax", "rbx"
        );

        return *out;
}

/*we can go upto scalarSum64 but this is not reqd now*/
double scalarSum32(double *input, double *out)
{

          __asm (       "mov  %0, %%rax;"
                        "mov %1, %%rbx;"
                        "movupd 0x00(%%rax), %%xmm0;"
                        "movupd 0x10(%%rax), %%xmm1;"
                        "movupd 0x20(%%rax), %%xmm2;"
                        "movupd 0x30(%%rax), %%xmm3;"
                        "movupd 0x40(%%rax), %%xmm4;"
                        "movupd 0x50(%%rax), %%xmm5;"
                        "movupd 0x60(%%rax), %%xmm6;"
                        "movupd 0x70(%%rax), %%xmm7;"
                        "addpd 0x80(%%rax), %%xmm0;"
                        "addpd 0x90(%%rax), %%xmm1;"
                        "addpd 0xa0(%%rax), %%xmm2;"
                        "addpd 0xb0(%%rax), %%xmm3;"
                        "addpd 0xc0(%%rax), %%xmm4;"
                        "addpd 0xd0(%%rax), %%xmm5;"
                        "addpd 0xe0(%%rax), %%xmm6;"
                        "addpd 0xf0(%%rax), %%xmm7;"
                        "addpd %%xmm1, %%xmm0;"
                        "addpd %%xmm3, %%xmm2;"
                        "addpd %%xmm5, %%xmm4;"
                        "addpd %%xmm7, %%xmm6;"
                        "addpd %%xmm2, %%xmm0;"
                        "addpd %%xmm6, %%xmm4;"
                        "addpd %%xmm4, %%xmm0;"
                        "movhlps %%xmm0, %%xmm1;"
                        "addpd %%xmm1, %%xmm0;"
                        "movlps %%xmm0, 0x00(%%rbx);"
                :
                :"r"(input), "r"(out)

                : "%rax", "rbx"
        );

        return *out;
}
