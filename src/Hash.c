#include "Common.h"

/*!
 *
 * Purpose:
 *
 * Converts a ANSI or Unicode String into a DJB2 hash. ANSI strings
 * are assumed to be NULL terminated, and a Unicode string must have
 * its length passed in
 *
!*/
D_SEC( B ) UINT32 HashString( IN PUINT8 Buffer, IN SIZE_T Length )
{
	UINT8	Char = 0;
	UINT32	Hash = 5381;
	PUINT8	Buff = C_PTR( Buffer );

	/* Loop through until we reach the NULL terminator or end of the length */
	while ( TRUE ) {
		/* Extract the current ANSI */
		Char = *Buff;

		/* No length was specified? */
		if ( ! Length ) {
			/* Have we reached the NULL terminator ? */
			if ( ! Char ) {
				/* Abort the loop */
				break;
			};
		} else
		{
			/* Have we exceeded the boundaries of the string? */
			if ( ( SIZE_T )( Buff - ( PUINT8 ) Buffer ) >= Length ) {
				/* Abort the loop */
				break;
			};

			/* Is this a NULL character */
			if ( ! Char ) { 
				/* Move onto the next character since we skip it */
				++Buff; continue;
			};
		};

		/* Is this a lowercase character? */
		if ( Char >= 'a' )  {
			/* Force to uppercase */
			Char -= 0x20;
		};

		/* Hash the current character */
		Hash = ( ( Hash << 5 ) + Hash ) + Char; ++Buff;
	};

	/* Return the hash */
	return Hash;
}; 
