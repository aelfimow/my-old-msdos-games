#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <time.h>

int utka01_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "utka01.bld", "r" )) )
   {
      puts( "Datei utka01.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Utka01.bld: %d Byte gelesen.\n", n );
   fclose( fp );
   return( 0 );
}
int utka02_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "utka02.bld", "r" )) )
   {
      puts( "Datei utka02.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Utka02.bld: %d Byte gelesen.\n", n );
   fclose( fp );
   return( 0 );
}
int utka03_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "utka03.bld", "r" )) )
   {
      puts( "Datei utka03.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Utka03.bld: %d Byte gelesen.\n", n );
   fclose( fp );
   return( 0 );
}
int tot1_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "tot1.bld", "r" )) )
   {
      puts( "Datei tot1.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Tot1.bld: %d Byte gelesen.\n", n );
   fclose( fp );
   return( 0 );
}
int tot2_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "tot2.bld", "r" )) )
   {
      puts( "Datei tot2.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Tot2.bld: %d Byte gelesen.\n", n );
   fclose( fp );
   return( 0 );
}
int tote1_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "tote1.bld", "r" )) )
   {
      puts( "Datei tote1.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Tote1.bld: %d Byte gelesen.\n", n );
   fclose( fp );
   return( 0 );
}
int tote2_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "tote2.bld", "r" )) )
   {
      puts( "Datei tote2.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Tote2.bld: %d Byte gelesen.\n", n );
   fclose( fp );
   return( 0 );
}
int tote3_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "tote3.bld", "r" )) )
   {
      puts( "Datei tote3.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Tote3.bld: %d Byte gelesen.\n", n );
   fclose( fp );
   return( 0 );
}
int tote01_lesen( unsigned int *zeiger )
{
   FILE *fp;
   unsigned int *z;
   register unsigned int n = 1;

   z = zeiger;
   if ( !(fp = fopen( "tote01.bld", "r" )) )
   {
      puts( "Datei tote01.bld kann nicht geîffnet werden." );
      return( 1 );
   }
   while ( feof( fp ) == 0 ) *z++ = getc( fp ), n++;
   printf( "Tote01.bld: %d Byte gelesen.\n", n );
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

void schiessen( int x, int y, int n, int *z1, int *z2, int *z3 )
{
   int *xkoo, *ykoo, *erwischt;
   int xh, yh, nn;

   xkoo = z1; ykoo = z2; erwischt = z3;
   xh = x; yh = y;
   for ( nn ^= nn; nn < n; nn++ )
   {
      if ( *erwischt == 0 )
      if ( (xh > (*xkoo + 10)) && (xh < (*xkoo + 60)) &&
	   (yh > (*ykoo + 10)) && (yh < (*ykoo + 24)) )
      {
	 sound( 1000 );
	 delay( 100 );
	 nosound();
	 *erwischt = random( 3 ) + 1;
	 return;
      }
      erwischt++; xkoo++; ykoo++;
   }
}
void ausgeben( void )
{
   unsigned char b[17], c[17] = { 0, 0, 0, 0, 50, 15, 0, 100, 120, 255, 0, 20, 36, 78, 69, 236, 236 };
   int n;
   b[0] = 88,  b[0] ^= 25;    // A
   b[1] = b[0] + 43;          // l
   b[2] = b[1] ^ 9;	      // e
   b[3] = 0xFF, b[3] ^= 135;  // x
   b[4] = 83 ^ c[4];          // a
   b[5] = c[5] ^ b[4];        // n
   b[6] = b[5] - 10;          // d
   b[7] = b[2];               // e
   b[8] = c[7] ^ 22;          // r
   b[9] = ' ';
   b[10] = b[2] - 32;         // E
   b[11] = b[1];              // l
   b[12] = 200, b[12] ^= 174; // f
   b[13] = c[7] + 5;          // i
   b[14] = b[13] ^ 4;         // m
   b[15] = 131 ^ c[16];       // o
   b[16] = b[8] ^ 5;          // w
   for ( n = 0; n <= 16; n++ ) putchar( b[n] );
}

void main( void )
{
   // Hintergrundfarbe
   const int hinten = 9;
   // Max. Zahl der Enten
   const int anzahl = 5;
   // Daten der Enten
   unsigned int u1[2236], u2[2236], u3[2356], t1[4543], t2[4543],
		te1[1585], te2[1279], te3[1066], te01[3336];
   // Zeiger auf die Daten
   unsigned int *z[6] = { u1, u2, u3, u3, u2, u1 },
		*z2[2] = { t1, t2 },
		*z3[3] = { te1, te2, te3 };
   // Bildschirmseite
   int seite = 0,
       dr = 9, // --> VGA
       md = 1; // --> 640 * 350, 2 Seiten
   // Koordinaten des Visiers
   int x_visier = 320, y_visier = 170;
   // Anfangskoordinaten
   int xa, ya;
   // Koordinaten der Enten
   int xe[anzahl] = { 0 }, ye[anzahl] = { 100 }, y[anzahl] = { 0 };
   // Geschwindigkeit der Enten
   int schritt = 3;
   // Geschindigkeit des Visiers
   int vv = 5;
   // Aktuelle Zahl der Enten
   int aktuelle_zahl = 1;
   // Angeschossen ja - nein;
   int angeschossen[anzahl] = { 0 };
   // Zeiten
   time_t zeit1;
   // Hilfsvariablen
   unsigned int n = 0, m = 0, p[anzahl] = { 0 };
   int taste = 0, t = 0;

   clrscr();
   // Lesen der Daten und öberprÅfung
   if ( (utka01_lesen( u1 ) != 0) ||
	(utka02_lesen( u2 ) != 0) ||
	(utka03_lesen( u3 ) != 0) ||
	(tot1_lesen( t1 ) != 0) ||
	(tot2_lesen( t2 ) != 0) ||
	(tote1_lesen( te1 ) != 0) ||
	(tote2_lesen( te2 ) != 0) ||
	(tote3_lesen( te3 ) != 0) ||
	(tote01_lesen( te01 ) != 0) ) exit( 0 );
   else puts( "Alle Dateien erfolgreich gelesen." );

   // Grafik
   initgraph( &dr, &md, "" ); cleardevice();

   // Warnhinweis
   settextstyle( 2, 0, 2 ); setcolor( 14 );
   outtextxy( 80, 40, "Dieses Spiel ist fÅr Spieler unter 18 nicht geeignet..." );
   delay( 5000 );

   // Kleine Vorbereitung
   setbkcolor( hinten ); clearviewport();
   // Hauptteil
   zeit1 = time(NULL);
   while ( t == 0 )
   {
      m == 5 ? m ^= m : m++;
      // Seiten umtauschen
      seite = 1 - seite;
      setactivepage( seite );
      setvisualpage( 1 - seite );
      clearviewport();
      // Testen, ob einen neuen Vogel ausgeben
      if ( aktuelle_zahl < anzahl ) if ( difftime( time(NULL), zeit1 ) > 3.0 )
      {
	 zeit1 = time(NULL);
	 aktuelle_zahl++;
      }
      for ( n ^= n; n < aktuelle_zahl; n++ )
      {
	    xe[n] > 639 ? xe[n] ^= xe[n], ye[n] = random( 300 ) : xe[n];
	    // Testen, ob Taste gedrÅckt
	    if ( kbhit() != 0 ) taste = getch();
	    switch ( taste )
	    {
	       case 13: taste ^= taste; schiessen( x_visier, y_visier, anzahl, xe, ye, angeschossen); break;
	       case 27: t++; break; // --> Abbruchbedingung erfÅllt
	       case 72: if ( (y_visier - vv) > 1 ) y_visier -= random(vv); break;
	       case 75: if ( (x_visier - vv) > 1 ) x_visier -= random(vv); break;
	       case 77: if ( (x_visier + vv) < 638 ) x_visier += random(vv); break;
	       case 80: if ( (y_visier + vv) < 348 ) y_visier += random(vv); break;
	       default: break;
	    }
	    // öberprÅfung, ob angeschossen
	    if ( angeschossen[n] == 0 ) bild_ausgeben( xe[n] += schritt, ye[n], z[m] );
	    else
	    {
	       switch ( angeschossen[n] )
	       {
		  case 1: bild_ausgeben( xe[n] += schritt, ye[n] += schritt, z3[p[n]] );
			  p[n] == 2 ? p[n] ^= p[n] : p[n]++;
			  ye[n] > 349 ? xe[n] = 650, angeschossen[n] = 0, p[n] ^= p[n] : ye[n];
			  break;

		  case 2: bild_ausgeben( xe[n], ye[n] += schritt + 5, z2[p[n]] );
			  y[n] > 10 ? p[n] == 1 ? p[n] : p[n]++ : y[n]++;
			  ye[n] > 349 ? xe[n] = 650, angeschossen[n] = 0, y[n] ^= y[n], p[n] = 0 : ye[n];
			  break;

		  case 3: bild_ausgeben( xe[n], ye[n] += schritt + 5, te01 );
			  ye[n] > 349 ? xe[n] = 650, angeschossen[n] = 0 : ye[n];
			  break;

		  default: break;
	       }
	    }
      }
      // Visier ausgeben
      setcolor( 1 );
      line( x_visier, 0, x_visier, 349 );
      line( 0, y_visier, 639, y_visier );
   }
   closegraph();
   puts( "Utka, Version 1.0," );
   printf( "Copyright by " ); ausgeben();
   printf( "\nAll rights reserved.\n\n" );
   puts( "Beliebige Taste drÅcken..." );
   taste = getch();
   puts( "... bitte 10 Sekunden warten." );
   delay( 10000 );
}


