#include "CHIP_8.hpp"

/*
 * Using manipulation of file fopen(), fread() etc.
 */
#include <cstdio>

/**
 * @brief Load a rom and store element into memory
 * @details return true if rom is correctely load else false
 * @param path of the rom file
 * @return boolean
 */
bool CHIP_8::load(const char*file_path)
{
	std::cout<<"Loading Rom : "<<file_path<<"\n";

    // Open ROM file
    FILE* rom = fopen(file_path, "rb");
    if (!rom) 
	{
        std::cerr << "Failed to open ROM" << std::endl;
        return false;
    }

    // Get file size
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);

    // Allocate memory to store rom
    char* rom_buffer = new char[rom_size];
    if (!rom_buffer) 
	{
        std::cerr << "Failed to allocate memory for ROM" << std::endl;
        return false;
    }

    // Copy ROM into buffer
    size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, rom);
    if (result != (size_t)rom_size) 
	{
        std::cerr << "Failed to read ROM" << std::endl;
        return false;
    }

    // Copy buffer to memory
    if ((MEMORY_SIZE-START_ADRESS) > rom_size)
	{
        for (int i = 0; i < rom_size; ++i) 
		{
            // Load into memory starting
			cpu.memory[i + START_ADRESS] = (u8)rom_buffer[i];   
        }
    }
    else 
	{
        std::cerr << "ROM too large to fit in memory" << std::endl;
        return false;
    }

    // Clean up
    fclose(rom);
    delete rom_buffer;
    return true;
}

/**
 * @brief Jump opcode depending to memory and program counter and execute instruction
 * @see CPU.cpp
 */
void CHIP_8::emulate_cycle(void) 
{
	// Fetch op code
    cpu.opcode = cpu.memory[cpu.pc] << 8 | cpu.memory[cpu.pc + 1];   // Op code is two bytes
	
    cpu.pc+=2;
	switch(cpu.opcode & 0xF000)
	{
		case 0x0000:
				switch (cpu.opcode & 0x000F) 
				{
					case 0x0000:
						cpu.OP_00E0();
						drawFlag=true;
						break;

					case 0x000E:
						cpu.OP_00EE();
						break;

					default:
						printf("\nUnknown op code: %.4X\n", cpu.opcode);
						exit(3);
				}
        break;
		
        case 0x1000:
            cpu.OP_1nnn();
            break;

		case 0x2000:
            cpu.OP_2nnn();
            break;

        case 0x3000:
            cpu.OP_3xkk();
            break;

        case 0x4000:
            cpu.OP_4xkk();
            break;

        case 0x5000:
            cpu.OP_5xy0();
            break;

        case 0x6000:
            cpu.OP_6xkk();
            break;

        case 0x7000:
            cpu.OP_7xkk();
            break;

        case 0x8000:
            switch (cpu.opcode & 0x000F) 
			{
				case 0x0000:
                    cpu.OP_8xy0();
                    break;

                case 0x0001:
                    cpu.OP_8xy1();
                    break;

                case 0x0002:
                    cpu.OP_8xy2();
                    break;

                case 0x0003:
                    cpu.OP_8xy3();
                    break;

                case 0x0004:
                    cpu.OP_8xy4();
                    break;

				case 0x0005:
                    cpu.OP_8xy5();
                    break;

				case 0x0006:
                    cpu.OP_8xy6();
                    break;

                case 0x0007:
                    cpu.OP_8xy7();
                    break;
				
				case 0x000E:
                    cpu.OP_8xyE();
                    break;

                default:
                    printf("\nUnknown op code: %.4X\n", cpu.opcode);
                    exit(3);
            }
            break;

        case 0x9000:
            cpu.OP_9xy0();
            break;

       case 0xA000:
            cpu.OP_Annn();
            break;

        case 0xB000:
            cpu.OP_Bnnn();
            break;

        case 0xC000:
            cpu.OP_Cxkk();
            break;

        case 0xD000:
			cpu.OP_Dxyn();
			drawFlag=true;
			break;

        case 0xE000:
			switch (cpu.opcode & 0x00FF) 
			{
                case 0x009E:
                    cpu.OP_Ex9E();
                    break;

				case 0x00A1:
                    cpu.OP_ExA1();
                    break;

                default:
                    printf("\nUnknown op code: %.4X\n", cpu.opcode);
                    exit(3);
            }
            break;

        case 0xF000:
            switch (cpu.opcode & 0x00FF)
            {
                case 0x0007:
                    cpu.OP_Fx07();
					break;

                case 0x000A:
					cpu.OP_Fx0A();
					break;

                case 0x0015:
                    cpu.OP_Fx15();
					break;

                case 0x0018:
                   break;

				case 0x001E:
                    cpu.OP_Fx1E();
					break;
					
				case 0x0029:
                    cpu.OP_Fx29();
					break;

                case 0x0033:
                    cpu.OP_Fx33();
					break;

				case 0x0055:
                    cpu.OP_Fx55();
					break;

                case 0x0065:
                    cpu.OP_Fx65();
					break;

                default:
                    printf ("Unknown opcode [0xF000]: 0x%X\n", cpu.opcode);
            }
            break;

        default:
            printf("\nUnimplemented op code: %.4X\n", cpu.opcode);
            exit(3);
    }
	
	// Update timers
    if (cpu.delay_timer > 0){
        --cpu.delay_timer;
	}
	
	if(cpu.sound_timer > 0){
		--cpu.sound_timer;
	}
}
	




	
	



	
	
	