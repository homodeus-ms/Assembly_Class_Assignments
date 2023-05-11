#ifndef CANVAS_H
#define CANVAS_H

typedef enum opcode {
    OPCODE_CLEAR = 0b0,
    OPCODE_SET_PALETTE = 0b001,
    OPCODE_SET_X = 0b010,
    OPCODE_SET_Y = 0b011,    
    OPCODE_DRAW_COLOR = 0b100,
    OPCODE_SET_BRUSH = 0b101,
    OPCODE_SET_XY = 0b110,
    OPCODE_SET_MOV = 0b111,
} opcode_t;

typedef struct canvas {
    unsigned char* canvas;
    signed char x_pos;
    signed char y_pos;
    unsigned char palette_ID;
    unsigned char brush_color;
    unsigned char pendown;
    
} canvas_t;

void set_canvas(unsigned char* canvas32x32);

void execute(unsigned char instruction);


#endif /* CANVAS_H */

