#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <map>
#include "common.h"
#include "texture.h"
#include "window.h"
#include "font.h"
#include "input.h"

bool loadMap();

LWindow gWindow;

LTexture gPaperTexture;
LTexture gCursorTexture;
LTexture gMenuTexture;
LTexture gScaleTexture;

Font* myFont1 = new Font(1);
Font* myFont2 = new Font(2);
Font* myFont3 = new Font(3);
Font* myFont4 = new Font(4);
Font* myFont5 = new Font(5);
Font* myFont6 = new Font(6);
Font* myFont7 = new Font(7);
Font* myFont8 = new Font(8);

Font* currentFont;
Input myInput;

std::map<char, int> myMap;

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} 
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		if (!gWindow.init()) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gRenderer = gWindow.createRenderer();
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				if (TTF_Init() == -1) {
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	if (!gPaperTexture.loadFromFile("Resources/paper1.png")) {
		printf("Unable to load paper texture!\n");
		success = false;
	}
	if (!gCursorTexture.loadFromFile("Resources/cursor.png")) {
		printf("Unable to load cursor texture!\n");
		success = false;
	}
	if (!gMenuTexture.loadFromFile("Resources/menu.png")) {
		printf("Unable to load menu texture!\n");
		success = false;
	}
	else {
		gMenuTexture.clipSprite(50, 50);
	}
	if (!gScaleTexture.loadFromFile("Resources/scale.png")) {
		printf("Unable to load scake texture!\n");
		success = false;
	}
	else {
		gScaleTexture.clipSprite(50, 5);
	}

	if (!myFont1->loadMedia()) {
		success = false;
	}
	if (!myFont2->loadMedia()) {
		success = false;
	}
	if (!myFont3->loadMedia()) {
		success = false;
	}
	if (!myFont4->loadMedia()) {
		success = false;
	}
	if (!myFont5->loadMedia()) {
		success = false;
	}
	if (!myFont6->loadMedia()) {
		success = false;
	}
	if (!myFont7->loadMedia()) {
		success = false;
	}

	if (!myFont8->loadMedia()) {
		success = false;
	}
	if (!loadMap()) {
		success = false;
	}

	return success;
}

void close()
{
	gPaperTexture.free();
	gMenuTexture.free();
	gScaleTexture.free();

	myInput.free();

	myFont1->free();
	myFont1 = NULL;

	myFont2->free();
	myFont2 = NULL;

	myFont3->free();
	myFont3 = NULL;

	myFont4->free();
	myFont4 = NULL;

	myFont5->free();
	myFont5 = NULL;

	myFont6->free();
	myFont6 = NULL;

	myFont7->free();
	myFont7 = NULL;

	myFont8->free();
	myFont8 = NULL;

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[])
{
	if (!init()) {
		printf("Failed to initialize!\n");
	}
	else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		}
		else {
			bool quit = false;

			SDL_Event e;
			char inputText;

			SDL_Point cursor;
			cursor.x = START_X;
			cursor.y = START_Y;

			int index = -1;

			SDL_StartTextInput();
			Font* currentFont = myFont1;

			while (!quit) {
				bool renderText = false;

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
						case SDLK_RETURN:
							myInput.addLetter(index, 0, currentFont);
							index = myInput.addIndex(index);
							break;
						case SDLK_BACKSPACE:
							myInput.deleteIndex(index);
							index = myInput.subtractIndex(index);
							break;
						case SDLK_TAB:
							EDIT_STATE++;
							if (EDIT_STATE >= 5) {
								EDIT_STATE = 0;
							}
							break;
						case SDLK_UP:
							switch (EDIT_STATE) {
							case 0:
								currentFont->updateSize(true);
								myInput.updateFont(currentFont);
								break;
							case 1:
								if (RANDOM_SCALE < 10) {
									RANDOM_SCALE++;
								}
								myInput.updateFont(currentFont);
								break;
							case 2:
								currentFont->updateKerning(true);
								myInput.updateFont(currentFont);
								break;
							case 3:
								if (RANDOM_ALPHA < 10) {
									RANDOM_ALPHA++;
								}
								myInput.updateFont(currentFont);
								break;
							case 4:
								if (RANDOM_ANGLE < 10) {
									RANDOM_ANGLE++;
								}
								myInput.updateFont(currentFont);
								break;
							}
							break;
						case SDLK_DOWN:
							switch (EDIT_STATE) {
							case 0:
								currentFont->updateSize(false);
								myInput.updateFont(currentFont);
								break;
							case 1:
								if (RANDOM_SCALE > 0) {
									RANDOM_SCALE--;
								}
								myInput.updateFont(currentFont);
								break;
							case 2:
								currentFont->updateKerning(false);
								myInput.updateFont(currentFont);
								break;
							case 3:
								if (RANDOM_ALPHA > 0) {
									RANDOM_ALPHA--;
								}
								myInput.updateFont(currentFont);
								break;
							case 4:
								if (RANDOM_ANGLE > 0) {
									RANDOM_ANGLE--;
								}
								myInput.updateFont(currentFont);
								break;
							}
							break;
						case SDLK_LEFT:
							index = myInput.subtractIndex(index);
							break;
						case SDLK_RIGHT:
							index = myInput.addIndex(index);
							break;
						case SDLK_F1:
							currentFont = myFont1;
							myInput.changeFont(currentFont);
							break;
						case SDLK_F2:
							currentFont = myFont2;
							myInput.changeFont(currentFont);
							break;
						case SDLK_F3:
							currentFont = myFont3;
							myInput.changeFont(currentFont);
							break;
						case SDLK_F4:
							currentFont = myFont4;
							myInput.changeFont(currentFont);
							break;
						case SDLK_F5:
							currentFont = myFont5;
							myInput.changeFont(currentFont);
							break;
						case SDLK_F6:
							currentFont = myFont6;
							myInput.changeFont(currentFont);
							break;
						case SDLK_F7:
							currentFont = myFont7;
							myInput.changeFont(currentFont);
							break;
						case SDLK_F8:
							currentFont = myFont8;
							myInput.changeFont(currentFont);
							break;
						}
					}
					else if (e.type == SDL_TEXTINPUT) {
						inputText = *(e.text.text);
						renderText = true;
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				gPaperTexture.render(0, 0);

				if (renderText) {
					myInput.addLetter(index, inputText, currentFont);
					index = myInput.addIndex(index);
				}

				myInput.render();
				
				cursor = myInput.getPosition(index);
				gCursorTexture.render(cursor.x - 15, cursor.y);
				
				currentFont->renderName(50, 50);
				gMenuTexture.renderSprite(50, 100, EDIT_STATE);
	
				switch (EDIT_STATE) {
				case 0:
					gScaleTexture.renderSprite(50, 155, currentFont->getSize());
					break;
				case 1:
					gScaleTexture.renderSprite(50, 155, RANDOM_SCALE);
					break;
				case 2:
					gScaleTexture.renderSprite(50, 155, currentFont->getKerning() + 5);
					break;
				case 3:
					gScaleTexture.renderSprite(50, 155, RANDOM_ALPHA);
					break;
				case 4:
					gScaleTexture.renderSprite(50, 155, RANDOM_ANGLE);
					break;
				}
				SDL_RenderPresent(gRenderer);
			}
			SDL_StopTextInput();
		}
	}
	close();
	return 0;
}

bool loadMap() 
{
	bool success = true;
	std::fstream map("Resources/header.txt");
	
	int numLetters;
	map >> numLetters;

	for (int i = 0; i < numLetters; i++) {
		int index;
		char character;

		map >> index;
		map >> character;

		if (map.fail()) {
			printf("Unable to load map!\n");
			success = false;
			break;
		}
		myMap[character] = index;

	}

	map.close();

	return success;
}
