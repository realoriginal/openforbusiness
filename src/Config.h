#pragma once

/* Configuration struct appended at the end the shellcode */
typedef struct __attribute__(( packed ))
{
	/* e_lfanew value */
	UINT16	LfaNew;

	/* AddressOfEntrypoint value */
	UINT32	AofEnt;
} SHELLCODE_CONFIG, *PSHELLCODE_CONFIG ;
