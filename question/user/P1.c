#include "P1.h"

uint32_t gcd( uint32_t x, uint32_t y ) {
  if     ( x == y ) {
    return x;
  }
  else if ( x >  y ) {
    return gcd( x - y, y );
  }
  else if ( x <  y ) {
    return gcd( x, y - x );
  }
}

void P1() {
  int n = 0;
  while ( 1 ) {
    printS( "Process 1: \n" );
    // compute the gcd between pairs of x and y for 2^8 < x, y < 2^24
    // write(0, "Process 1 \n", 10 );
    for ( uint32_t x = ( 1 << 8 ); x < ( 1 << 24 ); x++ ) {
      for ( uint32_t y = ( 1 << 8 ); y < ( 1 << 24 ); y++ ) {
        uint32_t r = gcd( x, y );  // printf( "gcd( %d, %d ) = %d\n", x, y, r );
        //write( 0, a, 20 );
        printS("gcd( " );
        printInt(x);
        printS(", " );
        printInt(y);
        printS(" ) = ");
        printInt(r);
        printS("\n");

      }
    }
  }

  return;
}

void (*entry_P1)() = &P1;