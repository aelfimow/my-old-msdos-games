#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>


unsigned char b[1080] = { 0 };

void schrift_einlesen( void );
void zeichen_ausgeben( int xoben, int yoben, int nummer );

void main( void )
{
   int gr = 9,
       md = 1;
   int xa = 10, ya = 20;
   int n;

   initgraph( &gr, &md, "" );
   cleardevice();
   schrift_einlesen();
   delay( 4000 );
   for ( n ^= n; n < 107; n++ )
   {
      zeichen_ausgeben( xa += 20, ya, n );
      xa > 600 ? ya += 20, xa = 10 : xa;
   }
   while ( kbhit() == 0 );
   getch();
   closegraph();
}

void schrift_einlesen( void )
{
   FILE *fp;
   unsigned char *zeiger;
   int n;

   zeiger = b;
   if ( !(fp = fopen( "russ01.buc", "r" )) ) return;
   while ( feof( fp ) == 0 ) *zeiger++ = ( unsigned char ) getc( fp );
   fclose( fp );
}
void zeichen_ausgeben( int xoben, int yoben, int nummer )
{
   unsigned char *z;
   int ya, ye, xa, xe;
   unsigned char sw;

   xe = xoben + 7 ;
   ye = yoben + 10 ;
   z = b;
   z += ( nummer * 10 );
   for ( ya = yoben; ya < ye; ya++ )
   {
      sw = 128;
      for ( xa = xoben; xa < xe; xa++ )
      {
	 if ( (*z & sw) == sw ) putpixel( xa, ya, 15 );
	 sw >>= 1;
      }
      z++;
   }
}


