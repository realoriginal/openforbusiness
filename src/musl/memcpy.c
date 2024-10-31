#include <string.h>
#include <stdint.h>

__attribute__(( section( ".text$B" ), __annotate__( "nofnrebase" ) )) void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;

	for (; n; n--) *d++ = *s++;
	return dest;
}
