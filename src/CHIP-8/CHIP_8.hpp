/**
 * @file CHIP_8.hpp
 * @brief Composition of CHIP-8
 * @see CHIP_8.cpp 
 */
#ifndef CHIP_8_HPP
#define CHIP_8_HPP
#include "CPU/CPU.HPP"

struct CHIP_8
{
	/*
	 * CPU of CHIP-8 which contains all instructions and components
	 */
	CPU cpu;
	
	/*
	 * To know if some pixels has drawn
	 */
	bool drawFlag;
	
	/**
	 * @brief Load a rom
	 * @see   CHIP_8.cpp
	 */
	bool load(const char*file_path);
	
	/**
	 * @brief Jump opcode and choose instructions
	 * @see   CHIP_8.cpp
	 * @see   CPU.cpp
	 */
	void emulate_cycle(void);
};



#endif