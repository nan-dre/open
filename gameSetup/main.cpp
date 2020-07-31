#include <Windows.h>
#include "gameStructs.h"
#include <iostream>
#include <fstream>

#define I (memory->ram[0]>>2)
#define T1 (memory->ram[0]&3)
#define T2 (memory->ram[1]>>6)
#define P1 (((memory->ram[1]&63)<<1)+(memory->ram[2]>>7))
#define P2 (memory->ram[2]&127)
#define X (memory->ram[126])
#define Y (memory->ram[127])

//to be removed
#define pos_x (memory->ram[5])
#define pos_y (memory->ram[6])

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD     fdwReason,
	LPVOID    lpvReserved
)
{

	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		OutputDebugString("gameLayer: dll attached");
	}

	return true;
}

std::ifstream in("D:\\Proiecte\\Infoedu\\open\\gameSetup\\rom.out");

extern "C" __declspec(dllexport) void gameLogic(GameInput* input, GameMemory* memory, 
	VolatileMemory *volatileMemory, GameWindowBuffer* windowBuffer)
{

	

	if(memory->isInitialized==0)
	{
		//incarc rom-ul
		//readEntireFile("D:\\Proiecte\\Infoedu\\open\\gameSetup\\rom.txt", memory->rom, 4092, &memory->count);
		
		/*memory->posX = 0;
		memory->posY = 0;*/
		memory->isInitialized = true;
	}
	memory->r[2] = 1;

	//do game logic
	//float deltaTime = input->deltaTime;
	while (memory->r[2])
	{
		char c;
		in.get(c);
		memory->ram[0] = c;
		in.get(c);
		memory->ram[1] = c;
		in.get(c);
		memory->ram[2] = c;
		//std::cin >> c;
		//std::cout << I << ' ' << T1 << ' ' << T2 << ' ' << P1 << ' ' << P2 << '\n';
		
		//std::cout << int(pos_x);
		if (T1 == 0)
			X = P1;
		if (T1 == 1)
			X = memory->ram[P1];
		if (T1 == 2)
			X = memory->r[P1];

		if (T2 == 0)
			Y = P2;
		if (T2 == 1)
			Y = memory->ram[P2];
		if (T2 == 2)
			Y = memory->r[P2];

		memory->r[3] = 0;
		if(input->up.held)
		{
			memory->r[3] += 4;
		}
		if (input->down.held)
		{
			memory->r[3] += 8;
		}
		if (input->left.held)
		{
			memory->r[3] += 1;
		}
		if (input->right.held)
		{
			memory->r[3] += 2;
		}
		
		//for(int i=0;i<5;i++)
		//    std::cout<<int(mask(i, memory))<<" ";
		switch (I)
		{
		case 0: //mov
			if (T1 == 1)
				memory->ram[P1] = Y;

			if (T1 == 2)
				memory->r[P1] = Y;
			break;
		case 1: // draw
			X = memory->ram[P1];
			Y = memory->ram[P2];
			windowBuffer->memory[4 * (X + Y * windowBuffer->w) + 0] = 255;	//blue
			windowBuffer->memory[4 * (X + Y * windowBuffer->w) + 1] = 255;	//green
			windowBuffer->memory[4 * (X + Y * windowBuffer->w) + 2] = 255;	//red
			windowBuffer->memory[4 * (X + Y * windowBuffer->w) + 3] = 0;
			break;
		case 2: //clear
			for (Y = 0; Y < windowBuffer->h; Y++)
				for (X = 0; X < windowBuffer->w; X++)
				{
					windowBuffer->memory[4 * (X + Y * windowBuffer->w) + 0] = 0;	//blue
					windowBuffer->memory[4 * (X + Y * windowBuffer->w) + 1] = 0;	//green
					windowBuffer->memory[4 * (X + Y * windowBuffer->w) + 2] = 0;	//red
					windowBuffer->memory[4 * (X + Y * windowBuffer->w) + 3] = 0;

				}
			break;
		case 3:
			//goto
				in.seekg(3 * Y + 1);
				memory->r[2] = 0;
			break;
		case 4: //exit game
			exit(0);
			break;
		case 5: // add
			memory->r[0] = X + Y;
			break;
		case 6: // sub
			memory->r[0] = X - Y;
			break;
		case 7: //multiply
			memory->r[0] = X * Y;
			break;
		case 8: // div
			memory->r[0] = X / Y;
			break;
		case 9: // mod
			memory->r[0] = X % Y;
			break;
		case 10: //compare <
			memory->r[1] &= (X < Y);
			break;
		case 11: //compare <=
			memory->r[1] &= (X <= Y);
			break;
		case 12: //compare ==
			memory->r[1] &= (X == Y);
			break;
		case 13: //compare >=
			memory->r[1] &= (X >= Y);
			break;
		case 14: //compare >
			memory->r[1] &= (X > Y);
			break;
		case 15: //not
			memory->r[1] = !memory->r[1];
			break;

		case 16: //jif
			if (X)
				in.seekg(3 * Y + 1);
			memory->r[2] = 0;
			break;
		case 17: //bitwise and
			memory->r[1] = bool(X & Y);
			break;
		case 18: //print score
			std::cout << int(X)<<  '\n';
			break;
		}
	}

}
