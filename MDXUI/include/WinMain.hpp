
#pragma once

//+--------------------------
// 
// �����ǰ���ǵ�����
//
//+--------------------------
#ifndef DIRECTUI_EXPORTS

#ifdef DXUI_WINDOW
#include <windows.h>
extern int main(int argc, char* argv[]);

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	return main(__argc, __argv);
}
#endif

#endif