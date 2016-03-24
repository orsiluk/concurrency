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
  /*
    // Used for testing
      while (1) {
      int n = 10;

      while (n > 0) {
        n--;
        printS("P1 ~~ ");
      }
      printS("\n");
      fork();
      printS("I want to fork");
      system_exit();
      printS("I exited");
      yield();

    }*/
  int n = 0;
  while ( 1 ) {
    write(0, "Process 1 \n", 11 );
    // compute the gcd between pairs of x and y for 2^8 < x, y < 2^24
    //write(0, "Process 1 \n", 10 );
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

        if (n < 21) {
          n++;
        } else {
          // system_exit();
          yield();
        }
      }
    }
    //system_exit();
  }

  return;
}

void (*entry_P1)() = &P1;