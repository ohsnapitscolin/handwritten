#include "input.h"
#include "common.h"

Input::Input() 
{
	mLetters.resize(1000);
	mLength = 0;
}

void Input::addLetter(int cursor, char character, Font* font) 
{	
	switch (character) {
	case 0:
		mLetters.insert(mLetters.begin() + (cursor + 1), new Letter(font, -2));
		mLength++;
		break;
	case ' ':
		mLetters.insert(mLetters.begin() + (cursor + 1), new Letter(font, -1));
		mLength++;
		break;
	default:
		if (myMap.find(character) == myMap.end()) {
			break;
		}

		int value = myMap[character];
		int index = (rand() % font->getRepeats() * 54) + value;
	
		float scale = getScale(font->getSize());
		int kerning = getKerning(font->getKerning());
		int angle = getAngle();
	
		if (font->containsIndex(index)) {
			mLetters.insert(mLetters.begin() + (cursor + 1), new Letter(font, index, scale, angle, kerning));
			mLength++;
		}
	}
}

void Input::render()
{
	SDL_Point pos;
	pos.x = START_X;
	pos.y = START_Y;
	for (int i = 0; i < mLength; i++) {
		if (mLetters[i] != NULL) {
			mLetters[i]->render(pos);
		}
	}
}

int Input::addIndex(int index) 
{
	if (index >= mLength - 1) {
		return index;
	}
	return ++index;
}

int Input::subtractIndex(int index) 
{
	if (index <= -1) {
		return index;
	}
	return --index;
}

void Input::deleteIndex(int index)
{
	if (index >= 0 && index < mLength) {
		delete mLetters[index];
		mLetters.erase(mLetters.begin() + index);
		mLength--;
	}
}

void Input::updateFont(Font* font) 
{
	for (int i = 0; i < int(mLetters.size()); i++) {

		if (mLetters[i] == NULL) {
			continue;
		}

		float scale = mLetters[i]->getScale();
		int kerning = mLetters[i]->getKerning();
		int angle = mLetters[i]->getAngle();

		switch (EDIT_STATE) {
		case 0:
			scale = (float)font->getSize() / 20.0f + .75f;
			break;
		case 1:
			scale = getScale(font->getSize());
			break;
		case 2:
			kerning = font->getKerning();
			break;
		case 3:
			kerning = getKerning(font->getKerning());
			break;
		case 4:
			angle = getAngle();
		}
		
		mLetters[i]->update(font, scale, angle, kerning);
	}
}


void Input::changeFont(Font* font) 
{
	for (int i = 0; i < int(mLetters.size()); i++) {

		if (mLetters[i] == NULL) {
			continue;
		}

		int index = mLetters[i]->getIndex();
		int value = index - (index / 54) * 54;

		if (index >= 0) {
			mLetters[i]->setIndex((rand() % font->getRepeats() * 54) + value);
		}

		float scale = getScale(font->getSize());
		int kerning = getKerning(font->getKerning());
		int angle = getAngle();

		mLetters[i]->update(font, scale, angle, kerning);
	}
}

SDL_Point Input::getPosition(int index) 
{
	if (index >= 0 && index < mLength) {
		return mLetters[index]->getPosition();
	}
	else {
		SDL_Point pos;
		pos.x = START_X;
		pos.y = START_Y;
		return pos;
	}
}

float Input::getScale(int fontSize) 
{
	float scale = (float) fontSize / 20.0f + .75f;

	float randomValue = RANDOM_SCALE / 40.0f;
	float scaleDelta = (float)rand() / (float)(RAND_MAX / randomValue);
	
	int direction = rand() % 2;
	if (direction == 1) {
		scale = scale * (1.0f - scaleDelta);
	}
	else {
		scale = scale * (1.0f + scaleDelta);
	}
	
	return scale;
}

int Input::getKerning(int kerning) 
{
	float randomValue = RANDOM_ALPHA;
	float kerningDelta = (float)rand() / (float)(RAND_MAX / randomValue);

	int direction = rand() % 2;
	
	if (direction == 1) {
		kerning = kerning + kerningDelta;
	}
	else {
		kerning = kerning - kerningDelta;
	}

	return kerning;
}

int Input::getAngle() 
{
	float randomValue = RANDOM_ANGLE;
	int angle = (float)rand() / (float)(RAND_MAX / (randomValue * 2.0f)) - randomValue;

	return angle;
}

void Input::free() 
{
	for (int i = 0; i < mLength; i++) {
		delete mLetters[i];
		mLetters[i] = NULL;
	}
}