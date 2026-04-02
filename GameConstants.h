#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

// IDs for the game objects
const int IID_PLAYER = 0;
const int IID_LEMMING_FACTORY = 1;
const int IID_BARREL = 2;
const int IID_PHEROMONE = 3;
const int IID_ICE_MONSTER = 4;
const int IID_FLOOR = 5;
const int IID_FIREBALL = 6;
const int IID_TRAMPOLINE = 8;
const int IID_BONFIRE = 9;
const int IID_NET = 10;
const int IID_LEMMING = 11;
//const int IID_LEFT_ONE_WAY_DOOR = 12;
const int IID_ONE_WAY_DOOR = 12;
const int IID_EXIT = 13;
const int IID_SPRING = 14;
const int IID_DEMO_OBJECT = 15;  // used only in demo, not the game

// Sounds
const int SOUND_NONE = 0;
const int SOUND_PLAYER_DIE = 1;
const int SOUND_ENEMY_DIE = 3;
const int SOUND_LEMMING_SAVED = 4;
const int SOUND_FINISHED_LEVEL = 5;
const int SOUND_THEME = 6;
const int SOUND_BOUNCE = 7;
const int SOUND_LEMMING_DIE = 8;
const int SOUND_DEMO_SOUND = 9;  // used only in demo, not the game

// Keys the user can hit
const int KEY_PRESS_LEFT = 1000;
const int KEY_PRESS_RIGHT = 1001;
const int KEY_PRESS_UP = 1002;
const int KEY_PRESS_DOWN = 1003;
const int KEY_PRESS_TAB = '\t';
const int KEY_PRESS_SPACE = ' ';
const int KEY_PRESS_ESCAPE = '\x1b';

// Board dimensions
const int VIEW_WIDTH = 20;
const int VIEW_HEIGHT = 20;

// Game status
const int GWSTATUS_PLAYER_DIED = 0;
const int GWSTATUS_CONTINUE_GAME = 1;
const int GWSTATUS_PLAYER_WON = 2;
const int GWSTATUS_FINISHED_LEVEL = 3;
const int GWSTATUS_LEVEL_ERROR = 4;


// Score constants
const int SCORE_KONG_COMPLETION = 1000;
const int SCORE_TRAMPOLINE = 25;
const int SCORE_SAVED_LEMMING = 100;

// Game mechanics constants
const int TEXTURE_KEY_MULTIPLIER = 1000;
const int RANDOM_DIRECTION_CHOICES = 2;

// Display formatting constants
const int LEVEL_DISPLAY_WIDTH = 2;
const int SCORE_DISPLAY_WIDTH = 7;
const int LIVES_DISPLAY_WIDTH = 2;

// display constants
const double SPRITE_WIDTH_GL = 1; // Updated for SFML - sprites touch but don't overlap
const double SPRITE_HEIGHT_GL = 1; // Updated for SFML - sprites touch but don't overlap
const double DEFAULT_SPRITE_SCALE = 1.0;
const int MAX_SPRITE_DEPTH = 3; // Maximum depth level for sprite rendering (background)


#endif // GAMECONSTANTS_H_
