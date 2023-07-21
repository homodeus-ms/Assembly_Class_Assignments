#define _CRT_SECURE_NO_WARNINGS
#include "transformer.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define ALIGN16 __declspec(align(16))

#define TRUE (1)
#define FALSE (0)


size_t read_points(vec4_t* points, const size_t count)
{
    enum { LINE_LENGTH = 1024 };
    char buffer[LINE_LENGTH];
    size_t i = 0;
    size_t read_count = 0;
    vec4_t temp;
    float read;

    while (scanf("%f", &read) == 1) {

        switch (read_count) {
        case 0:
            temp.x = read;
            break;
        case 1:
            temp.y = read;
            break;
        case 2:
            temp.z = read;
            break;
        default:
            assert(FALSE);
            break;
        }

        ++read_count;

        if (read_count == 3) {
            temp.w = 1.f;
            points[i++] = temp;
            read_count = 0;
        }
    }
    clearerr(stdin);

    return i;
}

void print_csv(const vec4_t* points, const size_t count)
{
    size_t i;
    for (i = 0; i < count; ++i) {
        printf("% 9.4f,% 9.4f,% 9.4f\n", points[i].x, points[i].y, points[i].z);
    }
}

void transpose(mat4_t* mat)
{
    __asm {
        mov eax, mat

        movaps xmm0,[eax]
        movaps xmm1,[eax+16]
        movaps xmm2,[eax+32]
        movaps xmm3,[eax+48]

        movaps xmm4,xmm0    // mm0: 3 2 1 0   mm1: 7 6 5 4
        shufps xmm4,xmm1,01000100b    // mm4 : 05 04 01 00
        shufps xmm0,xmm1,11101110b    // mm0 : 07 06 03 02

        movaps xmm5,xmm2    // mm2: 11 10 9 8  mm3: 15 14 13 12
        shufps xmm5,xmm3,01000100b    // mm5 : 13 12 09 08
        shufps xmm2,xmm3,11101110b    // mm2 : 15 14 11 10

        movaps xmm1,xmm4
        shufps xmm1,xmm5,10001000b    // xmm1 : 12 08 04 00
        movaps [eax],xmm1

        shufps xmm4,xmm5,11011101b    // xmm4: 13 09 05 01
        movaps [eax+16],xmm4

        movaps xmm3,xmm0
        shufps xmm3,xmm2,10001000b
        movaps [eax+32],xmm3

        shufps xmm0,xmm2,11011101b
        movaps [eax+48],xmm0
    }
}

void transform(vec4_t* dst, const vec4_t* src, const mat4_t* mat_tr)
{
    __asm {
        mov ecx,dst
        mov eax,mat_tr
        mov ebx,src

        movaps xmm7,[eax]

        movaps xmm0,[ebx]
        movaps xmm1,xmm0
        dpps xmm1,xmm7,0xFF    // xmm0=src , xmm1=dot product

        movaps xmm7,[eax+16]
        movaps xmm2,xmm0
        dpps xmm2,xmm7,0xFF

        movaps xmm7,[eax+32]
        movaps xmm3,xmm0
        dpps xmm3,xmm7,0xFF

        movaps xmm7,[eax+48]
        dpps xmm0,xmm7,0xFF

        unpcklps xmm1,xmm3
        unpcklps xmm2,xmm0
        unpcklps xmm1,xmm2

        movaps [ecx],xmm1
    }
}

void concatenate(mat4_t* dst, const mat4_t* m0, const mat4_t* m1_tr)
{
    __asm {
        mov eax,dst
        mov ebx,m0
        mov edx,m1_tr
        mov ecx,4
        mov esi,0

    cloop:
        movaps xmm7,[ebx+esi]
        movaps xmm1,xmm7
        movaps xmm0,[edx]
        dpps xmm1,xmm0,0xFF
        
        movaps xmm2,xmm7
        movaps xmm0,[edx+16]
        dpps xmm2,xmm0,0xFF
        
        unpcklps xmm1,xmm2

        movaps xmm3,xmm7
        movaps xmm0,[edx+32]
        dpps xmm3,xmm0,0xFF

        movaps xmm0,[edx+48]
        dpps xmm7,xmm0,0xFF

        unpcklps xmm3,xmm7

        movhps [eax+esi], xmm1
        movlps [eax+esi+8], xmm3

        add esi,16

        loop cloop
        
    }
}

void run(vec4_t* world_pts, const vec4_t* local_pts, const size_t count, const vec3_t* scale, const vec3_t* rotation, const vec3_t* translation)
{
    mat4_t mat_res0;
    mat4_t mat_res1;
    mat4_t mat2;

    mat_scale(&mat_res1, scale->x, scale->y, scale->z);
    mat_rotation_x(&mat2, rotation->x);
    transpose(&mat2);
    concatenate(&mat_res0, &mat_res1, &mat2);

    mat_rotation_y(&mat2, rotation->y);
    transpose(&mat2);
    concatenate(&mat_res1, &mat_res0, &mat2);

    mat_rotation_z(&mat2, rotation->z);
    transpose(&mat2);
    concatenate(&mat_res0, &mat_res1, &mat2);

    mat_translation(&mat2, translation->x, translation->y, translation->z);
    transpose(&mat2);
    concatenate(&mat_res1, &mat_res0, &mat2);   // 변환행렬 합친값 mat_res1에

    transpose(&mat_res1);

    for (size_t i = 0; i < count; ++i)
    {
        transform(&world_pts[i], &local_pts[i], &mat_res1);
    }
}
