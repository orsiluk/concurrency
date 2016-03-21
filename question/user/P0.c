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
  char x[11];

  /*  for ( uint32_t d = 10; d > 0 ; d -= 1 ) {
      write(0, "Process 0 \n", 11 );
    }*/
  while ( 1 ) {
    int n = 5;
    //memset(res, 0, 20);
    //write(0, "Process 0 \n", 10 );
    // test whether each x for 2^8 < x < 2^24 is prime or not
    /*          write(0, "pancakes\n", 10 );

              for ( uint32_t x = ( 1 << 8 ); x < ( 1 << 24 ); x++ ) {
                int r = is_prime( x );
                // printf( "is_prime( %d ) = %d\n", x, r );

                //strncpy(res, "is_prime(" , 9);
                //strncat(res, itoa(x, buf, 10), strlen(x));
                //strncat(res, xc, strlen(xc));
                // strncat(res, ") = ", 4);
                //strncat(res,  r , strlen(r));
                //strncat(res, ")\n", 3);
                write(0, "is_prime( " , 9 );
                printInt(x);
                write(0, " ) = ", 4 );
                printInt(r);
                write(0, "\n", 2);


              }*/
    /*    fork();
        write(0, " pancakes :) \n", 11 );
    */
    while (n > 0) {
      printS("Read text: ");
      read( 0, x, 11 );
      printS(x);
      printS("\n");
      n--;
    }
    yield();
    //system_exit();


  }

  return;
}
void (*entry_P0)() = &P0;