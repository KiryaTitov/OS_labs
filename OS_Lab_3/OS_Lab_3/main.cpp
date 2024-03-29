#include <Windows.h>
#include <iostream>
#include <ctime>
#include <process.h>
using namespace std;

CRITICAL_SECTION cs;
CRITICAL_SECTION cs2;
HANDLE sumElement;
HANDLE hOutEvent;
int k;
int sum;
int ssize;
int* mas;
int ttime;

DWORD WINAPI Work(LPVOID par)
{
	EnterCriticalSection(&cs);
	cout << "������� ��������� ��������, ��������� ��� �����a " << endl;
	cin >> ttime;
	bool key = false;
	int rs = 0;
	bool r = true;
	for (int i = 0; i < ssize; i++)
	{
		Sleep(ttime);
		key = false;
		for (int j = 2; j <= sqrt((double)mas[i]); j++)
		{
			if (mas[i] % j == 0 && mas[i] != j)
			{
				key = true;
				break;
			}
		}
		if (mas[i] == 1)
			key = true;
		if (key == false) {
			if (rs != i)
			{
				int t = mas[rs];
				mas[rs] = mas[i];
				mas[i] = t;
				rs++;
			}
			else
				rs++;
		}
		if (rs == k && r) {
			r = false;
			//LeaveCriticalSection(&cs);
			Sleep(100);
			//EnterCriticalSection(&cs);
		}
	}
	LeaveCriticalSection(&cs);
	return 0;
}

DWORD WINAPI SumElement(LPVOID par) {
	EnterCriticalSection(&cs2);
	WaitForSingleObject(hOutEvent, INFINITE);
	for (int i = 0; i < k; i++) {
		Sleep(ttime);
		sum += mas[i];
	}
	LeaveCriticalSection(&cs2);
	return 0;
}

int main()
{
	HANDLE work, sumElement;
	DWORD IDwork, IDs;
	HANDLE hOutEvent;
	setlocale(LC_ALL, "RUS");
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs2);
	cout << "������� ������ �������" << endl;
	cin >> ssize;
	mas = new int[ssize];
	cout << "������� ������ ����������� " << ssize << endl;
	for (int i = 0; i < ssize; i++) {
		cin >> mas[i];
	}
	cout << "�������� ������ ����������� " << ssize << endl;
	for (int i = 0; i < ssize; i++) {
		cout << mas[i] << " ";
	}



	work = CreateThread(NULL, 0, Work, NULL, CREATE_SUSPENDED, &IDwork);
	if (work == NULL) return GetLastError();
	hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hOutEvent == NULL)
		return GetLastError();
	
	cout << endl;
	cout << "B������ k" << endl;
	cin >> k;
	ResumeThread(work);
	sumElement = CreateThread(NULL, 0, SumElement, NULL, NULL, &IDs);
	if (sumElement == NULL) return GetLastError();

	Sleep(100);
	EnterCriticalSection(&cs);
	//ResumeThread(sumElement);

	cout << "������������� ����� " << endl;
	for (int i = 0; i < ssize; i++) {
		cout << mas[i] << " ";
	}
	cout << endl;
	LeaveCriticalSection(&cs);
	SetEvent(hOutEvent); 
	EnterCriticalSection(&cs2);
	cout << "��������� ������ ������ SumElement: sum = " << sum << endl;
	LeaveCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	EnterCriticalSection(&cs);
	cout << "�������� ������" << endl;
	for (int i = 0; i < ssize; i++) {
		cout << mas[i] << " ";
	}
	cout << endl;
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
	return 0;
}