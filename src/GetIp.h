#pragma once

/*!
 *
 * Purpose:
 *
 * Returns the pointer to itself. Additionally, acts as a 
 * as the end of code marker for the extraction script. It
 * is a bit of a doozy.
 *
!*/
D_SEC( C ) __attribute__(( naked )) ULONG_PTR GetIp( VOID );
