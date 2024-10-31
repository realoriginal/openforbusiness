#pragma once

/*!
 *
 * Purpose:
 *
 * Parses a portable executable to locate the specified export
 * if it exists.
 *
!*/
D_SEC( B )  PVOID PeGetFuncEat( IN PVOID ImageBase, IN PCHAR ExportName );
