#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

int grafik( void );
void leeren( int x1, int y1, int laenge, int breite, int farbe );

const int rahmen_farbe = 15;
const int text_farbe = 15;
const int markiert = 4;
const int status_farbe = 14;
const int erste = 0, zweite = 1; // Seitenbezeichnungen
const int xt = 10; // Textkoo
const int x[10] = { 0, 70, 140, 210, 280, 350, 420, 490, 560, 639 };
      int yt = 30; // Textkoo
      int globale_farbe = 15;

void aktuelle_farbe( void );
void nachricht( int zahl1, int abstand );
void worte_ausgeben( register int zahl );
int in_grenzen( int xkoo, int ykoo, int n );
void verschiebung( void );
int eingestellte_farbe( void );
void punkte_setzen( void );
void koo_ausgeben( int xkoo, int ykoo );
void spray( void );
void zeichnung_zeigen( void );
void kreis( void );
void linie_zeichnen( void );
void ellipse_zeichnen( void );
void laden( void );
void speichern( void );

void main( void )
{
   int n, vorher = 0; // Hilfsvariablen
   int abbruch = 0;
   int xm, ym, mt; // Mauskoo und Maustaste

   if ( grafik() != 0 ) puts( "Fehler beim Einschalten der Grafik !" ), exit( 0 );
// Reset der Maus
   asm { xor ax, ax; INT 0x33 }
   setcolor( rahmen_farbe );
   settextstyle( SMALL_FONT, HORIZ_DIR, 4 );
// Rahmen wird gezeichnet
   lineto( 639, 0 ), lineto( 639, 349 ), lineto( 0, 349 ), lineto( 0, 0 );
   line( 0, 15, 639, 15 ), line( 0, 334, 639, 334 );
// éhnliches auf der Zeichenseite
   setactivepage( zweite );
   line( 0, 349, 639, 349 ), line( 0, 334, 639, 334 );
   setactivepage( erste );
// Maus wird eingeschaltet und auf eine feste Position gesetzt
   asm { mov ax, 0x0001; INT 0x33; mov ax, 0x0004; mov cx, 100; mov dx, 100; INT 0x33 };
   // Mausgenzen festlegen
   asm { mov ax, 0x0008; xor cx, cx; mov dx, 333; INT 0x33 }
// MenÅ erstellen
   for ( n ^= n; n < 9; n++ ) line( x[n], 0, x[n], 15 ), worte_ausgeben( n );
   while( !abbruch )
   {
      // PrÅfung auf ESC
      if ( kbhit() != 0 ) if ( getch() == 27 ) abbruch++;
      // Mauskoo & Tastendruck ermitteln
      asm { mov ax, 0x0003; INT 0x33; mov xm, cx; mov ym, dx; mov mt, bx };
      // Markierungsvorgang
      n > 8 ? n ^= n : n++;
      if ( in_grenzen( xm, ym, n ) ) if ( vorher != n )
      {
	 // Maus aus
	 asm { mov ax, 0x0002; INT 0x33 };
	 // Markieren
	 leeren( x[n] + 1, 1, 68, 14, markiert );
	 leeren( x[vorher] + 1, 1, 68, 14, 0 );
	 setcolor( text_farbe );
	 worte_ausgeben( n );
	 worte_ausgeben( vorher );
	 // Maus ein
	 asm { mov ax, 0x0001; INT 0x33 };
	 vorher = n;
      }
      // MaustastendrÅcke feststellen und ensprechend reagieren
      switch ( mt )
      {
	 case 0: break; // Keine Taste gedrÅckt, dann nichts
	 case 1: for ( n ^= n; n < 9; n++ ) if ( in_grenzen( xm, ym, n ) )
		 {
		    // PrÅfung auf Textkoordinaten
		    yt > 280 ? verschiebung(), yt = 280 : yt;
		    setcolor( status_farbe );
		    switch ( n )
		    {
		       case 0: outtextxy( xt, yt += 10, "--> Bild speichern ..." );
			       speichern();
			       break;
		       case 1: outtextxy( xt, yt += 10, "--> Bild laden ..." );
			       laden();
			       break;
		       case 2: outtextxy( xt, yt += 10, "--> Linie zeichnen ..." );
			       linie_zeichnen();
			       break;
		       case 3: outtextxy( xt, yt += 10, "--> Kreis zeichnen ..." );
			       kreis();
			       break;
		       case 4: outtextxy( xt, yt += 10, "--> Ellipse zeichnen ..." );
			       ellipse_zeichnen();
			       break;
		       case 5: outtextxy( xt, yt += 10, "--> Punkte setzen ..." );
			       punkte_setzen();
			       break;
		       case 6: outtextxy( xt, yt += 10, "--> Spray aktivieren ..." );
			       spray();
			       break;
		       case 7: globale_farbe = eingestellte_farbe();
			       aktuelle_farbe();
			       break;
		       case 8: abbruch++; break;

		       default: break;
		    }
		 } delay( 100 ); break;
	 case 2: zeichnung_zeigen(); break;
	 default: break;
      }
   }
   closegraph();
}


int grafik( void )
{
   int gr = 9,
       md = 1;
   int fehler;

   initgraph( &gr, &md, "" );
   fehler = graphresult();
   cleardevice();
   return( fehler );
}
void worte_ausgeben( register int zahl )
{
   if ( zahl >= 9 ) return;
   switch ( zahl )
   {
      case 0: outtextxy( x[zahl] + 10, 1, "Speichern" ); break;
      case 1: outtextxy( x[zahl] + 10, 1, "  Laden" ); break;
      case 2: outtextxy( x[zahl] + 10, 1, "  Linie" ); break;
      case 3: outtextxy( x[zahl] + 10, 1, "  Kreis" ); break;
      case 4: outtextxy( x[zahl] + 10, 1, " Ellipse" ); break;
      case 5: outtextxy( x[zahl] + 10, 1, "  Punkt" ); break;
      case 6: outtextxy( x[zahl] + 10, 1, "  Spray" ); break;
      case 7: outtextxy( x[zahl] + 10, 1, "  Farbe" ); break;
      case 8: outtextxy( x[zahl] + 10, 1, " Beenden" ); break;
   }
}
int in_grenzen( int xkoo, int ykoo, int n )
{
   if ( (xkoo > x[n]) && (xkoo < x[n + 1]) && (ykoo < 15) ) return( 1 );
   else return( 0 );
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
void verschiebung( void )
{
   unsigned int groesse;
   void *zeiger;
   int x1 = xt,
       y1 = 30,
       x2 = 630,
       y2 = 330;

   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   groesse = imagesize( x1, 30, x2, 40 );
   zeiger = malloc( groesse );
   for ( y1 = 40; y1 < y2; y1 += 10 )
   {
      getimage( x1, y1, x2, y1 + 10, zeiger );
      putimage( x1, y1 - 10, zeiger, COPY_PUT );
   }
   free( zeiger );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
}
int eingestellte_farbe( void )
{
   char satz[10],
	zeichen[2] = { 0 };
   int taste = 0;
   int zahl = 0;
   int xa = 240;

   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   outtextxy( xt, yt, "--> Aktuelle Farbe =" );
   itoa( globale_farbe, satz, 10 );
   outtextxy( xt + 120, yt, satz );
   setcolor( 12 ); // Rote Farbe
   outtextxy( xt + 160, yt, "Neue Farbe =" );
   while ( (taste = getch()) != 13 )
   {
      if ( (taste > 47) && (taste < 58) )
      {
	 zeichen[0] = taste;
	 outtextxy( xa += 5, yt, zeichen );
	 zahl *= 10;
	 zahl += (taste - 48);
      }
   }
   setcolor( 2 ); // GrÅne Farbe
   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   outtextxy( xt, yt, "--> Neue Farbe festgelegt." );
   zahl > 15 ? zahl = 15 : zahl;
   return( zahl );
}
void punkte_setzen( void )
{
   int maustaste = 0;
   int xm, ym;
   int farbe = globale_farbe;

   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 };
   setactivepage( zweite );
   setvisualpage( zweite );
   aktuelle_farbe();
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 };
   delay( 500 );
   while ( maustaste != 2 )
   {
      // TastendrÅcke feststellen
      asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx };
      koo_ausgeben( xm, ym );
      if ( maustaste == 1 )
      {
	 // Maus aus
	 asm { mov ax, 0x0002; INT 0x33 }
	 // Setze Punkt
	 putpixel( xm, ym, farbe );
	 // Maus ein
	 asm { mov ax, 0x0001; INT 0x33 }
      }
      // Tastaturpuffer leeren
      if ( kbhit() != 0 ) getch();
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 };
   setactivepage( erste );
   setvisualpage( erste );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 };
   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   setcolor( 2 ); // GrÅne Farbe
   outtextxy( xt, yt, "--> ...beendet." );
}
void koo_ausgeben( int xkoo, int ykoo )
{
   char satz[10];
   static int x_vor, y_vor;

   setcolor( 2 );
   if ( x_vor != xkoo )
   {
      x_vor = xkoo;
      leeren( 400, 335, 100, 13, 0 );
      outtextxy( 410, 335, "X -->" );
      itoa( x_vor, satz, 10 );
      outtextxy( 440, 335, satz );
   }
   if ( y_vor != ykoo )
   {
      y_vor = ykoo;
      leeren( 500, 335, 100, 13, 0 );
      outtextxy( 510, 335, "Y -->" );
      itoa( y_vor, satz, 10 );
      outtextxy( 540, 335, satz );
   }
}
void spray( void )
{
   int xm, ym, maustaste;
   static int breite;
   int anzahl, n;
   int taste = 0;
   int zahl;
   int farbe = globale_farbe;
   char satz[10];
   char zeichen[2] = { 0 };
   int xa = 270;

   breite == 0 ? breite = 4 : breite;
   farbe = globale_farbe;
   maustaste ^= maustaste;
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 };
   setactivepage( zweite );
   setvisualpage( zweite );
   aktuelle_farbe();
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 };
   delay( 100 );
   while ( maustaste != 2 )
   {
      // TastendrÅcke feststellen
      asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx };
      koo_ausgeben( xm, ym );
      if ( maustaste == 1 )
      {
	 // Maus aus
	 asm { mov ax, 0x0002; INT 0x33 }
	 anzahl = breite >> 1;
	 for ( n ^= n; n < anzahl; n++ )
	 {
	    putpixel( xm - random(breite), ym - random(breite), farbe );
	    putpixel( xm + random(breite), ym - random(breite), farbe );
	    if ( (ym + breite) < 333 )
	    {
	       putpixel( xm - random(breite), ym + random(breite), farbe );
	       putpixel( xm + random(breite), ym + random(breite), farbe );
	    }
	 }
	 // Maus ein
	 asm { mov ax, 0x0001; INT 0x33 }
      }
      if ( kbhit() != 0 ) if ( getch() == 13 )
      {
	 zahl ^= zahl;
	 // Maus aus
	 asm { mov ax, 0x0002; INT 0x33 };
	 // Seiten wechseln
	 setactivepage( erste );
	 setvisualpage( erste );
	 setcolor( status_farbe );
	 yt > 280 ? verschiebung(), yt = 280 : yt += 10;
	 outtextxy( xt, yt, "--> Aktuelle Breite =" );
	 itoa( breite, satz, 10 );
	 outtextxy( xt + 130, yt, satz );
	 setcolor( 12 ); // Rote Farbe
	 outtextxy( xt + 180, yt, "Neue Breite =" );
	 while ( (taste = getch()) != 13 )
	 {
	    if ( (taste > 47) && (taste < 58) )
	    {
	       zeichen[0] = taste;
	       outtextxy( xa += 5, yt, zeichen );
	       zahl *= 10;
	       zahl += (taste - 48);
	    }
	 }
	 breite = zahl;
	 setcolor( 2 ); // GrÅne Farbe
	 yt > 280 ? verschiebung(), yt = 280 : yt += 10;
	 outtextxy( xt, yt, "--> Neue Breite festgelegt." );
	 setactivepage( zweite );
	 setvisualpage( zweite );
	 // Maus ein
	 asm { mov ax, 0x0001; INT 0x33 }
      }
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 };
   setactivepage( erste );
   setvisualpage( erste );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   setcolor( 2 ); // GrÅne Farbe
   outtextxy( xt, yt, "--> ...beendet." );
}
void zeichnung_zeigen( void )
{
   int maustaste = 0;
   int xm, ym;

   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setactivepage( zweite );
   setvisualpage( zweite );
   aktuelle_farbe();
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   delay( 100 );
   while ( maustaste != 2 )
   {
      asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
      koo_ausgeben( xm, ym );
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setactivepage( erste );
   setvisualpage( erste );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   delay( 100 );
}
void kreis( void )
{
   int maustaste = 0;
   int xm, ym;
   int x1, y1, radius;

   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setactivepage( zweite );
   setvisualpage( zweite );
   aktuelle_farbe();
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   delay( 100 );
   while ( maustaste != 2 )
   {
      while ( maustaste != 1 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
      x1 = xm, y1 = ym;
      putpixel( x1, y1, globale_farbe );
      maustaste ^= maustaste;
      delay( 100 );
      while ( maustaste != 1 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
      if ( xm > x1 ) radius = xm - x1; else radius = x1 - xm;
      // Maus aus
      asm { mov ax, 0x0002; INT 0x33 }
      setcolor( globale_farbe );
      circle( x1, y1, radius );
      putpixel( x1, y1, getbkcolor() );
      setactivepage( erste );
      yt > 280 ? verschiebung(), yt = 280 : yt += 10;
      setcolor( status_farbe );
      outtextxy( xt, yt, "Mittelpunkt (    ;    ) , Radius = " );
      nachricht( x1, 75 );
      nachricht( y1, 105 );
      nachricht( radius, 200 );
      setactivepage( zweite );
      // Maus ein
      asm { mov ax, 0x0001; INT 0x33 }
      maustaste ^= maustaste;
      delay( 100 );
      while ( maustaste == 0 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 };
   setactivepage( erste );
   setvisualpage( erste );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   setcolor( 2 ); // GrÅne Farbe
   outtextxy( xt, yt, "--> ...beendet." );
}
void nachricht( int zahl1, int abstand )
{
   char satz[10];

   setcolor( 2 );
   itoa( zahl1, satz, 10 );
   outtextxy( xt + abstand, yt, satz );
   setcolor( globale_farbe );
}
void linie_zeichnen( void )
{
   int maustaste = 0;
   int xm, ym;
   int x1, y1;

   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setactivepage( zweite );
   setvisualpage( zweite );
   aktuelle_farbe();
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   delay( 100 );
   while ( maustaste != 2 )
   {
      // Anfangskoo festlegen
      while ( maustaste != 1 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
      x1 = xm, y1 = ym;
      putpixel( x1, y1, globale_farbe );
      maustaste ^= maustaste;
      delay( 100 );
      // Endkoo festlegen
      while ( maustaste != 1 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
      setcolor( globale_farbe );
      // Maus aus
      asm { mov ax, 0x0002; INT 0x33 }
      line( x1, y1, xm, ym );
      setactivepage( erste );
      yt > 280 ? verschiebung(), yt = 280 : yt += 10;
      setcolor( status_farbe );
      outtextxy( xt, yt, "--> von (    ;    ) nach (    ;    )" );
      nachricht( x1, 55 );
      nachricht( y1, 85 );
      nachricht( xm, 155 );
      nachricht( ym, 185 );
      setactivepage( zweite );
      // Maus ein
      asm { mov ax, 0x0001; INT 0x33 }
      maustaste ^= maustaste;
      delay( 100 );
      while ( maustaste == 0 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 };
   setactivepage( erste );
   setvisualpage( erste );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   setcolor( 2 ); // GrÅne Farbe
   outtextxy( xt, yt, "--> ...beendet." );
}
void ellipse_zeichnen( void )
{
   int maustaste = 0;
   int xm, ym;
   int x1, y1;
   int xr, yr;

   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setactivepage( zweite );
   setvisualpage( zweite );
   aktuelle_farbe();
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   delay( 100 );
   while ( maustaste != 2 )
   {
      while ( maustaste != 1 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
      x1 = xm, y1 = ym;
      putpixel( x1, y1, globale_farbe );
      maustaste ^= maustaste;
      delay( 100 );
      while ( maustaste != 1 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
      if ( xm > x1 ) xr = xm - x1; else xr = x1 - xm;
      maustaste ^= maustaste;
      delay( 100 );
      while ( maustaste != 1 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
      if ( ym > y1 ) yr = ym - y1; else yr = y1 - ym;
      // Maus aus
      asm { mov ax, 0x0002; INT 0x33 }
      setcolor( globale_farbe );
      ellipse( x1, y1, 0, 360, xr, yr );
      putpixel( x1, y1, getbkcolor() );
      setactivepage( erste );
      yt > 280 ? verschiebung(), yt = 280 : yt += 10;
      setcolor( status_farbe );
      outtextxy( xt, yt, "Mittelpunkt (    ;    ) , Xr =     ; Yr = " );
      nachricht( x1, 75 );
      nachricht( y1, 105 );
      nachricht( xr, 175 );
      nachricht( yr, 240 );
      setactivepage( zweite );
      // Maus ein
      asm { mov ax, 0x0001; INT 0x33 }
      maustaste ^= maustaste;
      delay( 100 );
      while ( maustaste == 0 )
      {
	 asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
	 koo_ausgeben( xm, ym );
      }
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 };
   setactivepage( erste );
   setvisualpage( erste );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   setcolor( 2 ); // GrÅne Farbe
   outtextxy( xt, yt, "--> ...beendet." );
}
void laden( void )
{
   FILE *fp;
   char name[80] = { 0 };
   char zeichen[2] = { 0 };
   int xm, ym, maustaste = 0;
   int n, xkoo = 200;
   int taste = 0;
   int x1, x2, y1, y2, xh;
   unsigned long groesse;
   char satz[10];

   setcolor( 2 ); // Rot
   outtextxy( xt + 120, yt, "Dateiname :" );
   n ^= n;
   while ( taste != 13 )
   {
      taste = getch();
      if ( (taste > 32) && (taste < 123) )
      {
	 name[n] = ( char ) taste;
	 zeichen[0] = name[n];
	 outtextxy( xkoo += 7, yt, zeichen );
	 n++;
      }
   }
   if ( !( fp = fopen( name, "r" ) ) )
   {
      yt > 280 ? verschiebung(), yt = 280 : yt += 10;
      setcolor( 12 ); // GrÅne Farbe
      outtextxy( xt, yt, "--> Datei kann nicht gelesen werden !" );
      return;
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setactivepage( zweite );
   setvisualpage( zweite );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   delay( 100 );
   while ( maustaste != 1 )
   {
      asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
      koo_ausgeben( xm, ym );
   }
   x1 = xm;
   x2 = x1 + getc( fp );
   y1 = ym;
   y2 = y1 + getc( fp );
   groesse = ( x2 - x1 ) * ( y2 - y1) + 2;
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   for (; y1 < y2; y1++ ) for ( xh = x1; xh < x2; xh++ ) putpixel( xh, y1, getc( fp ) );
   fclose( fp );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   while ( maustaste != 2 )
   {
      asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx }
      koo_ausgeben( xm, ym );
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setactivepage( erste );
   setvisualpage( erste );
   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   setcolor( 2 ); // GrÅne Farbe
   outtextxy( xt, yt, "--> ...        Integerwerte erfolgreich geladen." );
   ultoa( groesse, satz, 10 );
   outtextxy( xt + 40, yt, satz );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
}
void speichern( void )
{
   FILE *fp;
   char name[80] = { 0 };
   char zeichen[2] = { 0 };
   int xm, ym, maustaste = 0;
   int n, xkoo = 220;
   int taste = 0;
   int x1, x2, y1, y2, xh;
   unsigned long groesse;
   char satz[10];

   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setcolor( 2 ); // Rot
   outtextxy( xt + 140, yt, "Dateiname :" );
   n ^= n;
   while ( taste != 13 )
   {
      taste = getch();
      if ( (taste > 32) && (taste < 123) )
      {
	 name[n] = ( char ) taste;
	 zeichen[0] = name[n];
	 outtextxy( xkoo += 7, yt, zeichen );
	 n++;
      }
   }
   if ( !( fp = fopen( name, "w" ) ) )
   {
      yt > 280 ? verschiebung(), yt = 280 : yt += 10;
      setcolor( 12 ); // GrÅne Farbe
      outtextxy( xt, yt, "--> Datei kann nicht erstellt werden !" );
      // Maus ein
      asm { mov ax, 0x0001; INT 0x33 }
      return;
   }
   setactivepage( zweite );
   setvisualpage( zweite );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
   delay( 100 );
   while ( maustaste != 1 )
   {
      asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
      koo_ausgeben( xm, ym );
   }
   x1 = xm;
   y1 = ym;
   maustaste ^= maustaste;
   delay( 100 );
   while ( maustaste != 1 )
   {
      asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
      koo_ausgeben( xm, ym );
   }
   x2 = xm;
   y2 = ym;
   groesse = ( x2 - x1 ) * ( y2 - y1 ) + 2;
   putc( x2 - x1, fp );
   putc( y2 - y1, fp );
   for ( ; y1 < y2; y1++ ) for ( xh = x1; xh < x2; xh++ ) putc( getpixel( xh, y1 ), fp );
   fclose( fp );
   maustaste ^= maustaste;
   delay( 100 );
   while ( maustaste != 2)
   {
      asm { mov ax, 0x0003; INT 0x33; mov maustaste, bx; mov xm, cx; mov ym, dx }
      koo_ausgeben( xm, ym );
   }
   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   setactivepage( erste );
   setvisualpage( erste );
   setcolor( 2 ); // GrÅne Farbe
   yt > 280 ? verschiebung(), yt = 280 : yt += 10;
   outtextxy( xt, yt, "--> ...        Integerwerte erfolgreich geschrieben." );
   ultoa( groesse, satz, 10 );
   outtextxy( xt + 40, yt, satz );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
}
void aktuelle_farbe( void )
{
   char satz[10];

   itoa( globale_farbe, satz, 10 );
   leeren( 10, 335, 100, 13, 0 );
   setcolor( globale_farbe );
   outtextxy( 15, 335, "Farbe = " );
   outtextxy( 60, 335, satz );
}