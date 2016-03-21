#include "P2.h"

uint32_t weight( uint32_t x ) {
  x = ( x & 0x55555555 ) + ( ( x >>  1 ) & 0x55555555 );
  x = ( x & 0x33333333 ) + ( ( x >>  2 ) & 0x33333333 );
  x = ( x & 0x0F0F0F0F ) + ( ( x >>  4 ) & 0x0F0F0F0F );
  x = ( x & 0x00FF00FF ) + ( ( x >>  8 ) & 0x00FF00FF );
  x = ( x & 0x0000FFFF ) + ( ( x >> 16 ) & 0x0000FFFF );

  return x;
}

void P2() {

  while (1) {
    int n = 10;
    while (n > 0) {
      n--;
      printS("P2 -- ");
    }
    printS("\n");
    yield();
  }

  /*  for ( uint32_t d = 10; d > 0 ; d -= 1 ) {
      printS(b);
    }*/

  /*  while ( 1 ) {
      // compute the Hamming weight of each x for 2^8 < x < 2^24
      write(0, "Process 2 \n", 10 );
                for ( uint32_t x = ( 1 << 8 ); x < ( 1 << 24 ); x++ ) {
                  uint32_t r = weight( x );  // printf( "weight( %d ) = %d\n", x, r );
                  //write( 0, b, 20 );
                  write(0, "weight( " , 8 );
                  printInt(x);
                  write(0, " ) = ", 5 );
                  printInt(r);
                  write(0, "\n", 2);
                }
    }*/

  return;
}
void (*entry_P2)() = &P2;