#pragma once
#include<Windows.h>
struct MyWindow {
	int id;
	HWND handler;
	char* title;
	RECT rect;
};
