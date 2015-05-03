#ifndef FONT_H_DEFINED
#define FONT_H_DEFINED

#include <SDL.h>
#include "texture.h"
#include <vector>

class Font {
public:
	Font(int number);

	void free();
	
	bool loadMedia();
	
	void render(int index, float scale, int angle, int kerning, SDL_Point& pos);
	void renderName(int x, int y);

	void updateSize(bool direction);
	void updateKerning(bool direction);

	bool containsIndex(int index);

	std::string getName();
	int getSize();
	int getKerning();
	int getRepeats();

private:
	std::string mName;
	
	TTF_Font* mFont;
	LTexture nameTexture;
	
	int mNumber;

	int mSize;
	int mKerning;
	int mRepeats;

	LTexture mFontTexture1;
	LTexture mFontTexture2;
	LTexture mFontTexture3;
	LTexture mFontTexture4;
	
	std::vector<SDL_Rect> mSprites;
};
#endif