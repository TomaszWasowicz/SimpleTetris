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
		case 0:
			pi = py * 4 + px;
			break;

		case 1: 
			pi = 12 + py - (px * 4);
			break;

		case 2:
			pi = 15 - (py * 4) - px;
			break;

		case 3:
			pi = 3 - py + (px * 4);
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

			if (nPosX + px >= 0 && nPosX + px < nFieldHeight)
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
	
	// game logic

	bool bKey[4];
	int nCurrentPiece = 0;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;
	int nSpeed = 20;
	int nSpeedCount = 0;
	bool bForceDown = false;
	bool bRotateHold = true;
	int nPieceCount = 0;
	int nScore = 0;
	vector<int> vLines;
	bool bGameOver = false;

	while(!bGameOver)
	{
		this_thread::sleep_for(50ms);
		nSpeedCount++;
		bForceDown = (nSpeedCount = nSpeed);

		//input
		for (int k = 0; k < 4; k++)
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		// game logic

		// handle player movement
		nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		// rotate, but latch to stop spinning
		if (bKey[3])
		{
			nCurrentRotation += (bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0;
			bRotateHold = false;
		}
		else
			bRotateHold - true;

		// force the piece down the playfield if it's time

		if (bForceDown)
		{
			// update diffuculty every 50 pieces
			nSpeedCount = 0;
			nPieceCount++;
			if (nPieceCount % 50 == 0)
				if (nSpeed >= 10) nSpeed--;

			// test fi piece can be moved down

			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; //if it can, so do it
			else
			{
				// if it can't, lock it in place
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;


				// check for lines
				for (int py = 0; py < 4; py++)
					if (nCurrentY + py < nFieldHeight - 1)
					{
						bool bLine = true;
						for (int px = 1; px < nFieldWidth - 1; px++)
							bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

						if (bLine)
						{
							// remove line, set to =
							for (int px = 1; px < nFieldWidth - 1; px++)
								pField[(nCurrentY + py) * nFieldWidth + px] = 8;
							vLines.push_back(nCurrentY + py);
						}
					}

				nScore += 25;
				if (!vLines.empty()) nScore += (1 << vLines.size() * 100);

				// pick new piece
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				// if the piece doesn't fit, game over

				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);

			}
		}


















	}






	CloseHandle(hConsole);
	cout << "Game Over!!Score:" << nScore << endl;
	system("pause");

	return 0;
}
