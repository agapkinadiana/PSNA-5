// ClientMS.cpp: определяет точку входа для консольного приложения.
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
	DWORD wb;                        
	char wbuf[50] = "Hello from Mailslot-client ";   
	try
	{
		if ((hM = CreateFile(L"\\\\*\\mailslot\\Box",
			GENERIC_WRITE,    
			FILE_SHARE_READ,  
			NULL,
			OPEN_EXISTING,    
			NULL, NULL)) == INVALID_HANDLE_VALUE)
			throw SetMailslotError("CreateFile:", GetLastError());

		int n = 0, k = 0, c = 0;
		cout << "Введите количество сообщений ";
		cin >> n;

		while (k != n) {
			k += 1;
			_itoa(k, wbuf + sizeof("Hello from Maislot-client ") - 1, 10);
			cout << wbuf << endl;
			if (!WriteFile(hM, wbuf, sizeof(wbuf), &wb, NULL))
				throw SetMailslotError("ReadFile:", GetLastError());
		}
		if (!WriteFile(hM,"", sizeof(""), &wb, NULL))
			throw SetMailslotError("ReadFile:", GetLastError());
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


