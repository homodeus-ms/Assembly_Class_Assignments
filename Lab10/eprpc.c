#include "eprpc.h"

static double temp = 0.0;

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
		fst temp
		fwait
		mov eax, dword ptr temp
		mov edx, dword ptr temp+4
		mov dword ptr [ebx], eax
		mov dword ptr [ebx+4], edx
	}
}

void eprpc_add(void)
{
	__asm {
		fadd
		//fst temp;
		fwait
	}
}
void eprpc_sub(void)
{
	__asm {
		fsub
		//fst temp;
		fwait
	}
}
void eprpc_mul(void)
{
	__asm {
		fmul
		//fst temp;
		fwait
	}
}
void eprpc_div(void)
{
	__asm {
		fdiv
		//fst temp;
		fwait
	}
}

