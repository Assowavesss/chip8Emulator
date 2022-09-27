#ifndef GUI_HPP
#define GUI_HPP
#include "../CHIP-8/CHIP_8.hpp"

/*
 * We use SDL as GUI
 */
#include <SDL.h>

/* Display Resolution */

/*
 * Window width
 */
#define WIDTH 1024

/*
 * Window height
 */
#define HEIGHT 512

/**
 * @brief Initialization of SDL
 * @see   GUI.cpp
 */
void Init_GUI(void);

/**
 * @brief Creation of window
 * @see   GUI.cpp
 * @return window
 */
SDL_Window 		*Create_Window(void);

/**
 * @brief Creation of renderer to fill our window
 * @see   GUI.cpp
 * @param window
 * @return renderer
 */
SDL_Renderer *Create_Renderer(SDL_Window* window);

/**
 * @brief Creation of pixel, square black and white
 * @see   GUI.cpp
 * @param renderer
 * @return texture
 */
SDL_Texture *Create_Texture(SDL_Renderer* renderer);

/**
 * @brief Manipulation of keydown and keyup
 * @see GUI.cpp
 * @param chip8 which can modificate the value of key
 */
void Manage_Events(CHIP_8 &chip8);

/**
 * @brief Redraw screen
 * @see GUI.cpp
 * @param chip8, pixels, texture, renderer
 */
void Redraw_Screen(CHIP_8 &chip8,uint32_t *pixels,SDL_Texture *texture,SDL_Renderer *renderer);

#endif