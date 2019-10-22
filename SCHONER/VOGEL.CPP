#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

int utka_zwei( unsigned int *z )
{
   FILE *fp;
   unsigned int *z1;

   z1 = z;
   if ( !(fp = fopen( "utka2.bld", "r" ) ) )
   {
      puts( "Utka2.bld kann nicht ge”ffet werden !" );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z1++ = fgetc( fp );
   fclose( fp );
   return( 0 );
}

int utka_drei( unsigned int *z )
{
   FILE *fp;
   unsigned int *z1;

   z1 = z;
   if ( !(fp = fopen( "utka3.bld", "r" ) ) )
   {
      puts( "Utka3.bld kann nicht ge”ffet werden !" );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z1++ = fgetc( fp );
   fclose( fp );
   return( 0 );
}

void bild_ausgeben( int x_oben, int y_oben, unsigned int  *zeiger )
{
   int x1, x2, xh, y1, y2;
   unsigned int *z;

   z = zeiger;
   x1 = x_oben;
   y1 = y_oben;
   x2 = x1 + *z++;
   y2 = y1 + *z++;
   for (; y1 != y2; y1++ ) for ( xh = x1; xh != x2; xh++ )
      putpixel( xh, y1, *z++ );
}

void main ( void )
{
   int gr = 9, // VGA
       md = 1, // 640 * 350
       hinten = 9, // Hintergrundfarbe
       schritt = 3;
   unsigned int u2[1668], u3[3184];
   int x, y; // Anfangskoordinaten
   int n; // Hilfsvariable
   int seite; // Bildschirmseite

   clrscr();
   if ( (utka_zwei( u2 ) != 0) || (utka_drei( u3 ) != 0) ) exit( 0 );
   seite ^= seite;

   initgraph( &gr, &md, "" );
   cleardevice();
   setbkcolor( hinten );
   setcolor( 1 );

   while ( kbhit() == 0 )
   {
      y = random( 300 ) + 1;
      x = 1;
      while ( x < getmaxx() )
      {
	 for ( n ^= n; n < 20; n++ )
	 {
	    seite = 1 - seite;
	    setactivepage( seite );
	    setvisualpage( 1 - seite );
	    clearviewport();
	    bild_ausgeben( x += schritt, y + 10, u2 );
	 }
	 for ( n ^= n; n < 10; n++ )
	 {
	    seite = 1 - seite;
	    setactivepage( seite );
	    setvisualpage( 1 - seite );
	    clearviewport();
	    bild_ausgeben( x += schritt, y, u3 );
	 }
      }
   }
   closegraph();
}



