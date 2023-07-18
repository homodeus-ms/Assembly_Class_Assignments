#include <assert.h>
#include <stdio.h>

#include "calc.h"
#include "eprpc.h"

enum { NUM_DATA = 16 };

void printArr(double* operands, op_t* ops)
{
    size_t i = 0;

    while (ops[i] != OP_END) {
        printf("%d ", ops[i++]);
    }
    printf("%d\n", ops[i]);

    i = 0;

    while (ops[i] != OP_END) {
        if (ops[i] != OP_LOAD) {
            printf("?? ");
            ++i;
            continue;
        }
        printf("%lf ", operands[i++]);
    }
    printf("END\n");
}

int main(void)
{
    double result;
    double operands[NUM_DATA];
    op_t ops[NUM_DATA];

    parse_rpn("-1.9 0.23 11.12 -1.23 0.9 -1.1 8.99 3.02 + + + + + + +", ops, operands, NUM_DATA);
    calculate(ops, operands);
    eprpc_get_result(&result);

    printArr(operands, ops);

    printf("result: %lf\n", result);


#if 0
    double result;
    
    eprpc_init();
    eprpc_load(2.2);
    eprpc_load(3.2);
    eprpc_div();
    eprpc_get_result(&result);

    printf("%lf\n", result);
#endif

#if 0
    double result;
    double operands[NUM_DATA];
    op_t ops[NUM_DATA];

    parse_rpn("3.2 5.7 8.1 9.0 - + 1.1 / *", ops, operands, NUM_DATA);

    printArr(operands, ops);

#endif

#if 0
    double result;
    double operands[NUM_DATA];
    op_t ops[NUM_DATA];

    parse_rpn("1.2 3.4 5.2 + * 3.4 / 2.7 +", ops, operands, NUM_DATA);

    calculate(ops, operands);
    eprpc_get_result(&result);
    printf("%lf\n", result);
    //assert(5.7352941176470589 == result);

    eprpc_get_result(&result);

    //assert(5.7352941176470589 == result);

    parse_rpn("2.34e-4 4.523e-3 3.2 0.002 * + /", ops, operands, NUM_DATA);

    calculate(ops, operands);
    eprpc_get_result(&result);

    assert(0.021422686075254049 == result);

    parse_rpn("3.2 5.7 8.1 9.0 - + 1.1 / *", ops, operands, NUM_DATA);

    calculate(ops, operands);
    eprpc_get_result(&result);

    assert(13.963636363636363 == result);
#endif

    printf("NoAssert!\n");

    return 0;
}
