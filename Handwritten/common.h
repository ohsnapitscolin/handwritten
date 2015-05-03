#ifndef COMMON_H
#define COMMON_H

#include <SDL.h>
#include <map>

#define SPACE -1
#define RETURN - 2

extern SDL_Renderer* gRenderer;

extern int LEVEL_WIDTH;
extern int LEVEL_HEIGHT;
extern int SCREEN_HEIGHT;
extern int SCREEN_WIDTH;

extern float RANDOM_SCALE;
extern float RANDOM_ANGLE;
extern float RANDOM_ALPHA;
extern int EDIT_STATE;

extern int LINE_HEIGHT;
extern int START_X;
extern int START_Y;

extern int SPACE_WIDTH;

extern std::map<char, int> myMap;
#endif