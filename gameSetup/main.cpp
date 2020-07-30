#include <Windows.h>
#include "gameStructs.h"
#include <iostream>
#include <fstream>


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


bool readEntireFile(const char* name, void* buffer, size_t size, size_t* sizeRead);


extern "C" __declspec(dllexport) void gameLogic(GameInput* input, GameMemory* memory, 
	VolatileMemory *volatileMemory, GameWindowBuffer* windowBuffer)
{
	

	if(memory->isInitialized==0)
	{
		//incarc rom-ul
		readEntireFile("D:\\Proiecte\\Infoedu\\open\\gameSetup\\rom.txt", memory->rom, 4091, &memory->count);
		std::cout << memory->rom << memory->count;
		
		
		//tot aici initializezi variabile etc

		memory->posX = 0;
		memory->posY = 0;
		memory->isInitialized = true;
	}


	//do game logic
	float deltaTime = input->deltaTime;


	char* c = (char*)volatileMemory->allocate(100);
	char* c1 = (char*)volatileMemory->allocate(100);

	c[10] = 10;
	c1[10] = 10;
	

	//execute instructions

	//swap buffers
	//
	//	buffer 1
	//
	//  buffer 2
	//

	// desenezi pe ecran

	for(int y=0; y<windowBuffer->h; y++)
		for (int x = 0; x < windowBuffer->w; x++)
		{
			windowBuffer->memory[4 * (x + y * windowBuffer->w) + 0] = 0 * ((y + x)%9);	//blue
			windowBuffer->memory[4 * (x + y * windowBuffer->w) + 1] = 1 * ((x + y*x ) % 17);	//green
			windowBuffer->memory[4 * (x + y * windowBuffer->w) + 2] = 1 * ((x*x*x+y)%20);	//red
			windowBuffer->memory[4 * (x + y * windowBuffer->w) + 3] = 0;
				
		}

	//draw player
	for(int y=0; y<5; y++)
		for(int x=0; x<5; x++)
		{
			int newX = memory->posX + x;
			int newY = memory->posY + y;

			if (newX < 0) { continue; }
			if (newY < 0) { continue; }

			if (newX >= windowBuffer->w) { continue; }
			if (newY >= windowBuffer->h) { continue; }

			windowBuffer->memory[4 * (newX + newY * windowBuffer->w) + 0] = 0;//blue
			windowBuffer->memory[4 * (newX + newY * windowBuffer->w) + 1] = 255; //green
			windowBuffer->memory[4 * (newX + newY * windowBuffer->w) + 2] = 255; //red
		}

	//move player

	//float spped = 140 * deltaTime;
	int spped = 1;
		
		if(input->up.pressed)
		{
			memory->posY -= spped;
		}
		if (input->down.pressed)
		{
			memory->posY += spped;
		}
		if (input->left.pressed)
		{
			memory->posX -= spped;
		}
		if (input->right.pressed)
		{
			memory->posX += spped;
		}

}

bool readEntireFile(const char* name, void* buffer, size_t size, size_t* sizeRead)
{
	if (sizeRead)
	{
		*sizeRead = 0;
	}

	HANDLE file = CreateFile(name, GENERIC_READ, NULL, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	DWORD sizeRead1 = 0;

	int rez = 1;

	if (!ReadFile(file, buffer, size, &sizeRead1, NULL))
	{
		rez = 0;
	}

	if (sizeRead)
	{
		*sizeRead = sizeRead1;
	}

	CloseHandle(file);

	return rez;
}