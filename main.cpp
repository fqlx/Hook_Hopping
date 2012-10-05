#include <Windows.h>

DWORD addrMessageBoxW = NULL;

__declspec(naked) int prolog_MessagBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	__asm
	{
		push ebp
		mov ebp, esp //"custom" implementation prolog of the standard function prolog
		add addrMessageBoxW, 5  //add +5 to jump over prolog and hook
		jmp [addrMessageBoxW]  //call MessageBoxW +5  
	}
}

__declspec(naked) void epilog_MessagBoxW()
{
	_asm
	{
		mov esp, ebp 
		pop ebp  
		ret  //epilog to pop our "custom" prolog to correct the stack
	}
}


int main(int argc, char *argv[])
{
	HMODULE hmodUser32 = LoadLibrary(L"user32.dll");
	addrMessageBoxW = (DWORD)GetProcAddress(hmodUser32, "MessageBoxW");

	prolog_MessagBoxW(0, L"Hook Hopping", L"Hook Hopping", 0);
	epilog_MessagBoxW();

	return 0;
}