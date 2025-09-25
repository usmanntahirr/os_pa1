#ifndef _SYSCALL_H
#define _SYSCALL_H
//*****************************************************************************
//*
//*  @file		syscall.h
//*  @author    Abdul Rafay (abdul.rafay@lums.edu.pk)
//*  @brief		System call interface header file
//*  @version	
//*
//****************************************************************************/
//-----------------------------------------------------------------------------
// 		REQUIRED HEADERS
//-----------------------------------------------------------------------------

#include <stddef.h>

//-----------------------------------------------------------------------------
// 		INTERFACE DEFINES/TYPES
//-----------------------------------------------------------------------------

//! enum to define syscall numbers
typedef enum {

	SYSCALL_READ = 0,   //? syscall number for read: 0
	SYSCALL_WRITE,      //? syscall number for write: 1
	SYSCALL_FORK,       //? syscall number for fork: 2

	/* REST will be added as the project progresses */
	
} syscall_nr;

//-----------------------------------------------------------------------------
// 		INTERFACE DATA STRUCTURES
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 		INTERFACE FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

//! initialize the system's syscall interface
void syscall_init (void);

//*****************************************************************************
//**
//** 	END _[filename]
//**
//*****************************************************************************
#endif //! _SYSCALL_H