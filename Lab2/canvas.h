#ifndef CANVAS_H
#define CANVAS_H

typedef enum opcode {
    OPCODE_CLEAR = 0,
    OPCODE_SET_PALETTE = 1,
    OPCODE_SET_X = 2,
    OPCODE_SET_Y = 3,
    OPCODE_DRAW_COLOR = 4,
    OPCODE_SET_BRUSH = 5,
    OPCODE_SET_XY = 6,
    OPCODE_SET_MOV = 7,
} opcode_t;

typedef struct canvas {
    unsigned char* canvas;
    signed char x_pos;
    signed char y_pos;
    unsigned char brush_color;
    unsigned char pendown;
    
} canvas_t;

void set_canvas(unsigned char* canvas32x32);

void execute(unsigned char instruction);


#endif /* CANVAS_H */

