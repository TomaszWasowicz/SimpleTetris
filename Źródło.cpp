#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <stdio.h>

using namespace std;

wstring tetromino[7];

int nScreenWidth = 80;			// Console Size X - columns
int nScreenHeight = 30;			// Console Screen Size Y - Rows

int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

int Rotate(int px, int py, int r)
{
	int pi = 0;
	switch (r % 4)
	{
		case 0: return py * 4 + px;
			break;
		case 1: return 12 + py - (px * 4);
			break;
		case 2: return 15 - (py * 4) - px;
			break;
		case 3: return 3 - py + (px * 4);
			break;
	}
	return pi;
}


bool DoesPieceFit(int nTetronimo, int nRotation, int nPosX, int nPosY)
{
	//All Field Cells that are >0 are occupied
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{
			//Get index into piece
			int pi = Rotate(px, py, nRotation);

			//Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			if (nPosX + px >= 0 && nPosY + py < nFieldHeight)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					//collision check
					if (tetromino[nTetronimo][pi] != L'.' && pField[fi] != 0)
						return false;
				}
			}
		}
	return true;
}

int main()
{

	wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	//Tetronimos 4x4
	tetromino[0].append(L"..X...X...X...X.");
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");

	pField = new unsigned char[nFieldWidth * nFieldHeight];
	for (int x = 0; x < nFieldWidth; x++) // Board Border
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 || nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
	







	return 0;
}
