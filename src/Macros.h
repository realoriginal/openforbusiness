#pragma once

/* Cast as a function or a global variable within a specific location of the .text section */
#define D_SEC( x ) 	__attribute__(( section( ".text$" #x ) ))

/* Cast as a function with the typedef of the same name for virtual method tables */
#define D_API( x )	__typeof__( x ) * x

/* Cast as a pointer-wide integer */
#define U_PTR( x )	( ( ULONG_PTR ) x )

/* Cast as a pointer */
#define C_PTR( x )	( ( PVOID ) x )
