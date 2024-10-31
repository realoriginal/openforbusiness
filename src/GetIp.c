#include "Common.h"

/*!
 *
 * Purpose:
 *
 * Returns the pointer to itself. Additionally, acts as a 
 * as the end of code marker for the extraction script. It
 * is a bit of a doozy.
 *
!*/
D_SEC( C ) __attribute__(( naked )) ULONG_PTR GetIp( VOID )
{
	__asm__ __volatile__(
		#if defined( _WIN64 )
		/* Execute the next instruction */
		"call	_next_instruction\n"

		"_next_instruction:\n"
		/* Pop the return address off the stack */
		"popq	%rax\n"

		/* Subtract the difference between the two labels */
		"subq	$(_next_instruction - GetIp), %rax\n"

		/* Return the pointer */
		"ret"
		#else
		/* Execute the next instruction */
		"call	__next_instruction\n"

		"__next_instruction:\n"
		/* Pop the return address off the stack */
		"popl	%eax\n"

		/* Subtract the difference between the two labels */
		"subl	$(__next_instruction - _GetIp), %eax\n"

		/* Return the pointer */
		"ret"
		#endif
	);

	/* End of code marker for the extractsc.py script */
	__asm__ __volatile__( ".string \"ENDOFCODE\"" );
}
