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
unsigned char mask(char type, GameMemory* memory);

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
	memory->r[1] = 1;

	//do game logic
	//float deltaTime = input->deltaTime;
	while (memory->r[1])
	{
		char c;
		in.get(c);
		memory->ram[0] = c;
		in.get(c);
		memory->ram[1] = c;
		in.get(c);
		memory->ram[2] = c;
		/*std::cout << I << ' ' << T1 << ' ' << T2 << ' ' << P1 << ' ' << P2 << '\n';
		std::cin >> c ;*/
		std::cout << int(pos_x);
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
			for(Y=0; Y<windowBuffer->h; Y++)
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

			in.seekg(3 * P1 + 1);
			memory->r[1] = memory->r[2];
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
		case 12: //compare ==
			memory->r[1] &= (X == Y);
		case 13: //compare >=
			memory->r[1] &= (X >= Y);
		case 14: //compare >
			memory->r[1] &= (X > Y);
		case 15: //not
			memory->r[1] = !memory->r[1];
		}
		std::cout << "\n";//<<strlen(ram);
	}

	/*char* c = (char*)volatileMemory->allocate(100);
	char* c1 = (char*)volatileMemory->allocate(100);

	c[10] = 10;
	c1[10] = 10;*/
	

	//execute instructions

	//swap buffers
	//
	//	buffer 1
	//
	//  buffer 2
	//

	// desenezi pe ecran

	//for(int y=0; y<windowBuffer->h; y++)
	//	for (int x = 0; x < windowBuffer->w; x++)
	//	{
	//		windowBuffer->memory[4 * (x + y * windowBuffer->w) + 0] = 0 * ((y + x)%9);	//blue
	//		windowBuffer->memory[4 * (x + y * windowBuffer->w) + 1] = 1 * ((x + y*x ) % 17);	//green
	//		windowBuffer->memory[4 * (x + y * windowBuffer->w) + 2] = 1 * ((x*x*x+y)%20);	//red
	//		windowBuffer->memory[4 * (x + y * windowBuffer->w) + 3] = 0;
	//			
	//	}

	//draw player
	//for(int y=0; y<5; y++)
	//	for(int x=0; x<5; x++)
	//	{
	//		int newX = memory->posX + x;
	//		int newY = memory->posY + y;

	//		if (newX < 0) { continue; }
	//		if (newY < 0) { continue; }

	//		if (newX >= windowBuffer->w) { continue; }
	//		if (newY >= windowBuffer->h) { continue; }

	//		windowBuffer->memory[4 * (newX + newY * windowBuffer->w) + 0] = 0;//blue
	//		windowBuffer->memory[4 * (newX + newY * windowBuffer->w) + 1] = 255; //green
	//		windowBuffer->memory[4 * (newX + newY * windowBuffer->w) + 2] = 255; //red
	//	}

	////move player

	////float spped = 140 * deltaTime;
	//int spped = 1;
	//	
	//	if(input->up.pressed)
	//	{
	//		memory->posY -= spped;
	//	}
	//	if (input->down.pressed)
	//	{
	//		memory->posY += spped;
	//	}
	//	if (input->left.pressed)
	//	{
	//		memory->posX -= spped;
	//	}
	//	if (input->right.pressed)
	//	{
	//		memory->posX += spped;
	//	}

}

unsigned char mask(char type, GameMemory* memory)
{
	switch (type)
	{
	case 0:
		return (memory->ram[0] >> 2);
	case 1:
		return (memory->ram[0] & 3);
	case 2:
		return (memory->ram[1] >> 6);
	case 3:
		return (((memory->ram[1] & 63) << 1) + (memory->ram[2] >> 7));
	case 4:
		return (memory->ram[2] & 127);
	}
	return 0;
}

//bool readEntireFile(const char* name, void* buffer, size_t size, size_t* sizeRead)
//{
//	if (sizeRead)
//	{
//		*sizeRead = 0;
//	}
//
//	HANDLE file = CreateFile(name, GENERIC_READ, NULL, NULL,
//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	if (file == INVALID_HANDLE_VALUE)
//	{
//		return 0;
//	}
//
//	DWORD sizeRead1 = 0;
//
//	int rez = 1;
//
//	if (!ReadFile(file, buffer, size, &sizeRead1, NULL))
//	{
//		rez = 0;
//	}
//
//	if (sizeRead)
//	{
//		*sizeRead = sizeRead1;
//	}
//
//	CloseHandle(file);
//
//	return rez;
//}