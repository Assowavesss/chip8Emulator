#include "GUI.hpp"

/*
 * Controller of CHIP-8
 */
static constexpr const u8 keymap[NUMBER_REGISTER] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};

/**
 * @brief Initialization of SDL
 * @see   main.cpp
 */
void Init_GUI(void)
{
	if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) 
	{
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }
}

/**
 * @brief Creation of window
 * @see   main.cpp
 * @return window
 */
SDL_Window *Create_Window(void)
{
	SDL_Window *window=nullptr;
	window = SDL_CreateWindow(
            "CHIP-8 Emulator",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIDTH, HEIGHT, SDL_WINDOW_SHOWN );
	
	if (!window)
	{
        printf( "Window could not be created! SDL_Error: %s\n",SDL_GetError());
        exit(2);
    }
	
	return window;
}

/**
 * @brief Creation of renderer to fill our window
 * @see   main.cpp
 * @param window
 * @return renderer
 */
SDL_Renderer *Create_Renderer(SDL_Window* window)
{
	SDL_Renderer *renderer=nullptr;
	renderer=SDL_CreateRenderer(window, -1, 0);
	return renderer;
}

/**
 * @brief Creation of pixel, square black and white
 * @see   main.cpp
 * @param renderer
 * @return texture
 */
SDL_Texture *Create_Texture(SDL_Renderer* renderer)
{
	SDL_Texture* Texture =nullptr;
	Texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            l, L);
	return Texture;
}

/**
 * @brief Manipulation of keydown and keyup
 * @see main.cpp
 * @param chip8 which can modificate the value of key
 */
void Manage_Events(CHIP_8 &chip8)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		// Exit program when user click to cross x
		if (e.type == SDL_QUIT){ 
			exit(0);
		}
		
		// Process keydown events
		if (e.type == SDL_KEYDOWN) 
		{
			// Exit program when user click to escape
			if (e.key.keysym.sym == SDLK_ESCAPE){
				exit(0);
			}
			
			for (unsigned i = 0; i < NUMBER_REGISTER; ++i) 
			{
				if (e.key.keysym.sym == keymap[i]) {
					chip8.cpu.key[i] = 1;
				}
			}
		}
		// Process keyup events
		if (e.type == SDL_KEYUP) 
		{
			for (unsigned i = 0; i < NUMBER_REGISTER; ++i) 
			{
				if (e.key.keysym.sym == keymap[i]) {
					chip8.cpu.key[i] = 0;
				}
			}
		}
	}
}

/**
 * @brief Redraw screen
 * @see main.cpp
 * @param chip8, pixels, texture, renderer
 */
void Redraw_Screen(CHIP_8 &chip8,uint32_t* pixels,SDL_Texture *texture,SDL_Renderer *renderer)
{
	for (int i = 0; i < l*L; ++i) 
	{
		uint8_t pixel = chip8.cpu.gfx[i];
		pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
	}
	// Update SDL texture
	SDL_UpdateTexture(texture, NULL, pixels, l * sizeof(Uint32));
	// Clear screen and render
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
