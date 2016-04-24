#include "P0.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int is_prime( uint32_t x ) {
  if ( !( x & 1 ) || ( x < 2 ) ) {
    return ( x == 2 );
  }

  for ( uint32_t d = 3; ( d * d ) <= x ; d += 2 ) {
    if (! ( x % d) ) {
      return 0;
    }
  }
  return 1;
}

void P0() {
  char x[50];
  while ( 1 ) {
    int n = 0;
    printS( "Process 0: \n" );
    for ( uint32_t x = ( 1 << 8 ); x < ( 1 << 24 ); x++ ) {
      int r = is_prime( x );
      printS("is_prime( ");
      printInt(x);
      printS(" ) = ");
      printInt(r);
      printS("\n");
    }
  }

  return;
}
void (*entry_P0)() = &P0;