#include <Windows.h>

DWORD addrMessageBoxW = NULL;

__declspec(naked) int (__stdcall prolog_MessagBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	__asm
	{
		push ebp
		mov ebp, esp //"custom" implementation prolog of the standard function prolog
		add addrMessageBoxW, 5  //add +5 to jump over prolog and skip hooks using hotpatching
		jmp [addrMessageBoxW]  //call MessageBoxW +5  
	}
}

int main(int argc, char *argv[])
{
	HMODULE hmodUser32 = LoadLibrary(L"user32.dll");
	addrMessageBoxW = (DWORD)GetProcAddress(hmodUser32, "MessageBoxW");

	prolog_MessagBoxW(0, L"Hook Hopping", L"Hook Hopping", 0);

	return 0;
}
