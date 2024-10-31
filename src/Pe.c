#include "Common.h"

/*!
 *
 * Purpose:
 *
 * Parses a portable executable to locate the specified export
 * if it exists.
 *
!*/
D_SEC( B )  PVOID PeGetFuncEat( IN PVOID ImageBase, IN PCHAR ExportName )
{
	PCHAR			ExportSz = NULL;
	PUINT16			ExpAoOrd = NULL;
	PUINT32			ExpAoStr = NULL;
	PUINT32			ExpAoFun = NULL;
	PIMAGE_DOS_HEADER	PeHeader = NULL;
	PIMAGE_NT_HEADERS	NtHeader = NULL;
	PIMAGE_DATA_DIRECTORY	IdHeader = NULL;
	PIMAGE_EXPORT_DIRECTORY	IeHeader = NULL;

	/* Get the pointer to the the export directory OPTIONAL header entry */
	PeHeader = C_PTR( ImageBase );
	NtHeader = C_PTR( U_PTR( ImageBase ) + PeHeader->e_lfanew );
	IdHeader = & NtHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ];

	/* Does the export directory exist? */
	if ( IdHeader->VirtualAddress != 0 ) {
		IeHeader = C_PTR( U_PTR( ImageBase ) + IdHeader->VirtualAddress );
		ExpAoStr = C_PTR( U_PTR( ImageBase ) + IeHeader->AddressOfNames );
		ExpAoFun = C_PTR( U_PTR( ImageBase ) + IeHeader->AddressOfFunctions );
		ExpAoOrd = C_PTR( U_PTR( ImageBase ) + IeHeader->AddressOfNameOrdinals );

		/* Loop through each name */
		for ( INT Idx = 0 ; Idx < IeHeader->NumberOfNames ; ++Idx ) {
			/* Extract the export name */
			ExportSz = C_PTR( U_PTR( ImageBase ) + ExpAoStr[ Idx ] );

			/* Is this the export we were looking for? */
			if ( HashString( ExportSz, 0 ) == HashString( ExportName, 0 ) ) {
				/* Return the pointer to the export */
				return C_PTR( U_PTR( ImageBase ) + ExpAoFun[ ExpAoOrd[ Idx ] ] ); 
			};
		};
	};
	return NULL;
};
