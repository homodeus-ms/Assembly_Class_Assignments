#include "calc.h"
#include "eprpc.h"
#include <stdio.h>
#include <assert.h>

#define TRUE (1)
#define FALSE (0)

void parse_rpn(const char* line, op_t* ops, double* operands, const size_t count)
{
    size_t idx = 0;
    const char* p = line;
    double temp;

    while (*p != '\0') {
        if (*p == ' ') {
            ++p;
            continue;
        } else if ((*p == '+' || *p == '-' || *p == '*' || *p == '/') && (*(p + 1) == ' ' || *(p + 1) == '\0')) {

            switch (*p) {
            case '+':
                ops[idx] = OP_ADD;
                break;
            case '-':
                ops[idx] = OP_SUB;
                break;
            case '*':
                ops[idx] = OP_MUL;
                break;
            case '/':
                ops[idx] = OP_DIV;
                break;
            default:
                assert(FALSE);
                break;
            }

        } else {

            ops[idx] = OP_LOAD;
            sscanf(p, "%lf", &temp);
            operands[idx] = temp;

            while (*p != ' ' && *p != '\0') {
                ++p;
            }
            if (*p == '\0') {
                ++idx;
                break;
            }
        }

        ++p;
        ++idx;
    }

    ops[idx] = OP_END;
}

void calculate(const op_t* op, const double* operands)
{
    size_t i = 0;

    while (op[i] != OP_END) {
        if (op[i] == OP_LOAD) {
            eprpc_load(operands[i++]);
            continue;
        } else {
            switch (op[i++]) {
            case OP_ADD:
                eprpc_add();
                break;
            case OP_SUB:
                eprpc_sub();
                break;
            case OP_MUL:
                eprpc_mul();
                break;
            case OP_DIV:
                eprpc_div();
                break;
            default:
                assert(FALSE);
                break;
            }
        }
    }
}

