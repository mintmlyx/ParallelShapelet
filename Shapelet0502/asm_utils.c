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

static inline void initVec8(double *inout, double *val) {

        __asm volatile ( "mov %0, %%rax;"
                "mov %1, %%rdx;"
                "movlpd 0x00(%%rdx), %%xmm0;"
                "movhpd 0x00(%%rdx), %%xmm0;"
                "movupd %%xmm0, 0x00(%%rax);"
                "movupd %%xmm0, 0x10(%%rax);"
                "movupd %%xmm0, 0x20(%%rax);"
                "movupd %%xmm0, 0x30(%%rax);"
        	:
                :"r"(inout), "r"(val)// , "r"(output)
                : "%rax","%rdx"//,"%rcx"
      	 );
}
static inline void initVec16(double *inout, double *val) {

        __asm volatile ( "mov %0, %%rax;"
                "mov %1, %%rdx;"
                "movlpd 0x00(%%rdx), %%xmm0;"
                "movhpd 0x00(%%rdx), %%xmm0;"
                "movupd %%xmm0, 0x00(%%rax);"
                "movupd %%xmm0, 0x10(%%rax);"
                "movupd %%xmm0, 0x20(%%rax);"
                "movupd %%xmm0, 0x30(%%rax);"
                "movupd %%xmm0, 0x40(%%rax);"
                "movupd %%xmm0, 0x50(%%rax);"
                "movupd %%xmm0, 0x60(%%rax);"
                "movupd %%xmm0, 0x70(%%rax);"
                :
                :"r"(inout), "r"(val)//, "r"(output)
                : "%rax","%rdx"//, "%rcx"
        );
}

static inline void initVec32(double *inout, double *val) {

        __asm volatile ( "mov %0, %%rax;"
                "mov %1, %%rdx;"
                "movlpd 0x00(%%rdx), %%xmm0;"
                "movhpd 0x00(%%rdx), %%xmm0;"
                "movupd %%xmm0, 0x00(%%rax);"
                "movupd %%xmm0, 0x10(%%rax);"
                "movupd %%xmm0, 0x20(%%rax);"
                "movupd %%xmm0, 0x30(%%rax);"
                "movupd %%xmm0, 0x40(%%rax);"
                "movupd %%xmm0, 0x50(%%rax);"
                "movupd %%xmm0, 0x60(%%rax);"
                "movupd %%xmm0, 0x70(%%rax);"
                "movupd %%xmm0, 0x80(%%rax);"
                "movupd %%xmm0, 0x90(%%rax);"
                "movupd %%xmm0, 0xA0(%%rax);"
                "movupd %%xmm0, 0xB0(%%rax);"
                "movupd %%xmm0, 0xC0(%%rax);"
                "movupd %%xmm0, 0xD0(%%rax);"
                "movupd %%xmm0, 0xE0(%%rax);"
                "movupd %%xmm0, 0xF0(%%rax);"
                :
                :"r"(inout), "r"(val)//, "r"(output)
                : "%rax","%rdx"//, "%rcx"
        );
}
static inline void divByScalarV8(double *inout, double *factor) {

        __asm volatile ( "mov %0, %%rax;"
                "mov %1, %%rdx;"
		//"mov %2, %%rcx;"
                "movlpd 0x00(%%rdx), %%xmm15;"
                "movhpd 0x00(%%rdx), %%xmm15;"
                "movupd 0x00(%%rax), %%xmm0;"
                "movupd 0x10(%%rax), %%xmm1;"
                "movupd 0x20(%%rax), %%xmm2;"
                "movupd 0x30(%%rax), %%xmm3;"
                "divpd %%xmm15, %%xmm0;"
                "divpd %%xmm15, %%xmm1;"
                "divpd %%xmm15, %%xmm2;"
                "divpd %%xmm15, %%xmm3;"
                "movupd %%xmm0, 0x00(%%rax);"
                "movupd %%xmm1, 0x10(%%rax);"
                "movupd %%xmm2, 0x20(%%rax);"
                "movupd %%xmm3, 0x30(%%rax);"
        	:
                :"r"(inout), "r"(factor)// , "r"(output)
                : "%rax","%rdx"//,"%rcx"
      	 );
}
static inline void divByScalarV16(double *inout, double *factor) {

        __asm volatile( "mov %0, %%rax;"
                "mov %1, %%rdx;"
		//"mov %2, %%rcx;"
                "movlpd 0x00(%%rdx), %%xmm15;"
                "movhpd 0x00(%%rdx), %%xmm15;"
                "movupd 0x00(%%rax), %%xmm0;"
                "movupd 0x10(%%rax), %%xmm1;"
                "movupd 0x20(%%rax), %%xmm2;"
                "movupd 0x30(%%rax), %%xmm3;"
                "movupd 0x40(%%rax), %%xmm4;"
                "movupd 0x50(%%rax), %%xmm5;"
                "movupd 0x60(%%rax), %%xmm6;"
                "movupd 0x70(%%rax), %%xmm7;"
                "divpd %%xmm15, %%xmm0;"
                "divpd %%xmm15, %%xmm1;"
                "divpd %%xmm15, %%xmm2;"
                "divpd %%xmm15, %%xmm3;"
                "divpd %%xmm15, %%xmm4;"
                "divpd %%xmm15, %%xmm5;"
                "divpd %%xmm15, %%xmm6;"
                "divpd %%xmm15, %%xmm7;"
                "movupd %%xmm0, 0x00(%%rax);"
                "movupd %%xmm1, 0x10(%%rax);"
                "movupd %%xmm2, 0x20(%%rax);"
                "movupd %%xmm3, 0x30(%%rax);"
                "movupd %%xmm4, 0x40(%%rax);"
                "movupd %%xmm5, 0x50(%%rax);"
                "movupd %%xmm6, 0x60(%%rax);"
                "movupd %%xmm7, 0x70(%%rax);"
                :
                :"r"(inout), "r"(factor)//, "r"(output)
                : "%rax","%rdx"//, "%rcx"
        );
}

static inline void divByScalarV32(double *inout, double *factor) {

        __asm volatile ( "mov %0, %%rax;"
                "mov %1, %%rdx;"
		//"mov %2, %%rcx;"
                "movlpd 0x00(%%rdx), %%xmm15;"
                "movhpd 0x00(%%rdx), %%xmm15;"
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
                "movupd 0xA0(%%rax), %%xmm10;"
                "movupd 0xB0(%%rax), %%xmm11;"
                "movupd 0xC0(%%rax), %%xmm12;"
                "movupd 0xD0(%%rax), %%xmm13;"
                "movupd 0xE0(%%rax), %%xmm14;"
                "divpd %%xmm15, %%xmm0;"
                "divpd %%xmm15, %%xmm1;"
                "divpd %%xmm15, %%xmm2;"
                "divpd %%xmm15, %%xmm3;"
                "divpd %%xmm15, %%xmm4;"
                "divpd %%xmm15, %%xmm5;"
                "divpd %%xmm15, %%xmm6;"
                "divpd %%xmm15, %%xmm7;"
                "divpd %%xmm15, %%xmm8;"
                "divpd %%xmm15, %%xmm9;"
                "divpd %%xmm15, %%xmm10;"
                "divpd %%xmm15, %%xmm11;"
                "divpd %%xmm15, %%xmm12;"
                "divpd %%xmm15, %%xmm13;"
                "divpd %%xmm15, %%xmm14;"
                "movupd %%xmm0, 0x00(%%rax);"
                "movupd %%xmm1, 0x10(%%rax);"
                "movupd %%xmm2, 0x20(%%rax);"
                "movupd %%xmm3, 0x30(%%rax);"
                "movupd %%xmm4, 0x40(%%rax);"
                "movupd %%xmm5, 0x50(%%rax);"
                "movupd %%xmm6, 0x60(%%rax);"
                "movupd %%xmm7, 0x70(%%rax);"
                "movupd %%xmm8, 0x80(%%rax);"
                "movupd %%xmm9, 0x90(%%rax);"
                "movupd %%xmm10, 0xA0(%%rax);"
                "movupd %%xmm11, 0xB0(%%rax);"
                "movupd %%xmm12, 0xC0(%%rax);"
                "movupd %%xmm13, 0xD0(%%rax);"
                "movupd %%xmm14, 0xE0(%%rax);"
                "movupd 0xF0(%%rax), %%xmm0;"
                "divpd %%xmm15, %%xmm0;"
                "movupd %%xmm0, 0xF0(%%rax);"
                :
                :"r"(inout), "r"(factor)//, "r"(output)
                : "%rax","%rdx"//, "%rcx"
        );
}
static inline void subScalarfromV8(double *input, double *factor, double *output) {

        __asm volatile ( "mov %0, %%rax;"
                "mov %1, %%rdx;"
		"mov %2, %%rcx;"
                "movlpd 0x00(%%rdx), %%xmm15;"
                "movhpd 0x00(%%rdx), %%xmm15;"
                "movupd 0x00(%%rax), %%xmm0;"
                "movupd 0x10(%%rax), %%xmm1;"
                "movupd 0x20(%%rax), %%xmm2;"
                "movupd 0x30(%%rax), %%xmm3;"
                "subpd %%xmm15, %%xmm0;"
                "subpd %%xmm15, %%xmm1;"
                "subpd %%xmm15, %%xmm2;"
                "subpd %%xmm15, %%xmm3;"
                "movupd %%xmm0, 0x00(%%rcx);"
                "movupd %%xmm1, 0x10(%%rcx);"
                "movupd %%xmm2, 0x20(%%rcx);"
                "movupd %%xmm3, 0x30(%%rcx);"
        	:
                :"r"(input), "r"(factor), "r"(output)
                : "%rax","%rdx","%rcx"
      	 );
}
static inline void subScalarfromV16(double *input, double *factor, double *output) {

        __asm volatile ( "mov %0, %%rax;"
                "mov %1, %%rdx;"
		"mov %2, %%rcx;"
                "movlpd 0x00(%%rdx), %%xmm15;"
                "movhpd 0x00(%%rdx), %%xmm15;"
                "movupd 0x00(%%rax), %%xmm0;"
                "movupd 0x10(%%rax), %%xmm1;"
                "movupd 0x20(%%rax), %%xmm2;"
                "movupd 0x30(%%rax), %%xmm3;"
                "movupd 0x40(%%rax), %%xmm4;"
                "movupd 0x50(%%rax), %%xmm5;"
                "movupd 0x60(%%rax), %%xmm6;"
                "movupd 0x70(%%rax), %%xmm7;"
                "subpd %%xmm15, %%xmm0;"
                "subpd %%xmm15, %%xmm1;"
                "subpd %%xmm15, %%xmm2;"
                "subpd %%xmm15, %%xmm3;"
                "subpd %%xmm15, %%xmm4;"
                "subpd %%xmm15, %%xmm5;"
                "subpd %%xmm15, %%xmm6;"
                "subpd %%xmm15, %%xmm7;"
                "movupd %%xmm0, 0x00(%%rcx);"
                "movupd %%xmm1, 0x10(%%rcx);"
                "movupd %%xmm2, 0x20(%%rcx);"
                "movupd %%xmm3, 0x30(%%rcx);"
                "movupd %%xmm4, 0x40(%%rcx);"
                "movupd %%xmm5, 0x50(%%rcx);"
                "movupd %%xmm6, 0x60(%%rcx);"
                "movupd %%xmm7, 0x70(%%rcx);"
                :
                :"r"(input), "r"(factor), "r"(output)
                : "%rax","%rdx", "%rcx"
        );
}

static inline void subScalarfromV32(double *input, double *factor, double *output) {

        __asm volatile ( "mov %0, %%rax;"
                "mov %1, %%rdx;"
		"mov %2, %%rcx;"
                "movlpd 0x00(%%rdx), %%xmm15;"
                "movhpd 0x00(%%rdx), %%xmm15;"
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
                "movupd 0xA0(%%rax), %%xmm10;"
                "movupd 0xB0(%%rax), %%xmm11;"
                "movupd 0xC0(%%rax), %%xmm12;"
                "movupd 0xD0(%%rax), %%xmm13;"
                "movupd 0xE0(%%rax), %%xmm14;"
                "subpd %%xmm15, %%xmm0;"
                "subpd %%xmm15, %%xmm1;"
                "subpd %%xmm15, %%xmm2;"
                "subpd %%xmm15, %%xmm3;"
                "subpd %%xmm15, %%xmm4;"
                "subpd %%xmm15, %%xmm5;"
                "subpd %%xmm15, %%xmm6;"
                "subpd %%xmm15, %%xmm7;"
                "subpd %%xmm15, %%xmm8;"
                "subpd %%xmm15, %%xmm9;"
                "subpd %%xmm15, %%xmm10;"
                "subpd %%xmm15, %%xmm11;"
                "subpd %%xmm15, %%xmm12;"
                "subpd %%xmm15, %%xmm13;"
                "subpd %%xmm15, %%xmm14;"
                "movupd %%xmm0, 0x00(%%rcx);"
                "movupd %%xmm1, 0x10(%%rcx);"
                "movupd %%xmm2, 0x20(%%rcx);"
                "movupd %%xmm3, 0x30(%%rcx);"
                "movupd %%xmm4, 0x40(%%rcx);"
                "movupd %%xmm5, 0x50(%%rcx);"
                "movupd %%xmm6, 0x60(%%rcx);"
                "movupd %%xmm7, 0x70(%%rcx);"
                "movupd %%xmm8, 0x80(%%rcx);"
                "movupd %%xmm9, 0x90(%%rcx);"
                "movupd %%xmm10, 0xA0(%%rcx);"
                "movupd %%xmm11, 0xB0(%%rcx);"
                "movupd %%xmm12, 0xC0(%%rcx);"
                "movupd %%xmm13, 0xD0(%%rcx);"
                "movupd %%xmm14, 0xE0(%%rcx);"
                "movupd 0xF0(%%rax), %%xmm0;"
                "subpd %%xmm15, %%xmm0;"
                "movupd %%xmm0, 0xF0(%%rcx);"
                :
                :"r"(input), "r"(factor), "r"(output)
                : "%rax","%rdx", "%rcx"
        );
}

static inline int computeSquare8(double *input, double *output)
{

	  __asm volatile ( 	"mov  %0, %%rax;"
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

static inline int computeSquare16(double *input, double *output)
{

	  __asm volatile ( 	"mov  %0, %%rax;"
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

static inline int computeSquare32(double *input, double *output)
{

	  __asm volatile ( 	"mov  %0, %%rax;"
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
static inline int computeDiff8(double *input1, double *input2, double *output)
{

          __asm volatile (       "mov  %0, %%rax;"
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
static inline int computeDiff16(double *input1, double *input2, double *output)
{

          __asm volatile (       "mov  %0, %%rax;"
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

static inline int computeDiff32(double *input1, double* input2, double *output)
{

	  __asm volatile ( 	"mov  %0, %%rax;"
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

static inline double scalarSum8(double *input, double *out)
{

          __asm volatile (       "mov  %0, %%rax;"
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

static inline double scalarSum16(double *input, double *out)
{

          __asm volatile (       "mov  %0, %%rax;"
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
static inline double scalarSum32(double *input, double *out)
{

          __asm volatile (       "mov  %0, %%rax;"
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

int computeSquareVec(double *source, double *dest, int off, int len)
{

		int offset = off;
		int vecLen = len;
	
		if(offset % 2) {
			dest[offset] = pow(source[offset], 2.0);
			offset++;
			--vecLen;
		}
                while(vecLen >= 8) {

                        if(vecLen >= 32) {

                                computeSquare32(&source[offset],&dest[offset]);
                                vecLen -= 32;
                                offset += 32;
                        }
                        if(vecLen < 32 && vecLen >=16) {

                                computeSquare16(&source[offset], &dest[offset]);
                                vecLen -= 16;
                                offset += 16;
                        }
                        if(vecLen < 16 && vecLen >=8) {

                                computeSquare8(&source[offset], &dest[offset]);
                                vecLen -=8;
                                offset += 8;
                        }
                }

                for(int i = 0; i < vecLen; i++) {

                        dest[offset] = pow(source[offset], 2.0);
                        ++offset;

                }
/*
		printf("SQUARE IS \n");
		for (int i=0; i < len; i++)
			printf("%f %f\n", source[i], dest[i]);

		printf("SQUARE END \n");
*/
}

double computeScalarSum(double *source, int from, int len)
{

		int offset = from;
		int vecLen = len;
		double sum = 0.0, temp = 0.0;
		if(offset % 2) {
			sum = source[offset];
			offset++;
			--vecLen;
		}
                while(vecLen >= 8) {

                        if(vecLen >= 32) {

                                sum += scalarSum32(&(source[offset]),&temp);
                                vecLen -= 32;
                                offset += 32;
                        }
                        if(vecLen < 32 && vecLen >=16) {

                                sum += scalarSum16(&(source[offset]), &temp);
                                vecLen -= 16;
                                offset += 16;
                        }
                        if(vecLen < 16 && vecLen >=8) {

                                sum += scalarSum8(&(source[offset]), &temp);
                                vecLen -=8;
                                offset += 8;
                        }
                }
                for(int i = 0; i < vecLen; i++) {

                        sum += source[offset];
                        ++offset;

		}
		return sum;
}

int computeDiffVec(double *array1, double *array2, double *buffer, int from, int len) {

	int vecLen = len;
	int offset = from;
	
	if(offset % 2) {
		buffer[offset] = (array1[offset] - array2[offset]);
		++offset;
		--vecLen;
	}
	while(vecLen >= 8) {


                if(vecLen >= 32) {
			
                        computeDiff32(&(array1[offset]), &(array2[offset]), &(buffer[offset]));
                        //computeSquare32(&(buffer[offset]), &(buffer[offset]));
                        //sum += scalarSum32(&(buffer[offset]), &temp);
                        vecLen -= 32;
                        offset += 32;
                }
                if(vecLen < 32 && vecLen >=16) {

                        computeDiff16(&(array1[offset]), &(array2[offset]), &(buffer[offset]));
                        //computeSquare16(&(buffer[offset]), &(buffer[offset]));
                        //sum += scalarSum16(&(buffer[offset]), &temp);
                        vecLen -= 16;
                        offset += 16;
                }
                if(vecLen < 16 && vecLen >=8) {

                        computeDiff8(&(array1[offset]), &(array2[offset]), &(buffer[offset]));
                        //computeSquare8(&(buffer[offset]), &(buffer[offset]));
                        //sum += scalarSum16(&(buffer[offset]), &temp);
                        vecLen -=8;
                        offset += 8;
                }
        }

        for(int i = 0; i < vecLen; i++) {

                buffer[offset] = (array1[offset] - array2[offset]);
                ++offset;

        }
}

void scalarDiffVec(double *source, double factor, double *dest, int len)
{

		int vecLen = len;
		int offset = 0; // off;

		if(vecLen % 2) {
			dest[offset] = source[offset] - factor;
			++offset;
			--vecLen;
		}
                while(vecLen >= 8) {

                        if(vecLen >= 32) {

                                subScalarfromV32(&(source[offset]),&factor, &(dest[offset]));
                                vecLen -= 32;
                                offset += 32;
                        }
                        if(vecLen < 32 && vecLen >=16) {

                                subScalarfromV16(&(source[offset]),&factor, &(dest[offset]));
                                vecLen -= 16;
                                offset += 16;
                        }
                        if(vecLen < 16 && vecLen >=8) {

                                subScalarfromV8(&(source[offset]),&factor, &(dest[offset]));
                                vecLen -=8;
                                offset += 8;
                        }
                }
                for(int i = 0; i < vecLen; i++) {

                        dest[offset] = source[offset] - factor;
                        ++offset;

		}

}

void divByScalarVec(double *source, double factor, int len)
{

		int vecLen = len;
		int offset = 0; // off;

		if(vecLen % 2) {
			source[offset] = source[offset]/factor;
			++offset;
			--vecLen;
		}
                while(vecLen >= 8) {

                        if(vecLen >= 32) {

                                divByScalarV32(&(source[offset]),&factor);
                                vecLen -= 32;
                                offset += 32;
                        }
                        if(vecLen < 32 && vecLen >=16) {

                                divByScalarV16(&(source[offset]),&factor);
                                vecLen -= 16;
                                offset += 16;
                        }

                        if(vecLen < 16 && vecLen >=8) {

                                divByScalarV8(&(source[offset]),&factor);
                                vecLen -=8;
                                offset += 8;
                        }
                }
                for(int i = 0; i < vecLen; i++) {

                        source[offset] = source[offset] / factor;
                        ++offset;

		}

}

void initVec(double *source, double initval, int len)
{

		int vecLen = len;
		int offset = 0; // off;

		if(vecLen % 2) {
			source[offset] = initval; 
			++offset;
			--vecLen;
		}
                while(vecLen >= 8) {

                        if(vecLen >= 32) {

                                initVec32(&(source[offset]),&initval);
                                vecLen -= 32;
                                offset += 32;
                        }
                        if(vecLen < 32 && vecLen >=16) {

                                initVec16(&(source[offset]),&initval);
                                vecLen -= 16;
                                offset += 16;
                        }

                        if(vecLen < 16 && vecLen >=8) {

                                initVec8(&(source[offset]),&initval);
                                vecLen -=8;
                                offset += 8;
                        }
                }
                for(int i = 0; i < vecLen; i++) {

                        source[offset] = initval;
                        ++offset;

		}

}
#ifdef ASM_TEST

int main() 
{

	double a[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0,
	 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0,
	 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0};

	double b[39];
	double c[39];
	double d[39];
	//printf("%f \n", computeScalarSum(a, 1, 38));

//	 computeSquareVec(a, b, 1, 32);

//	 computeDiffVec(a, b, c, 1, 32);

	scalarDiffVec(&a[1], 1.0, &b[1], 34);

	for(int i = 0; i < 39; i++) {
		printf(" a %f b%f  \n", a[i], b[i]);
	}
	printf("\nNEXT\n");
	divByScalarVec(b, 2.0,25);
	initVec(c, 0.0, 39);
	initVec(&d[1], 4.0, 38);
	initVec(c, 1.0, 5);
	for(int i = 0; i < 39; i++) {
		printf(" a %f b%f c %f d %f  \n", a[i], b[i], c[i], d[i]);
	}
}

#endif
