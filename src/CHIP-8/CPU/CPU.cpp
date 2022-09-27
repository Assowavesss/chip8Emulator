/**
 * @file  CPU.hpp
 * @brief Initialization and instructions of CHIP-8
 * @see   http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */
#include "CPU.hpp"
#include <time.h>
#include <cstring>

static constexpr const unsigned char chip8_fontset[NUMBER_FONTSET] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/**
 * @brief Initialization of all component of CHIP-8
 * @see   CPU.hpp
 */
CPU::CPU(void)
{
	// Initialization of program counter at 0x200
	pc     = START_ADRESS;
	
	opcode = 0;
	I      = 0;
	sp     = 0;
	
	// Clear the display
	memset(gfx,0,sizeof(gfx));
	
	for(unsigned i=0; i<NUMBER_REGISTER;++i)
	{
		stack[i] = 0;
		key[i]   = 0;
		V[i]	 = 0;
	}
	
	// Clear memory
	memset(memory,0,MEMORY_SIZE);
	
	// Load fontset
	for(unsigned i=0; i<NUMBER_FONTSET; ++i){
		memory[i] = chip8_fontset[i];
	}
	
	delay_timer = 0;
	
	// To control the choice of the seed
	srand(time(NULL));
}

/*
 * A 12-bit value, the lowest 12 bits of the instruction
 */
#define nnn (opcode & 0x0FFF)


/*
 * A 4-bit value, the lower 4 bits of the high byte of the instruction
 */
#define x ((opcode & 0x0F00) >> 8)

/*
 * A 4-bit value, the upper 4 bits of the low byte of the instruction
 */
#define y ((opcode & 0x00F0) >> 4)

/*
 * An 8-bit value, the lowest 8 bits of the instruction
 */
#define kk (opcode & 0x00FF)

/* List of Instructions : http://devernay.free.fr/hacks/chip8/C8TECH10.HTM */

/**
 * @brief Clear the display
 */
void CPU::OP_00E0(void){ 
	memset(gfx,0,sizeof(gfx));    
}

/**
 * @brief Return from a subroutine
 * @details
 * The interpreter sets the program counter to the address at the top of the stack 
 * then subtracts 1 from the stack pointer
 */
void CPU::OP_00EE(void)
{ 
	sp--; 
	pc=stack[sp];          
}

/**
 * @brief Jump to location nnn
 * @details
 * The interpreter sets the program counter to nnn
 */
void CPU::OP_1nnn(void){ 
	pc = nnn;                     
}

/**
 * @brief Call subroutine at nnn
 * @details
 * The interpreter increments the stack pointer
 * then puts the current PC on the top of the stack 
 * the PC is then set to nnn
 */
void CPU::OP_2nnn(void)
{ 
	stack[sp]=pc; 
	++sp; 
	pc = nnn; 
}

/**
 * @brief Skip next instruction if Vx = kk
 * @details
 * The interpreter compares register Vx to kk 
 * and if they are equal increments the program counter by 2
 */
void CPU::OP_3xkk(void){ 
	pc+= (V[x] == kk) ? 2 : 0;    
}

/**
 * @brief Skip next instruction if Vx != kk
 * @details
 * The interpreter compares register Vx to kk
 * and if they are not equal, increments the program counter by 2
 */
void CPU::OP_4xkk(void){ 
	pc+= (V[x] != kk) ? 2 : 0;   
}

/**
 * @brief Skip next instruction if Vx = Vy
 * @details
 * The interpreter compares register Vx to register Vy
 * and if they are equal, increments the program counter by 2
 */
void CPU::OP_5xy0(void){ 
	pc+= (V[x] == V[y]) ? 2 : 0; 
}

/**
 * @brief Set Vx = kk
 * @details
 * The interpreter puts the value kk into register Vx
 */
void CPU::OP_6xkk(void){ 
	V[x] = kk;					 
}

/**
 * @brief Set Vx = Vx + kk
 * @details
 * Adds the value kk to the value of register Vx 
 * then stores the result in Vx
 */
void CPU::OP_7xkk(void){ 
	V[x] += kk;                    
}

/**
 * @brief Set Vx = Vy
 * @details
 * Stores the value of register Vy in register Vx
 */
void CPU::OP_8xy0(void){ 
	V[x] = V[y];                   
}

/**
 * @brief Set Vx = Vx OR Vy
 * @details
 * Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx
 * A bitwise OR compares the corrseponding bits from two values and if either bit is 1
 * then the same bit in the result is also 1. Otherwise, it is 0
 */
void CPU::OP_8xy1(void){ 
	V[x] |= V[y];                  
}



/**
 * @brief Set Vx = Vx AND Vy
 * @details
 * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx
 * A bitwise AND compares the corrseponding bits from two values and if both bits are 1
 * then the same bit in the result is also 1. Otherwise, it is 0
 */
void CPU::OP_8xy2(void){ 
	V[x] &= V[y];                  
}

/** 
 * @brief Set Vx = Vx XOR Vy
 * @details
 * Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx
 * An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same
 * then the corresponding bit in the result is set to 1. Otherwise, it is 0
 */
void CPU::OP_8xy3(void){ 
	V[x] ^= V[y];                  
}

/**
 * @brief Set Vx = Vx + Vy, set VF = carry
 * @details
 * The values of Vx and Vy are added together
 * If the result is greater than 8 bits
 * F is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx
 */
void CPU::OP_8xy4(void)
{ 
	V[x]  += V[y]; 
	V[0xF] = (V[x] > 0xFF) ? 1 : 0; 
}

/**
 * @brief Set Vx = Vx - Vy, set VF = NOT borrow
 * @details
 * If Vx > Vy, then VF is set to 1, otherwise 0
 * Then Vy is subtracted from Vx, and the results stored in Vx
 */
void CPU::OP_8xy5(void)
{ 
	V[0xF] = (V[x] > V[y]) ? 1 : 0; 
	V[x]  -= V[y]; 
}

/** 
 * @brief Set Vx = Vx SHR 1
 * @details
 * If the least-significant bit of Vx is 1 
 * then VF is set to 1, otherwise 0
 * Then Vx is divided by 2
 */
void CPU::OP_8xy6(void)
{ 
	V[0xF] = V[x] & 0x1; 
	V[x] >>= 1;               
}

/**
 * @brief Set Vx = Vy - Vx, set VF = NOT borrow
 * @details
 * If Vy > Vx, then VF is set to 1, otherwise 0
 * Then Vx is subtracted from Vy, and the results stored in Vx
 */
void CPU::OP_8xy7(void)
{ 
	V[0xF] = (V[y] > V[x]) ? 1 : 0; 
	V[x]   = V[y] - V[x]; 
}

/**
 * @brief Set Vx = Vx SHL 1
 * @details
 * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0
 * Then Vx is multiplied by 2
 */
void CPU::OP_8xyE(void)
{ 
	V[0xF] = V[x] >> 7; 
	V[x] <<= 1;                        
}

/**
 * @brief Skip next instruction if Vx != Vy
 * @details
 * The values of Vx and Vy are compared, and if they are not equal, 
 * the program counter is increased by 2
 */
void CPU::OP_9xy0(void){ 
	pc += (V[x] != V[y]) ? 2 : 0;                          
}

/**
 * @brief Set I = nnn
 * @details
 * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0 
 * Then Vx is multiplied by 2
 */
void CPU::OP_Annn(void){ 
	I 	= nnn;                                                
}

/**
 * @brief Jump to location nnn + V0
 */
void CPU::OP_Bnnn(void){ 
	pc = nnn + V[0]; 
}

/**
 * @brief Set Vx = random byte AND kk
 * @details
 * The interpreter generates a random number from 0 to 255 which is then ANDed with the value kk
 * The results are stored in Vx. See instruction 8xy2 for more information on AND
 */
void CPU::OP_Cxkk(void){ 
	V[x] = (rand() % (0xFF + 1)) & kk;
}

/**
 * @brief Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
 * @details
 * The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then
 * displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If
 * this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is
 * positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side
 * of the screen.
 */
void CPU::OP_Dxyn(void)
{
	unsigned short X = V[x];
	unsigned short Y = V[y];
	unsigned short height = opcode & 0x000F;
	unsigned short pixel;

	V[0xF] = 0;
	for (unsigned yline = 0; yline < height; yline++)
	{
		pixel = memory[I + yline];
		for(unsigned xline = 0; xline < 8; xline++)
		{
			if((pixel & (l*2 >> xline)) != 0)
			{
				if(gfx[(X + xline + ((Y + yline) * l))] == 1)
				{
					V[0xF] = 1;
				}
				gfx[X + xline + ((Y + yline) * l)] ^= 1;
			}
		}
	}
	
}

/**
 * @brief Skip next instruction if key with the value of Vx is pressed
 * @details
 * Checks the keyboard, and if the key corresponding to the value of Vx
 * is currently in the down position, PC is increased by 2
 */
void CPU::OP_Ex9E(void){
	pc += key[V[x]] ? 2 : 0;
}

/**
 * @brief Skip next instruction if key with the value of Vx is not pressed
 * @details
 * Checks the keyboard, and if the key corresponding to the value of Vx is currently
 * in the up position, PC is increased by 2
 */
void CPU::OP_ExA1(void){
	pc += !key[V[x]] ? 2 : 0;
}

/**
 * @brief Set Vx = delay timer value
 * @details
 * The value of DT is placed into Vx
 */
void CPU::OP_Fx07(void){
	V[x] = delay_timer;
}

/**
 * @brief Wait for a key press, store the value of the key in Vx
 * @details 
 * All execution stops until a key is pressed, then the value of that key is stored in Vx
 */
void CPU::OP_Fx0A(void)
{
	bool key_pressed = false;

	for(unsigned i = 0; i < NUMBER_REGISTER; ++i)
	{
		if(key[i] != 0)
		{
			V[x] = i;
			key_pressed = true;
		}
	}

	// If no key is pressed, decrement and try again.
	if(!key_pressed){
		pc -=2;
	}
}

/**
 * @brief Set delay timer = Vx
 * @details
 * Delay Timer is set equal to the value of Vx
 */
void CPU::OP_Fx15(void){
	delay_timer = V[x];
}

void CPU::OP_Fx18(void){
	sound_timer=V[x];	
}

/**
 * @brief Set I = I + Vx
 */
void CPU::OP_Fx1E(void){
	I += V[x];
}

/**
 * @brief Set I = location of sprite for digit Vx
 * @details
 * The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx
 */
void CPU::OP_Fx29(void){
	I = V[x] * 0x5;
}

/** 
 * @brief Store BCD representation of Vx in memory locations I, I+1, and I+2
 * @details
 * The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I
 * the tens digit at location I+1, and the ones digit at location I+2
 */
void CPU::OP_Fx33(void)
{
	memory[I]     = V[x] / 100;
	memory[I + 1] = (V[x] / 10) % 10;
	memory[I + 2] = V[x] % 10;
	
}

/** 
 * @brief Store registers V0 through Vx in memory starting at location I
 * @details
 * The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I
 */
void CPU::OP_Fx55(void)
{
	for (int i = 0; i <= (x); ++i){
		memory[I + i] = V[i];
	}
}

/**
 * @brief Read registers V0 through Vx from memory starting at location I
 * @details 
 * The interpreter reads values from memory starting at location I into registers V0 through Vx
 */
void CPU::OP_Fx65(void)
{
	for (int i = 0; i <= (x); ++i){
		V[i] = memory[I + i];
	}
}