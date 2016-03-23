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

  /*  for ( uint32_t d = 10; d > 0 ; d -= 1 ) {
      write(0, "Process 0 \n", 11 );
    }*/
  while ( 1 ) {
    int n = 0;
    //memset(res, 0, 20);
    //write(0, "Process 0 \n", 10 );
    // test whether each x for 2^8 < x < 2^24 is prime or not
    write(0, "pancakes\n", 10 );

    for ( uint32_t x = ( 1 << 8 ); x < ( 1 << 24 ); x++ ) {
      int r = is_prime( x );
      printS("is_prime( ");
      printInt(x);
      printS(" ) = ");
      printInt(r);
      printS("\n");

      if (n < 100) {
        n++;
      } else {
        yield();
      }
    }
    /*
      // Used for testing
        while (n > 0) {
                printS("Read text: ");
                read( x );
                printS(x);
                printS("\n");
          n--;
          printS(" P0 :) \n");
        }

        yield();
        //system_exit();
    */

  }

  return;
}
void (*entry_P0)() = &P0;