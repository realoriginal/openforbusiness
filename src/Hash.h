#pragma once

/*!
 *
 * Purpose:
 *
 * Converts a ANSI or Unicode String into a DJB2 hash. ANSI strings
 * are assumed to be NULL terminated, and a Unicode string must have
 * its length passed in
 *
!*/
D_SEC( B )  UINT32 HashString( IN PUINT8 Buffer, IN SIZE_T Length );
