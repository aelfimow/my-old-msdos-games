#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>



unsigned char buchstaben[1070];
int hintergrund = 0,
    rahmen_farbe = 1,
    text_farbe = 15,
    markiert = 1,
    status = 15,
    gedrueckt = 12,
    alles_ok = 3,
    xm, ym, mt = 0, mtv = 10, // Mauskoordinaten, Maustaste, Maustaste vorher
    y[10] = { 0, 30, 60, 90, 120, 150, 180, 210, 240, 270 },
    xtt = 120,
    xt = xtt,
    yt = 20,
    aktuelle_farbe = 1;
unsigned char m0[] = { 16, 1, 7, 35, 41, 17, 19, 39, 59, 255 }, // Sagrusit
	      m1[] = { 36, 31, 45, 35, 1, 29, 19, 39, 59, 255 }, // Sochranit
	      m2[] = { 24, 19, 29, 19, 65, 255 }, // Linija
	      m3[] = { 38, 31, 49, 23, 1, 255 }, // Totschka
	      m4[] = { 22, 35, 41, 7, 255 }, // Krug
	      m5[] = { 60, 25, 25, 19, 33, 37, 255 }, // Ellips
	      m6[] = { 22, 19, 37, 39, 59, 255 }, // Kist
	      m7[] = { 46, 5, 11, 39, 255 }, // Cwet
	      m8[] = { 22, 31, 29, 11, 47, 255 }, // Konets
	      m9[] = { 35, 19, 37, 41, 29, 31, 23, 71, 255 }, // Risunok
	      m10[] = { 28, 1, 49, 11, 35, 39, 19, 39, 59, 255 }, // natschertit
	      m11[] = { 25, 19, 29, 19, 63, 71, 255 }, // liniju
	      m12[] = { 39, 31, 49, 23, 19, 71, 255 }, // totschki
	      m13[] = { 87, 87, 85, 255 }, // -->
	      m14[] = { 23, 35, 41, 7, 71, 255 }, // krug
	      m15[] = { 61, 25, 25, 19, 33, 37, 71, 255 }, // Ellips
	      m16[] = { 18, 37, 33, 31, 25, 59, 17, 31, 5, 1, 39, 59, 255 }, // ispolsovat
	      m17[] = { 23, 19, 37, 39, 59, 71, 255 }, // kist
	      m18[] = { 5, 5, 11, 37, 39, 19, 255 }, // vvesti
	      m19[] = { 29, 31, 5, 57, 21, 255 }, // novij
	      m20[] = { 47, 5, 11, 39, 255 }, // cwet
	      m21[] = { 22,31,31,35,9,19,29,1,39,57,106,27,57,51,19,73,106,106,78,106,106,106,87,106,106,106,106,79,255}, // Koordinati mischi
	      m_ende[] = { 41, 37, 33, 11, 51, 29, 31, 106, 31, 23, 31, 29, 49, 11, 29, 31, 69, 255 }; // uspeschno okontscheno

void schrift_einlesen( void );
static void bemalen( int x1, int y1, int l, int b, int f );
static void menue_ausgeben( int zahl );
static void satz_ausgeben( int xa, int ya, unsigned char *zeiger, int f );
static void verschieben( void );
static void koordinaten_ausgeben( void );
int neue_farbe( void );
void punkte_setzen( void );
void linie_zeichnen( void );
void spray( void );
void kreis_zeichnen( void );
void ellipse_zeichnen( void );
void bild_laden( void );
void datei_speichern( void );
void datei_laden( void );

int main( void )
{
   int dr = 9; // VGA
   int md = 1; // 640 * 350, 2 Seiten
   int n, vorher, zw;
   int beenden = 0;
   int auswahl = 0;


   schrift_einlesen(); // != 0 ) puts( "Fehler beim Einlesen der Schrift." ), exit( 0 );
   initgraph( &dr, &md, "" ); cleardevice();
   settextstyle( SMALL_FONT, HORIZ_DIR, 4 );
   setbkcolor( hintergrund );
   // Rahmen erstellen
   setcolor( rahmen_farbe );
   lineto( 0, 349 ), lineto( 639, 349 ), lineto( 639, 0 ), lineto( 0, 0 );
   line( 100, 0, 100, 349 );
   for ( n ^= n; n < 9; n++ ) menue_ausgeben( n ), line ( 0, y[n], 100, y[n] );
   bemalen( 0, y[9], 100, 349 - y[9], rahmen_farbe );
   // éhnliches auf der Gegenseite
   setactivepage( 1 );
   bemalen( 0, 332, 639, 16, rahmen_farbe );
   satz_ausgeben( 400, 336, m21, status );
   setactivepage( 0 );
   asm { xor ax, ax; INT 0x33 }
   asm { mov ax, 1; INT 0x33 }
   // Mausgrenzen festlegen
   asm { mov ax, 7; mov cx, 0; mov dx, 90; INT 0x33 }
   while ( beenden == 0 )
   {
      asm { mov ax, 3; INT 0x33; mov xm, cx; mov ym, dx; mov mt, bx }
      // Markierungsvorgang
      n > 7 ? n ^= n : n++;
      if ( (ym > y[n]) && (ym < y[n + 1]) )
      if ( vorher != n )
      {
	 auswahl = n;
	 asm { mov ax, 2; INT 0x33 }
	 bemalen( 2, y[vorher] + 2, 96, 26, hintergrund );
	 menue_ausgeben( vorher );
	 bemalen( 2, y[n] + 2, 96, 26, markiert );
	 menue_ausgeben( n );
	 asm { mov ax, 1; INT 0x33 }
	 vorher = n;
      }
      if ( mtv != mt )
      {
	 switch ( mt )
	 {
	    case 1:
		 asm { mov ax, 2; INT 0x33 }
		 zw = text_farbe, text_farbe = gedrueckt;
		 menue_ausgeben( auswahl );
		 text_farbe = zw;
		 switch ( auswahl )
		 {
		    case 0: // Datei speichern
			    satz_ausgeben( xt, yt, m13, status );
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m0, status );
			    satz_ausgeben( xt += sizeof(m0)*8, yt, m9, status );
			    datei_laden();
			    xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    break;
		    case 1: // Datei laden
			    satz_ausgeben( xt, yt, m13, status );
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m1, status );
			    satz_ausgeben( xt + sizeof(m1)*8, yt, m9, status );
			    datei_speichern();
			    xt = xtt;
			    yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    break;
		    case 2: // Linie zeichnen
			    satz_ausgeben( xt, yt, m13, status );
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m10, status );
			    satz_ausgeben( xt += sizeof(m10)*8, yt, m11, status );
			    linie_zeichnen();
			    xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m_ende, alles_ok );
			    xt = xtt;
			    yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    break;
		    case 3: // Punkte setzen
			    satz_ausgeben( xt, yt, m13, status );
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m12, status );
			    punkte_setzen();
			    xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m_ende, alles_ok );
			    xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    break;
		    case 4: // Kreis zeichnen
			    satz_ausgeben( xt, yt, m13, status );
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m10, status );
			    satz_ausgeben( xt += sizeof(m10)*8, yt, m14, status );
			    kreis_zeichnen();
			    xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m_ende, alles_ok );
			    xt = xtt;
			    yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    break;
		    case 5: // Ellipse zeichnen
			    satz_ausgeben( xt, yt, m13, status );
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m10, status );
			    satz_ausgeben( xt += sizeof(m10)*8, yt, m15, status );
			    ellipse_zeichnen();
			    xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m_ende, alles_ok );
			    xt = xtt;
			    yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    break;
		    case 6: // Spray benutzen
			    satz_ausgeben( xt, yt, m13, status );
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m16, status );
			    satz_ausgeben( xt += sizeof(m16)*8, yt, m17, status );
			    spray();
			    xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    satz_ausgeben( xt += sizeof(m13)*8, yt, m_ende, alles_ok );
			    xt = xtt;
			    yt > 260 ? verschieben(), yt = 236 : yt += 12;
			    break;

		    case 7: // Neue Farbe eingeben
			    aktuelle_farbe = neue_farbe();
			    break;

		    case 8: beenden++; break;

		    default: break;
		 }
		 asm { mov ax, 1; INT 0x33 }
		 // delay( 100 );
		 break;
	    case 2:
		 asm { mov ax, 2; INT 0x33 }
		 setactivepage( 1 );
		 setvisualpage( 1 );
		 asm { mov ax, 1; INT 0x33 }
		 // Mausgrenzen festlegen
		 asm { mov ax, 7; xor cx, cx; mov dx, 639; INT 0x33 }
		 while ( mt != 4 )
		 {
		    asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx }
		    koordinaten_ausgeben();
		 }
		 // Mausgrenzen festlegen
		 asm { mov ax, 7; mov cx, 0; mov dx, 90; INT 0x33 }
		 asm { mov ax, 2; INT 0x33 }
		 setactivepage( 0 );
		 setvisualpage( 0 );
		 asm { mov ax, 1; INT 0x33 }
		 break;

	    default: break;
	 }
	 mtv = mt;
      }
      if ( kbhit() != 0 ) if ( getch() == 27 ) beenden++;
   }
   asm { mov ax, 2; INT 0x33 }
   closegraph();
   return 0;
}
static void schrift_einlesen( void )
{
   FILE *fp;
   unsigned char *zeiger;
   int n;

   zeiger = buchstaben;
   fp = fopen( "russ01.buc", "r" );
   while ( feof( fp ) == 0 ) *zeiger++ = ( unsigned char ) getc( fp );
   fclose( fp );
}
static void bemalen( int x1, int y1, int l, int b, int f )
{
   int x2, y2;

   setlinestyle( SOLID_LINE, 0, THICK_WIDTH );
   setcolor( f );
   x2 = x1 + l;
   y2 = y1 + b;
   for (; y1 < y2; y1 += 3 ) line( x1, y1, x2, y1 );
   setlinestyle( SOLID_LINE, 0, NORM_WIDTH );
}
static void menue_ausgeben( int zahl )
{
   if ( zahl > 8 ) return;
   switch ( zahl )
   {
      case 0: satz_ausgeben( 10, y[zahl] + 12, m0, text_farbe ); break;
      case 1: satz_ausgeben( 10, y[zahl] + 12, m1, text_farbe ); break;
      case 2: satz_ausgeben( 10, y[zahl] + 12, m2, text_farbe ); break;
      case 3: satz_ausgeben( 10, y[zahl] + 12, m3, text_farbe ); break;
      case 4: satz_ausgeben( 10, y[zahl] + 12, m4, text_farbe ); break;
      case 5: satz_ausgeben( 10, y[zahl] + 12, m5, text_farbe ); break;
      case 6: satz_ausgeben( 10, y[zahl] + 12, m6, text_farbe ); break;
      case 7: satz_ausgeben( 10, y[zahl] + 12, m7, text_farbe ); break;
      case 8: satz_ausgeben( 10, y[zahl] + 12, m8, text_farbe ); break;
   }
}
static void satz_ausgeben( int xa, int ya, unsigned char *zeiger, int f )
{
   unsigned char *z;
   unsigned char sw;
   int xh, yh, xe, ye;

   ye = ya + 10;
   while ( *zeiger != 255 )
   {
      z = buchstaben + ((*zeiger) * 10);
//      z += ((*zeiger) * 10);
      xe = xa + 8;
      for ( yh = ya; yh < ye; yh++ )
      {
	 sw = 128;
	 for ( xh = xa; xh < xe; xh++ )
	 {
	    if ( (*z & sw) == sw ) putpixel( xh, yh, f );
	    sw >>= 1;
	 }
	 z++;
      }
      xa += 8;
      zeiger++;
   }
}
static void verschieben( void )
{
   unsigned int groesse;
   void *z;
   int x1 = xtt,
       y1 = 20,
       x2 = 600,
       y2 = 284;

   // Maus aus
   asm { mov ax, 0x0002; INT 0x33 }
   groesse = imagesize( x1, 30, x2, 66 );
   z = malloc( groesse );
   for ( y1 = 66; y1 < y2; y1 += 36 )
   {
      getimage( x1, y1, x2, y1 + 36, z );
      putimage( x1, y1 - 36, z, COPY_PUT );
   }
   free( z );
   // Maus ein
   asm { mov ax, 0x0001; INT 0x33 }
}
static void koordinaten_ausgeben( void )
{
   char satz[10];
   static xvor, yvor;

   if ( xm != xvor )
   {
      xvor = xm; // 560
      bemalen( 560, 335, 20, 14, rahmen_farbe );
      itoa( xm, satz, 10 );
      setcolor( status );
      outtextxy( 560, 334, satz );
   }
   if ( ym != yvor )
   {
      yvor = ym; // 570
      bemalen( 592, 335, 20, 14, rahmen_farbe );
      itoa( ym, satz, 10 );
      setcolor( status );
      outtextxy( 592, 334, satz );
   }
}
int neue_farbe( void )
{
   // vvedite novij cvet
   unsigned char mm1[] = { 87, 87, 85, 106, 4, 5, 11, 9, 19, 39, 11, 106, 29, 31, 5, 57, 21, 106, 47, 5, 11, 39, 106, 73, 255 },
		 zeichen[2] = { 0 };
   int zahl = 0, taste = 0;

   asm { mov ax, 2; INT 0x33 }
   // Meldung wird ausgegeben
   satz_ausgeben( xt, yt, mm1, 12 );
   xt += sizeof( mm1 )*8;
   setcolor( status );
   while ( (taste = getch()) != 13 )
   if ( (taste > 47) && (taste < 58) )
   {
      zeichen[0] = ( unsigned char )taste;
      //                   eine kleine Korrektur bei der Ausgabe
      outtextxy( xt += 6, yt - 2, zeichen );
      zahl *= 10;
      zahl += (taste - 48);
   }
   zahl > 15 ? zahl = 15 : zahl;
   bemalen( 10, 320, 80, 15, zahl );
   yt > 260 ? verschieben(), yt = 236 : yt += 12;
   xt = xtt;
   asm { mov ax, 1; INT 0x33 }
   return zahl;
}
void punkte_setzen( void )
{
   int t;

   asm { mov ax, 0x0007; xor cx, cx; mov dx, 639; INT 0x33 }
   asm { mov ax, 0x0008; xor cx, cx; mov dx, 330; INT 0x33 }
   asm { mov ax, 2; INT 0x33 };
   setactivepage( 1 );
   setvisualpage( 1 );
   while ( mt != 0 ) asm { mov ax, 0x0003; INT 0x33; mov mt, bx }
   while ( mt != 4 )
   {
      asm { mov ax, 0x0001; INT 0x33 }
      asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx }
      koordinaten_ausgeben();
      if ( mt == 1 )
      {
	 // Maus aus
	 asm { mov ax, 0x0002; INT 0x33 }
	 // Punkt wird gesetzt
	 putpixel( xm, ym, aktuelle_farbe );
	 // Maus ein
	 asm { mov ax, 0x0001; INT 0x33 }
      }
      if ( kbhit() != 0 ) getch();
   }
   asm { mov ax, 7; xor cx, cx; mov dx, 90; INT 0x33 }
   // Umschaltung
   asm { mov ax, 2; INT 0x33 };
   setactivepage( 0 );
   setvisualpage( 0 );
   asm { mov ax, 1; INT 0x33 }
}
void linie_zeichnen( void )
{
   unsigned char satz[10],
		 mm1[] = { 40, 37, 39, 1, 29, 31, 5, 19, 39, 11, 255 }, // ustanovite
		 mm2[] = { 29, 1, 49, 1, 25, 59, 29, 57, 11, 255 }, // natschalnije
		 mm3[] = { 23, 31, 29, 11, 49, 29, 57, 11, 255 }, // konetschnije
		 mm4[] = { 23, 31, 31, 35, 9, 19, 29, 1, 39, 57, 255 }, // koordinati
		 mm5[] = { 31, 39, 106, 78, 255 }, // ot (
		 mm6[] = { 106, 106, 106, 70, 106, 106, 106, 106, 79, 106, 9, 31, 106, 78, 255}, // ) do (
		 mm7[] = { 106, 106, 106, 70, 106, 106, 106, 106, 79, 255 };
   int x[2], y[2],
       zahl = 0,
       n;

   asm { mov ax, 0x0007; xor cx, cx; mov dx, 639; INT 0x33 }
   asm { mov ax, 0x0008; xor cx, cx; mov dx, 330; INT 0x33 }
   asm { mov ax, 2; INT 0x33 }
   setactivepage( 1 );
   setvisualpage( 1 );
//   bemalen( 0, 332, 639, 16, rahmen_farbe );
//   satz_ausgeben( 400, 336, m21, status );
   satz_ausgeben( 10, 336, mm1, status );
   satz_ausgeben( 10+sizeof(mm1)*8, 336, mm2, status );
   satz_ausgeben( 10+sizeof(mm1)*8+sizeof(mm2)*8, 336, mm4, status );
   while ( mt != 0 ) asm { mov ax, 3; INT 0x33; mov mt, bx };
   while ( mt != 4 )
   {
      if ( kbhit() != 0 ) getch();
      asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx }
      koordinaten_ausgeben();
      asm { mov ax, 1; INT 0x33 }
      if ( mt == 1 )
      {
	 x[zahl] = xm;
	 y[zahl] = ym;
	 putpixel( x[zahl], y[zahl], aktuelle_farbe );
	 bemalen( 10+sizeof(mm1)*8, 332, sizeof(mm2)*8, 16, rahmen_farbe );
	 if ( zahl == 0 ) satz_ausgeben( 10+sizeof(mm1)*8, 336, mm3, status );
	 else
	 {
	    satz_ausgeben( 10+sizeof(mm1)*8, 336, mm2, status );
	    asm { mov ax, 2; INT 0x33 }
	    setcolor( aktuelle_farbe );
	    line( x[0], y[0], x[1], y[1] );
	    setactivepage( 0 );
	    yt > 260 ? verschieben(), yt = 236 : yt += 12;
	    satz_ausgeben( xt = xtt+sizeof(m13)*8, yt, mm5, status );
	    satz_ausgeben( xt += sizeof(mm5)*8, yt, mm6, status );
	    satz_ausgeben( xt += sizeof(mm6)*8, yt, mm7, status );
	    setcolor( status );
	    itoa( x[0], satz, 10 ); outtextxy( xt = xtt+sizeof(m13)*8+sizeof(mm5)*8, yt - 2, satz );
	    itoa( y[0], satz, 10 ); outtextxy( xt = xtt+sizeof(m13)*8+sizeof(mm5)*8+40, yt - 2, satz );
	    itoa( x[1], satz, 10 ); outtextxy( xt = xtt+sizeof(m13)*8+sizeof(mm5)*8+sizeof(mm6)*8, yt - 2, satz );
	    itoa( y[1], satz, 10 ); outtextxy( xt = xtt+sizeof(m13)*8+sizeof(mm5)*8+sizeof(mm6)*8+40, yt - 2, satz );
	    setactivepage( 1 );
	 }
	 while ( mt != 0 ) asm { mov ax, 3; INT 0x33; mov mt, bx };
	 zahl = 1 - zahl;
      }
   }
   bemalen( 0, 332, 400, 16, rahmen_farbe );
   asm { mov ax, 2; INT 0x33 }
   asm { mov ax, 7; xor cx, cx; mov dx, 90; INT 0x33 }
   setactivepage( 0 );
   setvisualpage( 0 );
   asm { mov ax, 1; INT 0x33 }
}
void spray( void )
{
   unsigned char mm1[] = { 32,35,1,5,1,65,106,23,29,31,33,23,1,106,27,57,51,19,106,73,255 },
		 mm2[] = { 41,37,39,1,29,31,5,23,1,106,35,1,17,27,11,35,1,106,23,19,37,39,19,255 }, //
		 mm3[] = { 28,31,5,57,21,106,35,1,17,27,11,35,106,23,19,37,39,19,106,73,255 }; // novij rasmer kisti
   static int groesse;
   int anzahl, n,
       taste,
       zahl;
   unsigned char zeichen[2] = { 0 };

   asm { mov ax, 0x0007; xor cx, cx; mov dx, 639; INT 0x33 }
   asm { mov ax, 0x0008; xor cx, cx; mov dx, 330; INT 0x33 }
   asm { mov ax, 2; INT 0x33 }
   setactivepage( 1 );
   setvisualpage( 1 );
   satz_ausgeben( 10, 336, mm1, status );
   satz_ausgeben( 10+sizeof(mm1)*8, 336, mm2, status );
   while ( mt != 0 ) asm { mov ax, 3; INT 0x33; mov mt, bx }
   while ( mt != 4 )
   {
      asm { mov ax, 1; INT 0x33 }
      asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx }
      koordinaten_ausgeben();
      if ( mt == 1 )
      {
	 anzahl = groesse >> 1;
	 asm { mov ax, 2; INT 0x33 }
	 for ( n ^= n; n < anzahl; n++ )
	 {
	    putpixel( xm - random( groesse ), ym - random( groesse ), aktuelle_farbe );
	    putpixel( xm + random( groesse ), ym - random( groesse ), aktuelle_farbe );
	    if ( (ym + groesse) < 330 )
	    {
	       putpixel( xm - random( groesse ), ym + random( groesse ), aktuelle_farbe );
	       putpixel( xm + random( groesse ), ym + random( groesse ), aktuelle_farbe );
	    }
	 }
	 while ( mt != 0 ) asm { mov ax, 3; INT 0x33; mov mt, bx }
      }
      if ( mt == 2 )
      {
	 asm { mov ax, 2; INT 0x33 }
	 setactivepage( 0 );
	 setvisualpage( 0 );
	 yt > 260 ? verschieben(), yt = 236 : yt += 12;
	 xt = xtt;
	 satz_ausgeben( xt += sizeof(m13)*8, yt, mm3, status );
	 xt += sizeof( mm3 )*8;
	 zahl ^= zahl;
	 while ( (taste = getch()) != 13 )
	 if ( (taste > 47) && (taste < 58) )
	 {
	    zahl *= 10;
	    zahl += (taste - 48);
	    zeichen[0] = ( unsigned char ) taste;
	    outtextxy( xt += 6, yt - 2, zeichen );
	 }
	 groesse = zahl;
	 setactivepage( 1 );
	 setvisualpage( 1 );
      }
      if ( kbhit() != 0 ) getch();
   }
   bemalen( 0, 332, 400, 16, rahmen_farbe );
   asm { mov ax, 2; INT 0x33 }
   asm { mov ax, 7; xor cx, cx; mov dx, 90; INT 0x33 }
   setactivepage( 0 );
   setvisualpage( 0 );
   asm { mov ax, 1; INT 0x33 }
}
void kreis_zeichnen( void )
{
   unsigned char mm1[] = { 4,5,11,9,19,39,11,255 }, // Vvedite
		 mm2[] = { 37,35,11,9,29,63,63,106,39,31,49,23,41,255 }, // sredneju totschku
		 mm3[] = { 35,1,9,19,41,37,106,73,255 }, // radius
		 mm4[] = { 36,35,11,9,29,65,65,106,39,31,49,23,1,106,78,106,106,106,106,70,106,106,106,106,79,106,70,106,35,1,9,19,41,37,106,73,255 };
   int taste,
       zahl,
       x1, y1, x;
   unsigned char zeichen[2] = { 0 };
   unsigned char satz[10];

   asm { mov ax, 0x0007; xor cx, cx; mov dx, 639; INT 0x33 }
   asm { mov ax, 0x0008; xor cx, cx; mov dx, 330; INT 0x33 }
   asm { mov ax, 2; INT 0x33 }
   setactivepage( 1 );
   setvisualpage( 1 );
   satz_ausgeben( 10, 336, mm1, status );
   satz_ausgeben( 10+sizeof(mm1)*8, 336, mm2, status );
   while ( mt != 0 ) asm { mov ax, 3; INT 0x33; mov mt, bx }
   while ( mt != 4 )
   {
      if ( kbhit() != 0 ) getch();
      asm { mov ax, 1; INT 0x33 }
      asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx }
      koordinaten_ausgeben();
      if ( mt == 1 )
      {
	 asm { mov ax, 2; INT 0x33 }
	 x1 = xm;
	 y1 = ym;
	 putpixel( x1, y1, aktuelle_farbe );
	 bemalen( 10+sizeof(mm1)*8, 332, sizeof(mm2)*8, 16, rahmen_farbe );
	 satz_ausgeben( 10+sizeof(mm1)*8, 336, mm3, status );
	 zahl ^= zahl;
	 x = 10+sizeof(mm1)*8+sizeof(mm3)*8;
	 setcolor( status );
	 while ( (taste = getch()) != 13 )
	 if ( (taste > 47) && (taste < 58) )
	 {
	    zahl *= 10;
	    zahl += ( taste - 48 );
	    zeichen[0] = ( unsigned char ) taste;
	    outtextxy( x += 6, 334, zeichen );
	 }
	 setcolor( aktuelle_farbe );
	 circle( x1, y1, zahl );
	 putpixel( x1, y1, getbkcolor() );
	 setactivepage( 0 );
	 setcolor( status );
	 yt > 260 ? verschieben(), yt = 236 : yt += 12;
	 xt = xtt;
	 satz_ausgeben( xt += sizeof(m13)*8, yt, mm4, status );
	 itoa( x1, satz, 10 );
	 outtextxy( xtt+sizeof(m13)*8+128, yt - 2, satz );
	 itoa( y1, satz, 10 );
	 outtextxy( xtt+sizeof(m13)*8+168, yt - 2, satz );
	 itoa( zahl, satz, 10 );
	 outtextxy( xtt+sizeof(m13)*8+sizeof(mm4)*8, yt - 2, satz );
	 setactivepage( 1 );
	 bemalen( 10+sizeof(mm1)*8, 332, sizeof(mm2)*8, 16, rahmen_farbe );
	 satz_ausgeben( 10+sizeof(mm1)*8, 336, mm2, status );
      }
   }
   bemalen( 0, 332, 400, 16, rahmen_farbe );
   asm { mov ax, 2; INT 0x33 }
   asm { mov ax, 7; xor cx, cx; mov dx, 90; INT 0x33 }
   setactivepage( 0 );
   setvisualpage( 0 );
   asm { mov ax, 1; INT 0x33 }
}
void ellipse_zeichnen( void )
{
   unsigned char mm1[] = { 4,5,11,9,19,39,11,255 }, // Vvedite
		 mm2[] = { 37,35,11,9,29,63,63,106,39,31,49,23,41,255 }, // sredneju totschku
		 mm3[] = { 35,1,9,19,41,37,255 }, // radius
		 mm4[] = { 45,73,255 }, // x -
		 mm5[] = { 41,73,255 }, // y -
		 mm6[] = { 36,35,11,9,29,65,65,106,39,31,49,23,1,106,78,106,106,106,106,70,106,106,106,106,79,106,70,106,45,35,86,106,106,106,106,70,106,41,35,86,255 };
   int taste,
       zahl,
       x1, y1, x,
       xr, yr;

   unsigned char zeichen[2] = { 0 };
   unsigned char satz[10];
   asm { mov ax, 0x0007; xor cx, cx; mov dx, 639; INT 0x33 }
   asm { mov ax, 0x0008; xor cx, cx; mov dx, 330; INT 0x33 }
   asm { mov ax, 2; INT 0x33 }
   setactivepage( 1 );
   setvisualpage( 1 );
   satz_ausgeben( 10, 336, mm1, status );
   satz_ausgeben( 10+sizeof(mm1)*8, 336, mm2, status );
   while ( mt != 0 ) asm { mov ax, 3; INT 0x33; mov mt, bx }
   while ( mt != 4 )
   {
      if ( kbhit() != 0 ) getch();
      asm { mov ax, 1; INT 0x33 }
      asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx }
      koordinaten_ausgeben();
      if ( mt == 1 )
      {
	 asm { mov ax, 2; INT 0x33 }
	 x1 = xm;
	 y1 = ym;
	 putpixel( x1, y1, aktuelle_farbe );
	 bemalen( 10+sizeof(mm1)*8, 332, 390-sizeof(mm1)*8, 16, rahmen_farbe );
	 satz_ausgeben( 10+sizeof(mm1)*8, 336, mm3, status );
	 satz_ausgeben( 10+sizeof(mm1)*8+sizeof(mm3)*8, 336, mm4, status );
	 x = 10+sizeof(mm1)*8+sizeof(mm3)*8+sizeof(mm4)*8;
	 zahl ^= zahl;
	 setcolor( status );
	 while ( (taste = getch()) != 13 )
	 if ( (taste > 47) && (taste < 58) )
	 {
	    zahl *= 10;
	    zahl += ( taste - 48 );
	    zeichen[0] = ( unsigned char ) taste;
	    outtextxy( x += 6, 334, zeichen );
	 }
	 xr = zahl;
	 zahl ^= zahl;
	 bemalen( 10+sizeof(mm1)*8+sizeof(mm3)*8, 332, 400-sizeof(mm4)*8-sizeof(mm1)*8-sizeof(mm3)*8, 16, rahmen_farbe );
	 satz_ausgeben( 10+sizeof(mm1)*8+sizeof(mm3)*8, 336, mm5, status );
	 x = 10+sizeof(mm1)*8+sizeof(mm3)*8+sizeof(mm5)*8;
	 setcolor( status );
	 while ( (taste = getch()) != 13 )
	 if ( (taste > 47) && (taste < 58) )
	 {
	    zahl *= 10;
	    zahl += ( taste - 48 );
	    zeichen[0] = ( unsigned char ) taste;
	    outtextxy( x += 6, 334, zeichen );
	 }
	 yr = zahl;
	 setcolor( aktuelle_farbe );
	 ellipse( x1, y1, 0, 360, xr, yr );
	 putpixel( x1, y1, getbkcolor() );
	 setactivepage( 0 );
	 setcolor( status );
	 yt > 260 ? verschieben(), yt = 236 : yt += 12;
	 xt = xtt;
	 satz_ausgeben( xt+sizeof(m13)*8, yt, mm6, status );
	 itoa( x1, satz, 10 );
	 outtextxy( xtt+sizeof(m13)*8+128, yt-2, satz );
	 itoa( y1, satz, 10 );
	 outtextxy( xtt+sizeof(m13)*8+168, yt-2, satz );
	 itoa( xr, satz, 10 );
	 outtextxy( xtt+sizeof(m13)*8+255, yt-2, satz );
	 itoa( yr, satz, 10 );
	 outtextxy( xtt+sizeof(m13)*8+328, yt-2, satz );
	 setactivepage( 1 );
	 bemalen( 10+sizeof(mm1)*8, 332, 390-sizeof(mm1)*8, 16, rahmen_farbe );
	 satz_ausgeben( 10+sizeof(mm1)*8, 336, mm2, status );
      }
   }
   bemalen( 0, 332, 400, 16, rahmen_farbe );
   asm { mov ax, 2; INT 0x33 }
   asm { mov ax, 7; xor cx, cx; mov dx, 90; INT 0x33 }
   setactivepage( 0 );
   setvisualpage( 0 );
   asm { mov ax, 1; INT 0x33 }
}
void datei_speichern( void )
{
   unsigned char mm1[] = { 4,5,11,9,19,39,11,106,19,27,65,106,43,1,21,25,1,106,73,255 },
		 mm2[] = { 30,51,19,3,23,1,106,66,255 }, // Oschibka
		 mm3[] = { 40,37,39,1,29,31,5,19,39,11,106,23,31,31,35,9,19,29,1,39,57,255 }, // ustanovite koordinati
		 mm5[] = { 5,11,35,45,29,11,7,31,106,41,7,25,1,255 }, // vverchnego ugla
		 mm4[] = { 29,19,15,29,11,7,31,106,41,7,25,1,255 }; // nischnego ugla

   int xkk[2], ykk[2], xh;
   FILE *fp;
   char name[80] = { 0 };
   int taste, n;
   unsigned char zeichen[2] = { 0 };
   int v;

   asm { mov ax, 0x0007; xor cx, cx; mov dx, 639; INT 0x33 }
   asm { mov ax, 0x0008; xor cx, cx; mov dx, 330; INT 0x33 }
   asm { mov ax, 2; INT 0x33 }
   setactivepage( 1 );
   satz_ausgeben( 10, 336, mm3, status );
   satz_ausgeben( 10+sizeof(mm3)*8, 336, mm5, status );
   setactivepage( 0 );
   xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
   satz_ausgeben( xt += sizeof(m13)*8, yt, mm1, status );
   xt += sizeof(mm1)*8;
   n ^= n;
   setcolor( status );
   while ( (taste = getch()) != 13 )
   if ( (taste > 32) && (taste < 127) )
   {
      zeichen[0] = name[n++] = ( unsigned char )taste;
      outtextxy( xt += 6, yt - 2, zeichen );
   }
   if ( !(fp = fopen( name, "w" )) )
   {
      xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
      satz_ausgeben( xt += sizeof(m13)*8, yt, mm2, 12 );
      asm { mov ax, 1; INT 0x33; }
      return;
   }
   setactivepage( 1 );
   setvisualpage( 1 );
   v ^= v;
   while ( mt != 4 )
   {
      if ( kbhit() != 0 ) getch();
      asm { mov ax, 1; INT 0x33; }
      asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx; }
      koordinaten_ausgeben();
      if ( mt == 1 )
      {
	 xkk[v] = xm;
	 ykk[v] = ym;
	 putpixel( xkk[v], ykk[v], aktuelle_farbe );
	 bemalen( 10+sizeof(mm3)*8, 332, 390-sizeof(mm3)*8, 16, rahmen_farbe );
	 switch ( v )
	 {
	    case 0: satz_ausgeben( 10+sizeof(mm3)*8, 336, mm4, status ); break;
	    case 1: satz_ausgeben( 10+sizeof(mm3)*8, 336, mm5, status ); break;
	 }
	 v = 1 - v;
	 while ( mt != 0 ) asm { mov ax, 3; INT 0x33; mov mt, bx; }
      }
   }
   if ( (ykk[1] < ykk[0]) || (xkk[1] < xkk[0]) )
   {
      asm { mov ax, 2; INT 0x33; }
      asm { mov ax, 7; xor cx, cx; mov dx, 90; INT 0x33; }
      setactivepage( 0 );
      setvisualpage( 0 );
      xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
      satz_ausgeben( xt += sizeof(m13)*8, yt, mm2, 12 );
      fclose( fp );
      asm { mov ax, 1; INT 0x33; }
      return;
   }
   putc( xkk[1] - xkk[0], fp );
   putc( ykk[1] - ykk[0], fp );
   asm { mov ax, 2; INT 0x33; }
   for (; ykk[0] < ykk[1]; ykk[0]++ ) for ( xh = xkk[0]; xh < xkk[1]; xh++ ) putc( getpixel( xh, ykk[0] ), fp );
   fclose( fp );
   bemalen( 0, 332, 400, 16, rahmen_farbe );
   asm { mov ax, 2; INT 0x33 }
   asm { mov ax, 7; xor cx, cx; mov dx, 90; INT 0x33 }
   setactivepage( 0 );
   setvisualpage( 0 );
   asm { mov ax, 1; INT 0x33 }
   xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
   satz_ausgeben( xt += sizeof(m13)*8, yt, m_ende, alles_ok );
}
void datei_laden( void )
{
   unsigned char mm1[] = { 4,5,11,9,19,39,11,106,19,27,65,106,43,1,21,25,1,106,73,255 },
		 mm2[] = { 30,51,19,3,23,1,106,66,255 }, // Oschibka
		 mm3[] = { 40,37,39,1,29,31,5,19,39,11,106,23,31,31,35,9,19,29,1,39,57,106,5,11,35,45,29,11,7,31,106,41,7,25,1,255 };
   int x1, y1, x2, y2, xh;
   FILE *fp;
   char name[80] = { 0 };
   int taste, n;
   unsigned char zeichen[2] = { 0 };
   int v, f;

   asm { mov ax, 0x0007; xor cx, cx; mov dx, 639; INT 0x33 }
   asm { mov ax, 0x0008; xor cx, cx; mov dx, 330; INT 0x33 }
   asm { mov ax, 2; INT 0x33 }
   setactivepage( 1 );
   satz_ausgeben( 10, 336, mm3, status );
   setactivepage( 0 );
   xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
   satz_ausgeben( xt += sizeof(m13)*8, yt, mm1, status );
   xt += sizeof(mm1)*8;
   n ^= n;
   setcolor( status );
   while ( (taste = getch()) != 13 )
   if ( (taste > 32) && (taste < 127) )
   {
      zeichen[0] = name[n++] = ( unsigned char )taste;
      outtextxy( xt += 6, yt - 2, zeichen );
   }
   if ( !(fp = fopen( name, "r" )) )
   {
      xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
      satz_ausgeben( xt += sizeof(m13)*8, yt, mm2, 12 );
      asm { mov ax, 1; INT 0x33; }
      return;
   }
   setactivepage( 1 );
   setvisualpage( 1 );
   v ^= v;
   while ( mt != 2 )
   {
      if ( kbhit() != 0 ) getch();
      asm { mov ax, 1; INT 0x33; }
      asm { mov ax, 3; INT 0x33; mov mt, bx; mov xm, cx; mov ym, dx; }
      koordinaten_ausgeben();
      if ( mt == 1 )
      {
	 x1 = xm;
	 y1 = ym;
	 while ( mt != 0 ) asm { mov ax, 3; INT 0x33; mov mt, bx; }
      }
   }
   x2 = x1 + getc( fp );
   y2 = y1 + getc( fp );
   asm { mov ax, 0x0002; INT 0x33 }
   for (; y1 < y2; y1++ ) for ( xh = x1; xh < x2; xh++ )
   if ( (f = getc( fp )) != 0 ) putpixel( xh, y1, f );
   while ( mt != 4 ) asm { mov ax, 3; INT 0x33; mov mt, bx; }
   fclose( fp );
   bemalen( 0, 332, 400, 16, rahmen_farbe );
   asm { mov ax, 7; xor cx, cx; mov dx, 90; INT 0x33 }
   setactivepage( 0 );
   setvisualpage( 0 );
   asm { mov ax, 1; INT 0x33 }
   xt = xtt; yt > 260 ? verschieben(), yt = 236 : yt += 12;
   satz_ausgeben( xt += sizeof(m13)*8, yt, m_ende, alles_ok );
}
