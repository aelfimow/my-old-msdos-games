#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>

const int rahmen_farbe = 2;  // grn
const int kasten_farbe = 10; // leicht grn
const int text_farbe = 15; // weiss
const int xa = 240, xe = 400; // x-Anfang, x-Ende
const int ya = 50, ye = 250;  // y-Anfang, y-Ende

void kasten_zeichnen( void );
void bemalen( int x1, int y1, int x2, int y2, int farbe );
void zeichen_ausgeben( int xkoo, int ykoo, unsigned char *zeiger );
void leeren( int x1, int y1, int laenge, int breite, int farbe );

void main( void )
{
   int gr = 9, // VGA
       md = 1; // 640 * 350, 2 Seiten

   FILE *fp;
   int beenden = 0;
   int xm, ym, mt = 0;
   int xh, yh;
   int y = 10, x = 10;
   unsigned char sw;
   unsigned char z[10] = { 0 };
   unsigned char *zeiger;
   char name[80] = { 0 },
	satz[10];
   int taste = 0;
   int xt = 60;
   char zeichen[2] = { 0 };
   unsigned char n = 0;
   unsigned int anzahl = 0;

   // Grafik einschalten
   initgraph( &gr, &md, "" );
   cleardevice();
   // Mausreset
   asm { xor ax, ax; INT 0x33 }
   settextstyle( SMALL_FONT, HORIZ_DIR, 4 );
   // Rahmen wird gezeichnet
   setcolor( rahmen_farbe );
   lineto( 639, 0 ), lineto( 639, 349 ), lineto( 0, 349 ), lineto( 0, 0 );
   line( 0, 334, 639, 334 );
   // Žhnliches auf der anderen Seite
   setactivepage( 1 );
   lineto( 639, 0 ), lineto( 639, 349 ), lineto( 0, 349 ), lineto( 0, 0 );
   setactivepage( 0 );
   // Kasten wird gezeichnet
   kasten_zeichnen();
   // Datei wird erstellt
   setcolor( text_farbe );
   outtextxy( 10, 335, "Datei -->" );
   // Dateinamen eingeben
   while ( (taste = getch()) != 13 )
   if ( (taste > 32) && (taste < 123) )
   {
      zeichen[0] = ( char )taste;
      outtextxy( xt += 6, 335, zeichen );
      name[n] = ( char ) taste;
      n++;
   }
   outtextxy( 300, 335, "ENTER drcken, um das Zeichen zu speichern" );
   if ( !(fp = fopen( name, "a" )) ) beenden++;
   // Maus einschalten
   asm { mov ax, 1; INT 0x33 }
   // Hauptteil
   while ( beenden == 0 )
   {
      // Tastaturberwachung
      if ( kbhit() != 0 )
      if ( (taste = getch()) == 27 ) beenden++;
      else
      if ( taste == 13 ) for ( n ^= n; n < 10; n++ )
      {
	 anzahl++;
	 putc( ( int )z[n], fp );
	 itoa( anzahl, satz, 10 );
         setcolor( text_farbe );
	 leeren( 10, 10, 300, 15, 0 );
	 outtextxy( 15, 10, "Bytes erfolgreich geschrieben:" );
	 outtextxy( 220, 10, satz );
      }
      // Mausberwachung
      asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx }
      switch ( mt )
      {
	 case 1: // Felder setzen oder l”schen
		if ( (xm >= xa) && (xm <= xe) && (ym >= ya) && (ym <= ye) )
		{
		   zeiger = z;
		   for ( yh = ya; yh < ye + 1; yh += 20 )
		   {
		      sw = 128;
		      for ( xh = xa; xh < xe + 1; xh += 20 )
		      {
			 if ( (xm > xh) && (xm < xh + 20) && (ym > yh) && (ym < yh + 20) )
			 {
			    // L”schen oder setzen
			    if ( (*zeiger & sw) == sw )
			    {
			       bemalen( xh, yh, xh + 20, yh + 20, 0);
			       *zeiger ^= sw;
			    }
			    else
			    {
			       bemalen( xh, yh, xh + 20, yh + 20, kasten_farbe);
			       *zeiger |= sw;
			    }
			 }
			 sw >>= 1;
		      }
		      zeiger++;
		   }
		} delay( 100 ); break; // delay ist dazu da, um negative
				       // Effekte zu vermeiden

	 case 2: // Die zweite Seite wird angezeigt:
		 // Maus ausschalten
		 asm { mov ax, 2; INT 0x33 }
		 setactivepage( 1 );
		 setvisualpage( 1 );
		 // Maus einschalten
		 asm { mov ax, 1; INT 0x33 }
		 x > 600 ? x = 10, y += 15: x;
		 zeichen_ausgeben( x += 8, y, z );
		 while ( mt != 4 ) asm { mov ax, 3; INT 0x33; mov mt, bx }
		 // Maus ausschalten
		 asm { mov ax, 2; INT 0x33 }
		 setactivepage( 0 );
		 setvisualpage( 0 );
		 // Maus einschalten
		 asm { mov ax, 1; INT 0x33 }
		 break;
	 default: break;
      }

   }
   // Maus ausschalten
   asm { mov ax, 2; INT 0x33 }
   closegraph();
   fclose( fp );
}

void kasten_zeichnen( void )
{
   int xh, yh; // Hilfvariablen

   setcolor( kasten_farbe );
   // Die Stelle wird gereinigt
   setviewport( xa, ya, xe, ye, 0 ), clearviewport(), setviewport( 0, 0, 639, 349, 0 );
   // Kasten wird erstellt
   for ( yh = ya; yh < ye + 1; yh += 20 ) for ( xh = xa; xh < xe + 1; xh += 20 )
   {
      line( xa, yh, xe, yh );
      line( xh, ya, xh, ye );
   }
}
void bemalen( int x1, int y1, int x2, int y2, int farbe )
{
   // Maus ausschalten
   asm { mov ax, 2; INT 0x33 }

   y2 -= 1;
   y1 += 2;
   x1 += 2;
   x2 -= 2;
   setcolor( farbe );
   for (; y1 < y2; y1++ ) line( x1, y1, x2, y1 );

   // Maus einschalten
   asm { mov ax, 1; INT 0x33 }
}
void zeichen_ausgeben( int xkoo, int ykoo, unsigned char *zeiger )
{
   int xh, yh;
   unsigned char sw;

   for ( yh = ykoo; yh < ykoo + 10; yh++ )
   {
      sw = 128;
      for ( xh = xkoo; xh < xkoo + 8; xh++ )
      {
	 if ( (*zeiger & sw) == sw ) putpixel( xh, yh, 15 );
	 sw >>= 1;
      }
      zeiger++;
   }
}
void leeren( int x1, int y1, int laenge, int breite, int farbe )
{
   int alte_farbe;
   int x2, y2, xh;

   x2 = x1 + laenge;
   y2 = y1 + breite;
   alte_farbe = getcolor();
   setcolor( farbe );
   for (; y1 < y2; y1++ ) line( x1, y1, x2, y1 );
   setcolor( alte_farbe );
}