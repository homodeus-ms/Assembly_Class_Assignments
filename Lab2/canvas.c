#include "canvas.h"
#include "palette.h"

#include <stdio.h>

#define MAX_SIZE (32)
#define TRUE (1)
#define FALSE (0)

static canvas_t s_canvas;
const unsigned char* g_palette;
void set_vector_by_opcode_xy(unsigned char xy);

void set_canvas(unsigned char* canvas32x32)
{
    s_canvas.mCanvas = canvas32x32;
    s_canvas.mX = 0;
    s_canvas.mY = 0;

    return;
}

void execute(unsigned char instruction)
{
    opcode_t opcode = instruction >> 5;
    unsigned char operand = instruction & 0x1f;
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char color = *(g_palette + operand);
    unsigned char pendown;
    unsigned char dirX;
    unsigned char dirY;

    switch (opcode) {
    case OPCODE_CLEAR:
        for (i = 0; i < MAX_SIZE; ++i) {
            for (j = 0; j < MAX_SIZE; ++j) {
                s_canvas.mCanvas[j * MAX_SIZE + i] = color;
            }
        }
        break;
    case OPCODE_SET_PALETTE:
        g_palette = get_palette(operand);
        break;
    case OPCODE_SET_X:
        s_canvas.mX = operand;
        break;
    case OPCODE_SET_Y:
        s_canvas.mY = operand;
        break;
    case OPCODE_DRAW_COLOR:
        color = *(g_palette + operand);
        s_canvas.mCanvas[s_canvas.mY * MAX_SIZE + s_canvas.mX] = color;
        break;
    case OPCODE_SET_BRUSH:
        s_canvas.mBrushColor = *(g_palette + operand);
        break;
    case OPCODE_SET_XY:
        set_vector_by_opcode_xy(operand);
        break;
    case OPCODE_SET_MOV:
        pendown = (operand & 0x10) >> 4;
        dirX = (operand & 0xc) >> 2;
        dirY = operand & 0x3;

        if (pendown == TRUE)
        {
            s_canvas.mCanvas[s_canvas.mY * MAX_SIZE + s_canvas.mX] = s_canvas.mBrushColor;
        }

        if (dirX > 0)
        {
            s_canvas.mX += dirX % 2 == 1 ? 1 : -1;
            s_canvas.mX = s_canvas.mX < 0 ? 31 : s_canvas.mX;
            s_canvas.mX = s_canvas.mX == 32 ? 0 : s_canvas.mX;
        }
        if (dirY > 0)
        {
            s_canvas.mY += dirY % 2 == 1 ? -1 : 1;
            s_canvas.mY = s_canvas.mY < 0 ? 31 : s_canvas.mY;
            s_canvas.mY = s_canvas.mY == 32 ? 0 : s_canvas.mY;
        }
        if (pendown == TRUE)
        {
            s_canvas.mCanvas[s_canvas.mY * MAX_SIZE + s_canvas.mX] = s_canvas.mBrushColor;
        }

        break;
    default:
        printf("INVALID OPCODE!\n");
        break;
    }
}
void set_vector_by_opcode_xy(unsigned char xy)
{
    unsigned char x = 0;
    unsigned char y = 0;
    xy &= 0xf;
    unsigned quad = (xy & 0xc);
    unsigned corner = (xy & 0x3);

    y += corner % 2 == 1 ? 15 : 0;
    x += corner >= 2 ? 15 : 0;
    y += quad >= 8 ? 16 : 0;
    x += (quad == 0 || quad == 9) ? 16 : 0;

    s_canvas.mX = x;
    s_canvas.mY = y;
}
