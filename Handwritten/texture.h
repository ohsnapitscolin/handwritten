#ifndef TEXTURE_H_DEFINED
#define TEXTURE_H_DEFINED

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>

class LTexture 
{
public:
	LTexture();
	~LTexture();

	void free();

	bool loadFromFile(std::string path);
	bool loadFromRenderedText(TTF_Font* font, std::string textureText, SDL_Color textColor);

	void clipSprite(int width, int height);

	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	void setScale(float scale);
	void setOffset(int x, int y);

	void renderSprite(int x, int y, int currentSprite, int angle = 0, bool flip = false);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	

	int getWidth();
	int getHeight();
	
	int getSpriteCount();
	int getSpriteWidth();
	int getSpriteHeight();
private:
	SDL_Texture* mTexture;

	std::vector<SDL_Rect> mSpriteSheet;

	int mSpriteCount;

	int mSpriteWidth;
	int mSpriteHeight;

	int mWidth;
	int mHeight;

	float mScale;

	int mOffsetX;
	int mOffsetY;
};

#endif