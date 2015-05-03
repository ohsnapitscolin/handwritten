#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

enum {WHITE, BLACK, CYAN};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LINE_HEIGHT = 101;

int hasBlack(std::vector<std::vector<Uint32>> pixelGrid, int letterRow, int column, Uint32 white, Uint32 cyan);
void convert(std::vector<SDL_Rect> boxes);

class LTexture
{
public:
	LTexture();
	~LTexture();

	bool loadFromFile(std::string path);

	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();

	bool lockTexture();
	bool unlockTexture();
	void* getPixels();
	int getPitch();

private:
	SDL_Texture* mTexture;
	void* mPixels;
	int mPitch;

	int mWidth;
	int mHeight;
};


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture gFontTexture;


LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;
}

LTexture::~LTexture() {
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
		SDL_Surface* formattedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(gWindow)->format, NULL);
		if (formattedSurface == NULL) {
			printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
		}
		else {
			newTexture = SDL_CreateTexture(gRenderer, SDL_GetWindowPixelFormat(gWindow), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
			if (newTexture == NULL) {
				printf("Unable to create blank texture! SDL Error: %s\n", SDL_GetError());
			}
			else {
				SDL_LockTexture(newTexture, NULL, &mPixels, &mPitch);

				memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

				SDL_UnlockTexture(newTexture);
				mPixels = NULL;

				mWidth = formattedSurface->w;
				mHeight = formattedSurface->h;
			}
			SDL_FreeSurface(formattedSurface);
		}
		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::free()
{
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mPixels = NULL;
		mPitch = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() {
	return mWidth;
}

int LTexture::getHeight() {
	return mHeight;
}

bool LTexture::lockTexture()
{
	bool success = true;

	if (mPixels != NULL) {
		printf("Texture is already locked!\n");
		success = false;
	}
	else {
		if (SDL_LockTexture(mTexture, NULL, &mPixels, &mPitch) != 0) {
			printf("Unable to lock texture! %s\n", SDL_GetError());
			success = false;
		}
	}

	return success;
}

bool LTexture::unlockTexture()
{
	bool success = true;

	if (mPixels == NULL) {
		printf("Texture is not locked!\n");
		success = false;
	}
	else {
		SDL_UnlockTexture(mTexture);
		mPixels = NULL;
		mPitch = 0;
	}

	return success;
}

void* LTexture::getPixels() {
	return mPixels;
}

int LTexture::getPitch() {
	return mPitch;
}

bool init();
bool loadMedia();
void close();


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

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadLetters()
{
	bool success = true;

	if (!gFontTexture.loadFromFile("font.png")) {
		printf("Failed to load corner texture!\n");
		success = false;
	}
	else {
		if (!gFontTexture.lockTexture()) {
			printf("Unable to lock font texture!\n");
		}
		else {
			Uint32* pixels = (Uint32*)gFontTexture.getPixels();
			int pixelCount = (gFontTexture.getPitch() / 4) * gFontTexture.getHeight();

			int width = gFontTexture.getWidth();
			int height = gFontTexture.getHeight();

			std::vector<std::vector<Uint32>> pixelGrid;
			pixelGrid.resize(height, std::vector<Uint32>(width));

			Uint32 white = SDL_MapRGB(SDL_GetWindowSurface(gWindow)->format, 255, 255, 255);
			Uint32 cyan = SDL_MapRGB(SDL_GetWindowSurface(gWindow)->format, 0, 255, 255);

			int k = 0;
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					pixelGrid[i][j] = pixels[k];
					k++;
				}
			}
			
			std::vector<SDL_Rect> boxes;

			int current = 0;
			int rightEdge = 0;
			int leftEdge = 0;
			int state = 1;

			int rowCount = height / LINE_HEIGHT;
			printf("%d\n", rowCount);
			for (int i = 0; i < rowCount; i++) {
				for (int j = 0; j < width; j++) {
					printf("%d %d\n", i, j);
					int value = hasBlack(pixelGrid, i, j, white, cyan);
					if (value == 1) {
						if (state == 1) {
							leftEdge = j;
							state = 2;
						}
						if (state == 2) {
							rightEdge = j;
						}
					}
					if (value == 2) {
						SDL_Rect currentBox;
						currentBox.x = leftEdge;
						currentBox.y = i * LINE_HEIGHT;
						currentBox.w = rightEdge - leftEdge;
						currentBox.h = LINE_HEIGHT - 1;

						if (currentBox.w < 10) {
							currentBox.w = 10;
						}
						
						printf("%d %d %d %d\n", currentBox.x, currentBox.y, currentBox.w, currentBox.h);
						boxes.push_back(currentBox);

						state = 1;
					}
				}
			}

			convert(boxes);
			gFontTexture.unlockTexture();
		}
	}

	return success;
}

void close()
{
	gFontTexture.free();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

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
			printf("Failed to load Letters!\n");
		}
	}

	close();
	return 0;
}

int hasBlack(std::vector<std::vector<Uint32>> pixelGrid, int letterRow, int column, Uint32 white, Uint32 cyan) 
{
	int type = WHITE;

	for (int i = 0; i < LINE_HEIGHT; i++) {
		if (pixelGrid[i + (LINE_HEIGHT * letterRow)][column] != white) {
			if (pixelGrid[i + (LINE_HEIGHT * letterRow)][column] == cyan) {
				type = CYAN;
			}
			else {
				type = BLACK;
			}
			break;
		}
	}
	return type;
}

void convert(std::vector<SDL_Rect> boxes) 
{
	int total =  int(boxes.size());

	std::ofstream hedaer("header.txt", std::ios::out | std::ios::trunc);

	hedaer << total << '\n';

	for (int i = 0; i < int(boxes.size()); i++) {
		hedaer << boxes[i].x << ' ';
		hedaer << boxes[i].y << ' ';
		hedaer << boxes[i].w << ' ';
		hedaer << boxes[i].h << '\n';
	}
	hedaer.close();
}