#include "font.h"
#include "common.h"
#include <fstream>

Font::Font(int number) {
	mNumber = number;
	mSize = 7;
	mKerning = 0;
	mRepeats = 4;
}

bool Font::loadMedia() 
{
	bool success = true;

	if (!mFontTexture1.loadFromFile("Resources/" + std::to_string(mNumber) + "/font1.png")) {
		printf("Unable to load font1 texture!\n");
		mRepeats--;
	}

	if (!mFontTexture2.loadFromFile("Resources/" + std::to_string(mNumber) + "/font2.png")) {
		printf("Unable to load font2 texture!\n");
		mRepeats--;
	}

	if (!mFontTexture3.loadFromFile("Resources/" + std::to_string(mNumber) + "/font3.png")) {
		printf("Unable to load font3 texture!\n");
		mRepeats--;
	}

	if (!mFontTexture4.loadFromFile("Resources/" + std::to_string(mNumber) + "/font4.png")) {
		printf("Unable to load font4 texture!\n");
		mRepeats--;
	}

	if (mRepeats == 0) {
		return false;
	}

	std::ifstream map("Resources/" + std::to_string(mNumber) + "/header.txt");
	
	map >> mName;
	map >> mSize;
	map >> mKerning;

	if (mSize > 10) {
		mSize = 10;
	}
	else if (mSize < 0) {
		mSize = 0;
	}

	if (mKerning > 5) {
		mKerning = 5;
	}
	else if (mKerning < -5) {
		mKerning = -5;
	}
	
	int spriteCount = 0;
	map >> spriteCount;

	mSprites.resize(spriteCount);

	for (int i = 0; i < spriteCount; i++) {
		map >> mSprites[i].x;
		map >> mSprites[i].y;
		map >> mSprites[i].w;
		map >> mSprites[i].h;

		if (map.fail()) {
			printf("Error reading the header file!\n");
			success = false;
			break;
		}
	}

	map.close();

	mFont = TTF_OpenFont("ComingSoon.ttf", 32);
	if (mFont == NULL)
	{
		printf("Failed to font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else {
		SDL_Color black = { 0, 0, 0 };
		nameTexture.loadFromRenderedText(mFont, mName, black);
	}
	return success;
}

void Font::render(int index, float scale, int angle, int kerning, SDL_Point& pos)
{
	if (index < 0 || index >= int(mSprites.size())) {
		return;
	}

	int y = pos.y;

	LTexture* currentTexture = &mFontTexture1;
	
	switch (index / 54) {
	case 0:
		currentTexture = &mFontTexture1;
		break;
	case 1:
		currentTexture = &mFontTexture2;
		break;
	case 2:
		currentTexture = &mFontTexture3;
		break;
	case 3:
		currentTexture = &mFontTexture4;
		break;
	}

	currentTexture->setScale(scale);

	y = 101 - mSprites[index].h * scale;
	y = pos.y + y / 2;

	currentTexture->render(pos.x, y, &mSprites[index], angle);
	pos.x += mSprites[index].w * scale + kerning;
}

void Font::renderName(int x, int y) {
	nameTexture.render(x, y);
}

void Font::updateSize(bool direction) {
	if (direction == true) {
		if (mSize < 10) {
			mSize++;
		}
	}
	else {
		if (mSize > 0) {
			mSize--;
		}
	}
}

void Font::updateKerning(bool direction) {
	if (direction == true) {
		if (mKerning < 5) {
			mKerning++;
		}
	}
	else {
		if (mKerning > -5) {
			mKerning--;
		}
	}
}

bool Font::containsIndex(int index) {
	if (index >= 0 && index < int(mSprites.size())) {
		return true;
	}
	else {
		return false;
	}
}

int Font::getSize() {
	return mSize;
}

int Font::getKerning() {
	return mKerning;
}

int Font::getRepeats() {
	return mRepeats;
}

void Font::free() 
{
	TTF_CloseFont(mFont);

	mFontTexture1.free();
	mFontTexture2.free();
	mFontTexture3.free();
	mFontTexture4.free();

	nameTexture.free();
}