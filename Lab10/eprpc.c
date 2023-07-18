#include "eprpc.h"

static double s_temp = 0.0;

void __declspec(naked) eprpc_init(void)
{
    __asm {
        finit
        fldz
        ret
    }
}

void eprpc_load(const double num)
{
    __asm {
        fld num
        fwait
    }
}

void eprpc_get_result(double* out_result)
{
    __asm {
        mov ebx, out_result
        fst s_temp
        fwait
        mov eax, dword ptr s_temp
        mov edx, dword ptr s_temp+4
        mov dword ptr [ebx], eax
        mov dword ptr [ebx+4], edx
    }
}

void eprpc_add(void)
{
    __asm {
        fadd
        fwait
    }
}
void eprpc_sub(void)
{
    __asm {
        fsub
        fwait
    }
}
void eprpc_mul(void)
{
    __asm {
        fmul
        fwait
    }
}
void eprpc_div(void)
{
    __asm {
        fdiv
        fwait
    }
}

