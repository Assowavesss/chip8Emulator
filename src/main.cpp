/**
 * @file main.cpp
 * @see inspired by https://github.com/JamesGriffin/CHIP-8-Emulator
 */
#include "GUI/GUI.hpp"

int main(int argc, char **argv)
{
	// Command usage
    if (argc != 2) {
        std::cout << "Usage: chip8 <ROM file>" << std::endl;
        return 1;
    }

    CHIP_8 chip8 = CHIP_8();
	
	Init_GUI();
   
    SDL_Window* window = Create_Window();
	
	// Create renderer
    SDL_Renderer *renderer =Create_Renderer(window);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    // Create texture that stores frame buffer
    SDL_Texture* sdlTexture = Create_Texture(renderer);

    // Temporary pixel buffer
    uint32_t pixels[l*L];
	
	// Attempt to load ROM
    if (!chip8.load(argv[1]))
        return 2;
		
	
    // Emulation loop
    for(;;) {
        chip8.emulate_cycle();

        // Process SDL events
        Manage_Events(chip8);

        // If draw occurred, redraw SDL screen
        if (chip8.drawFlag) 
		{
            chip8.drawFlag = false;
			Redraw_Screen(chip8,pixels,sdlTexture,renderer);
        }

		SDL_Delay(5);

    }
	SDL_Quit();
	free(window);
	free(renderer);
	free(sdlTexture);
}