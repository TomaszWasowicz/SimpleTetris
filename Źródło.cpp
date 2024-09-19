#include <iostream>
using namespace std;

wstring tetromino[7];


int Rotate(int px, int py, int r)
{
	switch (r % 4)
	{
	case 0: return py * 4 + px;					// 0 degress
	case 1: return 12 + py - (px * 4);			//90 degrees
	case 2: return 15 - (py * 4) - px;			// 180 degress
	case 3: return 3 - py + (px * 4);			// 270 degrees

	}
}

int main()
{

	//Create the assets
	tetromino[0].append(L"..X...X...X...X.");
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");
	







	return 0;
}
