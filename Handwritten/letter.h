#ifndef LETTER_H_DEFINED
#define LETTER_H_DEFINED

#include "SDL.h"
#include "font.h"

class Letter 
{
public:
	Letter(Font* font, int index);
	Letter(Font* font, int index, float size, int angle, int kerning);

	void render(SDL_Point& pos);
	
	void setIndex(int index);

	void update(Font* font, float size, int angle, int kerning);
	
	SDL_Point getPosition();

	int getIndex();
	float getScale();
	int getKerning();
	int getAngle();
	Font* getFont();

private:
	SDL_Point mPoint;

	int mIndex;
	float mSize;
	int mKerning;
	int mAngle;
	Font* mFont;
};
#endif