#include "texture.h"
#include "common.h"

LTexture::LTexture()
{
	mTexture = NULL;

	mWidth = 0;
	mHeight = 0;

	mOffsetX = 0;
	mOffsetY = 0;

	mScale = 1;
}

LTexture::~LTexture()
{
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(TTF_Font* font, std::string textureText, SDL_Color textColor)
{
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface != NULL) {
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	else {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}

	return mTexture != NULL;
}

void LTexture::clipSprite(int width, int height)
{
	int totalRows = mHeight / height;
	int totalColumns = mWidth / width;
	mSpriteCount = totalRows * totalColumns;

	mSpriteSheet.resize(mSpriteCount);

	int currentRow = 0;
	for (int i = 0; i < mHeight; i += height) {
		int currentColumn = 0;
		for (int j = 0; j < mWidth; j += width) {
			int currentSprite = currentRow * totalColumns + currentColumn;
			mSpriteSheet[currentSprite].x = j;
			mSpriteSheet[currentSprite].y = i;
			mSpriteSheet[currentSprite].w = width;
			mSpriteSheet[currentSprite].h = height;
			currentColumn++;
		}
		currentRow++;
	}

	mSpriteHeight = height;
	mSpriteWidth = width;
}

void LTexture::setOffset(int x, int y)
{
	mOffsetX = x;
	mOffsetY = y;
}

void LTexture::setScale(float scale) {
	mScale = scale;
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

int LTexture::getWidth() {
	return mWidth * mScale;
}

int LTexture::getHeight() {
	return mHeight * mScale;
}

int LTexture::getSpriteWidth() {
	return mSpriteWidth;
}

int LTexture::getSpriteHeight() {
	return mSpriteHeight;
}

int LTexture::getSpriteCount() {
	return mSpriteCount;
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { mOffsetX + x, mOffsetY + y, mWidth * mScale, mHeight * mScale };

	if (clip != NULL) {
		renderQuad.w = clip->w * mScale;
		renderQuad.h = clip->h * mScale; 
	}

	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::renderSprite(int x, int y, int currentSprite, int angle, bool flip) 
{
	SDL_Point center = { x + mSpriteWidth / 2, y + mSpriteHeight / 2 };
	SDL_RendererFlip flipType = SDL_FLIP_NONE;
	if (flip) {
		flipType = SDL_FLIP_HORIZONTAL;
	}

	if (currentSprite >= int(mSpriteSheet.size())) {
		currentSprite = 0;
	}

	render(x, y, &mSpriteSheet[currentSprite], angle, &center, flipType);
}

void LTexture::free()
{
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
	}
}



