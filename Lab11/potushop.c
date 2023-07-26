#include <assert.h>

#include "potushop.h"
#include "image.h"

#define SET_VALUE(v, value) do { \
    v.x = value; \
    v.y = value; \
    v.z = value; \
} while(0)  

ALIGN16 typedef struct vector4 {
    float x;
    float y;
    float z;
    float w;
} vector4_t;

const static vector4_t GRAYSCALE = { 0.3f, 0.59f, 0.11f, 0.f };

const static vector4_t SEPIA_R = { 0.393f, 0.769f, 0.189f, 0.f };
const static vector4_t SEPIA_G = { 0.349f, 0.686f, 0.168f, 0.f };
const static vector4_t SEPIA_B = { 0.272f, 0.534f, 0.131f, 0.f };

static vector4_t s_brightness = { 0.f, 0.f, 0.f, 1.f };
static vector4_t s_in_min = { 0.f, 0.f, 0.f, 1.f };
static vector4_t s_in_max = { 0.f, 0.f, 0.f, 1.f };
static vector4_t s_out_min = { 0.f, 0.f, 0.f, 1.f };
static vector4_t s_out_max = { 0.f, 0.f, 0.f, 1.f };

void set_brightness_arg(int brightness)
{
    assert(brightness >= -100 && brightness <= 100);
    assert(sizeof(vector4_t) == 16);

    float temp = brightness / 255.f;
    s_brightness.x = temp;
    s_brightness.y = temp;
    s_brightness.z = temp;
}

void set_level_args(int in_min, int in_max, int out_min, int out_max)
{
    assert(in_min >= 0 && in_min <= 253);
    assert(in_max >= 2 && in_max <= 255);
    assert(in_min < in_max);

    assert(out_min >= 0 && out_min <= 255);
    assert(out_max >= 0 && out_max <= 255);

    float temp = in_min / 255.f;
    SET_VALUE(s_in_min, temp);
    
    temp = in_max / 255.f;
    SET_VALUE(s_in_max, temp);

    temp = out_min / 255.f;
    SET_VALUE(s_out_min, temp);

    temp = out_max / 255.f;
    SET_VALUE(s_out_max, temp);
}

void to_grayscale(void)
{
    __asm {
        mov eax, 1
        cvtsi2ss xmm1, eax
        unpcklps xmm1, xmm1
        unpcklps xmm1, xmm1

        pxor xmm2, xmm2
        movaps xmm7, GRAYSCALE
        
        mov ecx, g_num_pixels

    grayloop:
        
        mov esi, ecx
        dec esi
        shl esi, 4
        movaps xmm0, g_pixels[esi]
        dpps xmm0, xmm7, 0xFF
        
        minps xmm0, xmm1
        maxps xmm0, xmm2
        movaps g_pixels[esi], xmm0
        
        loop grayloop
    }
}

void to_sepia(void)
{
    __asm {
        mov eax, 1
        cvtsi2ss xmm1, eax
        unpcklps xmm1, xmm1
        unpcklps xmm1, xmm1

        pxor xmm2, xmm2
        movaps xmm3, SEPIA_R
        movaps xmm4, SEPIA_G
        movaps xmm5, SEPIA_B

        mov ecx, g_num_pixels

    sepialoop:
        mov esi, ecx
        dec esi
        shl esi, 4

        movaps xmm0, g_pixels[esi]
        dpps xmm0, xmm3, 0xF1
        movaps xmm6, g_pixels[esi]
        dpps xmm6, xmm4, 0xF2
        movaps xmm7, g_pixels[esi]
        dpps xmm7, xmm5, 0xF4

        orps xmm0, xmm6
        orps xmm0, xmm7

        minps xmm0, xmm1
        maxps xmm0, xmm2

        movaps g_pixels[esi], xmm0

        loop sepialoop
    }
}

void change_brightness(void)
{
    __asm {
        mov eax, 1
        cvtsi2ss xmm1, eax
        unpcklps xmm1, xmm1
        unpcklps xmm1, xmm1
        pxor xmm2, xmm2
        movaps xmm3, s_brightness

        mov ecx, g_num_pixels

    brightloop:
        mov esi, ecx
        dec esi
        shl esi, 4

        movaps xmm0, g_pixels[esi]
        addps xmm0, xmm3
        minps xmm0, xmm1
        maxps xmm0, xmm2
        movaps g_pixels[esi], xmm0

        loop brightloop
    }
}

void change_levels(void)
{
    __asm {
        mov ecx, g_num_pixels
        movaps xmm1, s_in_min
        movaps xmm2, s_in_max
        movaps xmm3, s_out_min
        movaps xmm4, s_out_max

        movaps xmm5, xmm4
        subps xmm5, xmm3

        movaps xmm6, xmm2
        subps xmm6, xmm1

    levelloop:
        mov esi, ecx
        dec esi
        shl esi, 4

        movaps xmm0, g_pixels[esi]
        maxps xmm0, xmm1
        minps xmm0, xmm2

        subps xmm0, xmm1
        mulps xmm0, xmm5
        divps xmm0, xmm6
        addps xmm0, xmm3

        movaps g_pixels[esi], xmm0

        loop levelloop
    }
}
