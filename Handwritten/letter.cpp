#include "letter.h"
#include "common.h"
#include "font.h"

Letter::Letter(Font* font, int index) 
{
	mIndex = index;
	mSize = 0.0f;
	mKerning = 0;
	mAngle = 0;
	mFont = font;
}

Letter::Letter(Font* font, int index, float size, int angle, int kerning) 
{
	mIndex = index;
	mSize = size;
	mKerning = kerning;
	mAngle = angle;
	mFont = font;
}

void Letter::render(SDL_Point& pos) 
{
	if (pos.x > SCREEN_WIDTH - 100) {
		pos.x = START_X;
		pos.y += LINE_HEIGHT;
	}
	
	if (mIndex == SPACE) {
		pos.x += SPACE_WIDTH;
	}
	else if (mIndex == RETURN) {
		pos.y += LINE_HEIGHT;
		pos.x = START_X;
	}
	else {
		mFont->render(mIndex, mSize, mAngle, mKerning, pos);
	}

	mPoint.x = pos.x;
	mPoint.y = pos.y;
}


void Letter::update(Font* font, float size, int angle, int kerning) {
	mFont = font;
	mSize = size;
	mAngle = angle;
	mKerning = kerning;
}

void Letter::setIndex(int index) {
	mIndex = index;
}

SDL_Point Letter::getPosition() {
	return mPoint;
}

int Letter::getIndex() {
	return mIndex;
}

float Letter::getScale() {
	return mSize;
}

int Letter::getKerning() {
	return mKerning;
}

int Letter::getAngle() {
	return mAngle;
}

Font* Letter::getFont() {
	return mFont;
}