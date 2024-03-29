// ServerMS.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>

using namespace std;

string  GetErrorMsgText(int code)    // error text 
{
	char intStr[20];
	_itoa(code, intStr, 10);
	string str = string(intStr);
	if (code == 121)
		str += "Вышело время ожидания";
	return "**ERROR**Code: " + str;
};
string  SetMailslotError(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};

int main()
{
	setlocale(0, "");
	HANDLE hM;  
	DWORD rb;                           
	char rbuf[50];                      
	try
	{
		if ((hM = CreateMailslot(L"\\\\.\\mailslot\\Box",
			300,  //кол-во байт 
			180000,   //количество сек ожидания для функции ReadFile  
			NULL)) == INVALID_HANDLE_VALUE)
			throw SetMailslotError("CreateMailslot:", GetLastError());
		clock_t start = clock();
		for (;;) {
		if (!ReadFile(hM, rbuf, sizeof(rbuf), &rb, NULL))
			throw SetMailslotError("ReadFile:", GetLastError());
			cout << rbuf << endl;
			if (rb == 0) break;
		}
		clock_t finish = clock();
		cout << "Затраченное время: " << ((float)(finish - start)) / CLOCKS_PER_SEC << " секунд" << endl;
		if (!CloseHandle(hM))
			throw SetMailslotError("close:", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText << endl;
	}
	system("pause");
	return 0;
}

