#ifndef INPUT_H_DEFINED
#define INPUT_H_DEFINED

#include "SDL.h"
#include "letter.h"
#include "font.h"

class Input 
{
public:
	Input();
	void free();

	void addLetter(int index, char character, Font* font);
	
	int addIndex(int index);
	int subtractIndex(int index);
	void deleteIndex(int index);

	void render();

	void changeFont(Font* font);
	void updateFont(Font* font);

	SDL_Point getPosition(int index);

private:
	int mLength;

	float getScale(int fontSize);
	int getKerning(int kerning);
	int getAngle();

	std::vector<Letter*> mLetters;
};
#endif