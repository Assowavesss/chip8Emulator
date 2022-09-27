/**
 * @file CPU.hpp
 * @brief Composition and instructions of CPU of CHIP-8 emulator !
 * @author Jonathan Elbaz
 */
 
#ifndef CPU_HPP
#define CPU_HPP

#include<iostream>

/*
 * Using type to contain into variable 8,16 or 32 bytes
 */
#include<stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;

/*
 * CHIP-8 contain 4096 octets of memory
 */
#define MEMORY_SIZE 4096

/*
 * CHIP-8 contain 16 register which can hold any value form 0x00 to 0xFF
 */
#define NUMBER_REGISTER 16

/*
 * CHIP-8 contain 80 fontsets to display 0,1,2...F
 */
#define NUMBER_FONTSET 80

/*
 * The instructions of departure is 512 octets <=> 0x200
 */
#define START_ADRESS 0x200

/*
 * Number of pixel depending of the height
 */
#define l 64

/*
 * Number of pixel dependig of the height
 */
#define L 32

struct CPU
{
	/*
	 * List of registers Vx which can be (V0,V1,V2 .... VF)
	 */
	u8 V[NUMBER_REGISTER];
	
	/*
	 * Memory of CHIP-8 which contain Interpreter, Hexadecimal Number and Instruction
	 */
	u8 memory[MEMORY_SIZE];
	
	/*
	 * Delay timer which decrease of 60 hertz when is more than 0
	 */
	u8 delay_timer;
	/* We can't use sound timer */
	u8 sound_timer;
	
	/*
	 * Stack pointer to browsing stack
	 */
	u8 sp;
	
	/*
	 * Keypad of CHIP-8 which contain 16 key 
	 */
	u8 key[NUMBER_REGISTER];
	
	/*
	 * Graphic buffer
	 */
	u8 gfx[l*L];
	
	/*
	 * Index of register to store memory adress
	 */
	u16 I;
	
	/*
	 * Program counter to hold adress of next instruction
	 */
	u16 pc;
	
	/*
	 * Stack which contain different pc
	 */
	u16 stack[NUMBER_REGISTER];
	
	/*
	 * opcode store adress of instruction 
	 */
	u16 opcode;

	CPU(void);
	
	/* List of instructions */
	void OP_00E0(void);
	void OP_00EE(void);
	void OP_1nnn(void);
	void OP_2nnn(void);
	void OP_3xkk(void);
	void OP_4xkk(void);
	void OP_5xy0(void);
	void OP_6xkk(void);
	void OP_7xkk(void);
	void OP_8xy0(void);
	void OP_8xy1(void);
	void OP_8xy2(void);
	void OP_8xy3(void);
	void OP_8xy4(void);
	void OP_8xy5(void);
	void OP_8xy6(void);
	void OP_8xy7(void);
	void OP_8xyE(void);
	void OP_9xy0(void);
	void OP_Annn(void);
	void OP_Bnnn(void);
	void OP_Cxkk(void);
	void OP_Dxyn(void);
	void OP_Ex9E(void);
	void OP_ExA1(void);
	void OP_Fx07(void);
	void OP_Fx0A(void);
	void OP_Fx15(void);
	void OP_Fx18(void);
	void OP_Fx1E(void);
	void OP_Fx29(void);
	void OP_Fx33(void);
	void OP_Fx55(void);
	void OP_Fx65(void);
};

#endif