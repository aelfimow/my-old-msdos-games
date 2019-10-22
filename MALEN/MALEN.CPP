#include <dos.h>
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>


int maus_ermitteln( void )
{
   int n_tasten;

   asm { XOR AX, AX; INT 0x33; MOV n_tasten, BX }
   return ( n_tasten );
}

void maus( int ein_oder_aus )    // Maus ein-, ausschalten
{
   if ( ein_oder_aus != 0 )
      asm { XOR AX, AX; INC AX; INT 0x33 }
   else
      asm { MOV AX, 0x0002; INT 0x33 }
}

void maus_koordinaten_in( unsigned int *zeiger ) // Koordinaten ermitteln
{
   unsigned int x, y, tasten,
		*z;

   asm { MOV AX, 0x0003; INT 0x33; MOV x, CX; MOV y, DX; MOV tasten, BX }
   z = zeiger;
   *z++ = tasten;
   *z++ = x;
   *z = y;
}

void vertikale_grenzen( unsigned int oben, unsigned int unten )
{
   asm { MOV AX, 0x0008; MOV CX, oben; MOV DX, unten; INT 0x33 }
}

void horizontale_grenzen( unsigned int links, unsigned int rechts )
{
   asm { MOV AX, 0x0007; MOV CX, links; MOV DX, rechts; INT 0x33 }
}


void maus_geh_zu( int x, int y ) // Position des Mauszeigers festlegen
{
   asm { MOV AX, 0x0004; MOV CX, x; MOV DX, y; INT 0x33 }
}

unsigned int koordinaten_ausgeben( void )
{
   unsigned int kw[3];
   static unsigned int akw[3];
   unsigned char xkoosatz[5],
		 ykoosatz[5],
		 *xkoosatzzeiger,
		 *ykoosatzzeiger;
   int koofarbe = 14,
       y;

   maus_koordinaten_in( kw );
   if ( kw[1] != akw[1] )
   {
      akw[1] = kw[1];
      xkoosatzzeiger = xkoosatz;
      itoa( kw[1], xkoosatz, 10 );
      setcolor( 0 );
      for ( y = 320; y < 334; y++ ) line( 455, y, 495, y );// line( 537, y, 577, y );
      setcolor( koofarbe );
      outtextxy( 460, 320, xkoosatzzeiger );
   }
   if ( kw[2] != akw[2] )
   {
      akw[2] = kw[2];
      ykoosatzzeiger = ykoosatz;
      itoa( kw[2], ykoosatz, 10 );
      setcolor( 0 );
      for ( y = 320; y < 334; y++ ) line( 537, y, 577, y );
      setcolor( koofarbe );
      outtextxy( 540, 320, ykoosatzzeiger );
   }
   return ( kw[0] );
}

int grenzen_testen( int x1, int x2, int y1, int y2 )
{
   unsigned int kw[3];

   maus_koordinaten_in( kw );
// Hier wird es geprft, ob die Mauskoordinaten sich
// innerhalb der festgelegten Grenzen befinden
   if ( (kw[1] > x1) && (kw[1] < x2) && (kw[2] > y1) && (kw[2] < y2) )
      return( 1 );
   else return( 0 );
}

void worte_ausgeben( int m )
{
   const int x[9] = { 10, 80, 150, 220, 290, 360, 430, 500, 570 },
	     y = 1;

   switch ( m )
   {
      case 0: outtextxy( x[m], y, "Speichern" ); break;
      case 1: outtextxy( x[m], y, "  Laden" ); break;
      case 2: outtextxy( x[m], y, "  Kreis" ); break;
      case 3: outtextxy( x[m], y, " Ellipse" ); break;
      case 4: outtextxy( x[m], y, "  Linie" ); break;
      case 5: outtextxy( x[m], y, "  Punkt" ); break;
      case 6: outtextxy( x[m], y, "  Spray" ); break;
      case 7: outtextxy( x[m], y, "  Farbe" ); break;
      case 8: outtextxy( x[m], y, "Beenden" ); break;
   }
}


void auswahl_zeigen( void )
{
   const int x[10] = { 10, 80, 150, 220, 290, 360, 430, 500, 570, 639 },
//	     y = 1,
	     tf = 15,
	     neue_farbe = 12;
   int yh, n;
   static int m; // m muss im Speicher bleiben

   m > 8 ? 0 : m;
   for ( n ^= n; n <= 8; n++ )
   {
      // Die Grenzen werden getestet, wenn ja, dann wird
      if ( (grenzen_testen( x[n] + 10, x[n + 1] - 10, 0, 13 ) == 1) && ( m != n ) )
      {
	 // Maus ausgeschaltet, um einige negative Effekte auszuschliessen
	 maus( 0 );
	 // die Stelle  gel”scht
	 setcolor( 0 );
	 for ( yh = 1; yh < 14; yh++ ) line( x[n], yh, x[n] + 50, yh );
	 // sichtbare Farbe und entsprechendes Wort ausgegeben
	 setcolor( neue_farbe ), worte_ausgeben( n );
	 // die alte Stelle gel”scht
	 setcolor( 0 );
	 for ( yh = 1; yh < 14; yh++ ) line( x[m], yh, x[m] + 50, yh );
	 // und entsprechendes Wort ausgegeben
	 setcolor( tf );
	 if ( m != n ) worte_ausgeben( m );
	 // Alte Stelle ist jetzt die neue Stelle
	 m = n;
      }
   }
   // Maus einschalten
   maus( 1 );
}

void speichern( void )
{
   char datei_name[80] = { 0 };
   FILE *fp;
   int x = 180, y, x1, x2, y1, y2,
       taste, n;
   unsigned char zeichen[2] = { 0, '\000' };
   unsigned int kw[3];
   unsigned long groesse;


   maus_geh_zu( 20, 20 );
   maus( 0 );
   // Neue Fenstergrenzen fr die Maus werden festgelegt
   vertikale_grenzen( 17, 317 );
   setcolor( 15 );
   outtextxy( 90, 320, "Bild speichern" );
   setcolor( 14 );
   outtextxy( 10, 335, "Dateiname eingeben : -->" );
   // Dateiname wird eingegeben
   n ^= n;
   while ( taste != 13 )
   {
      taste = getch();
      if ( (taste > 32) && (taste < 123) )
      {
	 datei_name[n] = ( char ) taste;
	 zeichen[0] = datei_name[n];
	 outtextxy( x += 7, 335, zeichen );
	 n++;
      }
   }
   maus( 1 );
   // Testen, ob man die Datei ”ffnen kann
   if ( !( fp = fopen( datei_name, "w" ) ) )
   {
      // Anweisung wird gel”scht
      setcolor( 0 );
      for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
      setcolor( 12 );
      outtextxy( 10, 335, "Datei kann nicht erstellt werden .    Beliebige Taste drcken..." );
      while ( kbhit() == 0 );
      taste = getch();
      vertikale_grenzen( 0, 317 );
      setcolor( 0 );
      for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
      for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
      return;
   }
   // Untere Ausgaben l”schen
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   // Neue Ausgabe
   setcolor( 14 );
   outtextxy( 10, 335, "Linke obere Ecke mit der linken Maustaste festlegen..." );
   while ( koordinaten_ausgeben() == 0 );
   maus_koordinaten_in( kw );
   x1 = kw[1]; y1 = kw[2];
   // Untere Ausgaben l”schen
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   // Neue Ausgabe
   setcolor( 14 );
   outtextxy( 10, 335, "Rechte untere Ecke mit der linken Maustaste festlegen..." );
   delay( 1000 );
   while ( koordinaten_ausgeben() == 0 );
   maus_koordinaten_in( kw );
   x2 = kw[1]; y2 = kw[2];
   if ( (x2 < x1) || (y2 < y1) )
   {
      setcolor( 0 );
      for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
      setcolor( 12 );
      outtextxy( 10, 335, "Falsche Fensterkoordinaten .    Beliebige Taste drcken..." );
      while ( kbhit() == 0 );
      taste = getch();
      vertikale_grenzen( 0, 317 );
      setcolor( 0 );
      for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
      for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
      return;
   }
   groesse = (x2 - x1) * ( y2 - y1);
   if ( groesse > 65000 )
   {
      setcolor( 0 );
      for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
      setcolor( 12 );
      outtextxy( 10, 335, "Bild zu gross .    Beliebige Taste drcken..." );
      while ( kbhit() == 0 );
      taste = getch();
      vertikale_grenzen( 0, 317 );
      setcolor( 0 );
      for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
      for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
      return;
   }
   putc( x2 - x1, fp );
   putc( y2 - y1, fp );
   for ( ; y1 != y2; y1++ ) for ( n = x1; n != x2; n++ ) putc( getpixel( n, y1 ), fp );
   // Die Datei wird geschlossen
   fclose( fp );
   vertikale_grenzen( 0, 317 );
   // Alle Ausgaben werden gel”scht
   setcolor( 0 );
   for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
}

void laden( void )
{
   char datei_name[80] = { 0 };
   FILE *fp;
   int x = 180, y,
       taste, n, x1, y1, x2, y2;
   unsigned char zeichen[2] = { 0, '\000' };
   unsigned int kw[3];

   maus_geh_zu( 20, 20 );
   maus( 0 );
   // Neue Fenstergrenzen fr die Maus werden festgelegt
   vertikale_grenzen( 17, 317 );
   setcolor( 15 );
   outtextxy( 90, 320, "Bild laden" );
   setcolor( 14 );
   outtextxy( 10, 335, "Dateiname eingeben : -->" );
   // Dateiname wird eingegeben
   n ^= n;
   while ( taste != 13 )
   {
      taste = getch();
      if ( (taste > 32) && (taste < 123) )
      {
	 datei_name[n] = ( char ) taste;
	 zeichen[0] = datei_name[n];
	 outtextxy( x += 7, 335, zeichen );
	 n++;
      }
   }
   // Testen, ob man die Datei ”ffnen kann
   if ( !( fp = fopen( datei_name, "r" ) ) )
   {
      // Anweisung wird gel”scht
      setcolor( 0 );
      for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
      setcolor( 12 );
      outtextxy( 10, 335, "Datei kann nicht ge”ffnet werden .    Beliebige Taste drcken..." );
      maus( 1 );
      while ( kbhit() == 0 );
      taste = getch();
      vertikale_grenzen( 0, 317 );
      setcolor( 0 );
      for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
      for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
      return;
   }
   // Anweisung wird gel”scht
   maus( 1 );
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   setcolor( 14 );
   outtextxy( 10, 335, "Koordinaten der linken oberen Ecke mit der linken Maustaste festlegen..." );
   while ( koordinaten_ausgeben() == 0 );
   maus_koordinaten_in( kw );
   x1 = kw[1]; y1 = kw[2];
   x2 = x1 + getc( fp );
   y2 = y1 + getc( fp );
   maus( 0 );
   for ( ; y1 != y2; y1++ ) for ( n = x1; n != x2; n++ ) putpixel( n, y1, getc( fp ) );
   maus( 1 );
   // Die Datei wird geschlossen
   fclose( fp );
   vertikale_grenzen( 0, 317 );
   // Alle Ausgaben werden gel”scht
   setcolor( 0 );
   for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
}

void linie( int aktuelle_farbe )
{
   int taste = 0,
       x1, y1, y;
   unsigned int kw[3];

   maus_geh_zu( 20, 20 );
   // Neue Fenstergrenzen fr die Maus werden festgelegt
   vertikale_grenzen( 17, 317 );
   setcolor( 15 );
   outtextxy( 90, 320, "Linie zeichnen" );
   setcolor( 14 );
   outtextxy( 10, 335, "Anfangskoordinaten festlegen ..." );
   delay( 1000 );
   while ( taste != 27 ) // Taste auf ESC berprfen
   {
      if ( kbhit() == 0 )
      {
	 if ( koordinaten_ausgeben() == 1 )
	 {
	    taste = 27;
	    maus_koordinaten_in( kw );
	    putpixel( x1 = kw[1], y1 = kw[2], aktuelle_farbe );
	 }
      } else taste = getch();
   }
   taste ^= taste;
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   setcolor( 14 );
   outtextxy( 10, 335, "Endkoordinaten festlegen ... Die Linie wird dann gezeichnet." );
   delay( 1000 );
   while ( taste != 27 ) // Taste auf ESC berprfen
   {
      if ( kbhit() == 0 )
      {
	 if ( koordinaten_ausgeben() == 1 )
	 {
	    taste = 27;
	    maus_koordinaten_in( kw );
	    maus ( 0 );
	    setcolor( aktuelle_farbe );
	    line( x1, y1, kw[1], kw[2] );
	    maus ( 1 );
	 }
      } else taste = getch();
   }
   // Alle Ausgaben werden gel”scht
   setcolor( 0 );
   for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   // Alte Fensterkoordinaten fr die Maus werden wieder hergestellt
   vertikale_grenzen( 0, 317 );
}

void spray( int aktuelle_farbe )
{
   int taste = 0,
       breite = 10,
       y, x = 170, x1, y1, farbe;
   unsigned int kw[3],
		n, m;
   unsigned char zeichen[2] = { 0, '\000' };

   farbe = aktuelle_farbe;
   // Mausgrenzen werden festgelegt
   maus_geh_zu( 20, 20 );
   vertikale_grenzen( 17, 317 );
   // Befehl wird ausgegeben
   setcolor( 15 );
   outtextxy( 90, 320, "Spray" );
   // Anweisung wird ausgegeben
   setcolor( 14 );
   outtextxy( 10, 335, "ENTER - Breite einstellen.          ESC - Vorgang abbrechen." );
   // Kurze Verz”gerung, um negative Effekte zu vermeiden
   delay( 500 );
   while ( taste != 27 ) // Taste auf ESC prfen
   {
      if ( kbhit() == 0 )
      {
	 if ( koordinaten_ausgeben() == 1 )
	 {
	    maus_koordinaten_in( kw );
	    maus( 0 );
	    delay( 50 );
	    x1 = kw[1];
	    y1 = kw[2];
	    // Anzahl der Punkte wird bestimmt und dann durch 2 geteilt
	    m = breite * breite;
	    m >>= 1;
	    // Die Punkte werden gesetzt
	    for ( n ^= n; n < m; n++ )
	    {
	       putpixel( x1 - random( breite ), y1 - random( breite ), farbe );
	       putpixel( x1 + random( breite ), y1 - random( breite ), farbe );
	       putpixel( x1 - random( breite ), y1 + random( breite ), farbe );
	       putpixel( x1 + random( breite ), y1 + random( breite ), farbe );
	    }
	    maus( 1 );
	 }
      } else taste = getch();
      if ( taste == 13 ) // Wenn ENTER gedrckt
      {
	 taste ^= taste;
	 setcolor( 0 );
	 for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
	 setcolor( 14 );
	 outtextxy( 10, 335, "Breite eingeben : -->" );
	 maus( 0 );
	 breite ^= breite;
	 while ( taste != 13 ) // Taste auf ENTER prfen
	 {
	    taste = getch();
	    if ( (taste >= 48) && (taste < 58) )
	    {
	       // Breite wird ausgerechnet
	       breite *= 10, breite += (taste - 48);
	       zeichen[0] = ( unsigned char ) taste;
	       outtextxy( x += 7, 335, zeichen );
	    }
	 }
	 maus( 1 );
	 taste ^= taste;
      }
   }
   // Alle Ausgaben werden gel”scht
   setcolor( 0 );
   for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   // Alte Fensterkoordinaten fr die Maus werden wieder hergestellt
   vertikale_grenzen( 0, 317 );
}

void ellipse( int aktuelle_farbe )
{
   int taste = 0,
       x = 200, y,
       x1, y1,
       radiusx = 0, radiusy = 0, aw = 0, ew = 0;
   unsigned int kw[3];
   unsigned char zeichen[2] = { 0, '\000' };

   // Mausgrenzen werden festgelegt
   maus_geh_zu( 20, 20 );
   vertikale_grenzen( 17, 317 );
   // Befehl wird ausgegeben
   setcolor( 15 );
   outtextxy( 90, 320, "Ellipse zeichnen" );
   // Anweisung wird ausgegeben
   setcolor( 14 );
   outtextxy( 10, 335, "Mittelpunkt mit der linken Maustaste festlegen ..." );
   // Etwas Verz”gerung, um unangeneme Effekte zu vermeiden
   delay( 500 );
   while ( taste != 27 ) // Taste auf ESC prfen
   {
      if ( kbhit() == 0 )
      {
	 if ( koordinaten_ausgeben() == 1 )
	 {
	    taste = 27; // Abbruchbedingung erfllt
	    maus_koordinaten_in( kw );
	    x1 = kw[1];  // Mittelpunkt festlegen
	    y1 = kw[2];
	 }
      } else taste = getch();
   }
   x = 200;
   taste ^= taste; // Taste wieder auf Null
   // Die Anweisung wird gel”scht und eine neue wird ausgegeben
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   // Maus aus
   maus( 0 );
   // Jetzt werden die notwendigen Werte eingelesen
   setcolor( 14 );
   outtextxy( 10, 335, "Radius x in Pixel eingeben : -->" );
   while ( taste != 13 ) // Taste auf ENTER prfen
   {
      taste = getch();
      if ( (taste >= 48) && (taste < 58) )
      {
	 radiusx *= 10, radiusx += (taste - 48);
	 zeichen[0] = ( unsigned char ) taste;
	 outtextxy( x += 7, 335, zeichen );
      }
   }
   x = 200;
   taste ^= taste; // Taste auf Null
   // Die Anweisung wird gel”scht und eine neue wird ausgegeben
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   setcolor( 14 );
   outtextxy( 10, 335, "Radius y in Pixel eingeben : -->" );
   while ( taste != 13 ) // Taste auf ENTER prfen
   {
      taste = getch();

      if ( (taste >= 48) && (taste < 58) )
      {
	 radiusy *= 10, radiusy += (taste - 48);
	 zeichen[0] = ( unsigned char ) taste;
	 outtextxy( x += 7, 335, zeichen );
      }
   }
   x = 200;
   taste ^= taste;
   // Die Anweisung wird gel”scht und eine neue wird ausgegeben
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   setcolor( 14 );
   outtextxy( 10, 335, "Startwinkel in Pixel eingeben : -->" );
   while ( taste != 13 ) // Taste auf ENTER prfen
   {
      taste = getch();

      if ( (taste >= 48) && (taste < 58) )
      {
	 aw *= 10, aw += (taste - 48);
	 zeichen[0] = ( unsigned char ) taste;
	 outtextxy( x += 7, 335, zeichen );
      }
   }
   x = 200;
   taste ^= taste;
   // Die Anweisung wird gel”scht und eine neue wird ausgegeben
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   setcolor( 14 );
   outtextxy( 10, 335, "Endwinkel in Pixel eingeben : -->" );
   while ( taste != 13 ) // Taste auf ENTER testen
   {
      taste = getch();

      if ( (taste >= 48) && (taste < 58) )
      {  ew *= 10, ew += (taste - 48);
	 zeichen[0] = ( unsigned char ) taste;
	 outtextxy( x += 7, 335, zeichen );
      }
   }
   // Ellipse wird gezeichnet
   setcolor( aktuelle_farbe );
   ellipse( x1, y1, aw, ew, radiusx, radiusy );
   maus( 1 );
   // Anweisung und Befehl l”schen
   setcolor( 0 );
   for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   maus( 1 );
   vertikale_grenzen( 0, 317 );
}

void kreis( int aktuelle_farbe )
{
   int taste = 0,
       x = 180, y,
       x1, y1,
       radius = 0;
   unsigned int kw[3];
   unsigned char zeichen[2] = { 0, '\000' };


   maus_geh_zu( 20, 20 );
   vertikale_grenzen( 17, 317 );
   // Befehl wird ausgegeben
   setcolor( 15 );
   outtextxy( 90, 320, "Kreis zeichnen" );
   setcolor( 14 );
   outtextxy( 10, 335, "Mittelpunkt mit der linken Maustaste festlegen ..." );
   delay( 500 );
   while ( taste != 27 )
   {
      if ( kbhit() == 0 )
      {
	 if ( koordinaten_ausgeben() == 1 )
	 {
	    taste = 27; // Abbruchbedingung erfllt
	    maus_koordinaten_in( kw );
	    x1 = kw[1];
	    y1 = kw[2];
	 }
      } else taste = getch();
   }
   taste ^= taste;
   setcolor( 0 );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   setcolor( 14 );
   outtextxy( 10, 335, "Radius in Pixel eingeben : -->" );
   maus( 0 );
   while ( taste != 13 )
   {
      taste = getch();

      if ( (taste >= 48) && (taste < 58) )
      {
	 radius *= 10, radius += (taste - 48);
	 zeichen[0] = ( unsigned char ) taste;
	 outtextxy( x += 7, 335, zeichen );
      }
   }
   setcolor( aktuelle_farbe );
   circle( x1, y1, radius );
   maus( 1 );
   vertikale_grenzen( 0, 317 );
   setcolor( 0 );
   for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
}

void punkt( int aktuelle_farbe )
{
   int taste = 0,
       f, y;
   unsigned int kw[3];

   // Befehl wird ausgegeben
   setcolor( 15 );
   outtextxy( 90, 320, "Punkte setzen" );
   setcolor( 14 );
   outtextxy( 10, 335, "Linke Maustaste - Punkt setzen.            ESC - Vorgang abbrechen." );
   // Maus an eine andere Position geschickt
   maus_geh_zu( 20, 20 );
   // Neue Fenstergrenzen fr die Maus werden festgelegt
   vertikale_grenzen( 17, 317 );
   f = aktuelle_farbe;
   delay( 100 );
   while ( taste != 27 ) // Taste auf ESC prfen
   {
      if ( kbhit() == 0 )  // wenn keine Taste gedrckt wurde und
      {                    // die linke Maustaste bet„tigt wird, dann wird
			   // ein Punkt gesetzt
	 if ( koordinaten_ausgeben() == 1 )
	 {
	    maus_koordinaten_in( kw );
	    putpixel( kw[1], kw[2], f );
	 }
      }
      else taste = getch();
   }
   // Befehl wird gel”scht
   setcolor( 0 );
   for ( y = 320; y <= 333; y++ ) line( 80, y, 330, y );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   vertikale_grenzen( 0, 317 );
}

int farbe( void )
{
   int taste = 0,
       farbe = 0,
       x = 160, y;
   unsigned char zeichen[2] = { 0, '\000' };

   maus( 0 );
   setcolor( 15 );
   outtextxy( 90, 320, "Neue Farbe festlegen" );
   setcolor( 14 );
   outtextxy( 10, 335, "Neue Farbe ( max. 15 ): -->" );
   while ( (taste != 13 ) )
   {
      taste = getch();

      if ( (taste > 48) && (taste < 58) ) farbe *= 10, farbe += (taste - 48);

      zeichen[0] = ( unsigned char ) taste;
      outtextxy( x += 7, 335, zeichen );
   }
   if ( farbe > 15 ) farbe = 15;
   // Befehl & untere Leiste werden gereinigt
   setcolor( 0 );
   for ( y = 320; y < 333; y++ ) line( 80, y, 300, y );
   for ( y = 335; y < 349; y++ ) line( 10, y, 630, y );
   maus( 1 );
   return( farbe );
}


const int x[9] = { 10, 80, 150, 220, 290, 360, 430, 500, 570 },
	  y = 1;

void main( void )
{

   const int tf = 15,
	     fr = 2;
   int gr = 9,  // VGA
       md = 1,
       xkoo, ykoo,
       aktuelle_farbe = 15,
       n;
   unsigned int werte[3],
		beenden = 0;


   initgraph( &gr, &md, "" );
   cleardevice();
   setcolor( fr );
      line ( 0, 15, 639, 15 );
      line ( 0, 0, 639, 0 );
      line ( 639, 0, 639, 349 );
      line ( 0, 349, 639, 349 );
      line ( 0, 0, 0, 349 );
      line ( 0, 334, 639, 334 );
      line ( 0, 319, 639, 319 );
      for ( xkoo = 70; xkoo < 630; xkoo += 70 ) line( xkoo, 0, xkoo, 15 );
   setcolor( tf );
   settextstyle( 2, 0, 4);
   for ( n ^= n; n <= 8; n++ ) worte_ausgeben( n );
   outtextxy( 10, 320, "Befehl :" );
   outtextxy( 340, 320, "Koordinaten :   x =           y =" );
   maus( 1 );
   vertikale_grenzen( 0, 317 );
   horizontale_grenzen( 2, 637 );
   while ( beenden == 0 )
   {
      switch (koordinaten_ausgeben() )
      {
	 case 1:
		 if ( grenzen_testen( x[0], x[1], 0, 15 ) == 1 ) speichern();
		 if ( grenzen_testen( x[1], x[2], 0, 15 ) == 1 ) laden();
		 if ( grenzen_testen( x[2], x[3], 0, 15 ) == 1 ) kreis( aktuelle_farbe );
		 if ( grenzen_testen( x[3], x[4], 0, 15 ) == 1 ) ellipse( aktuelle_farbe );
		 if ( grenzen_testen( x[4], x[5], 0, 15 ) == 1 ) linie( aktuelle_farbe );
		 if ( grenzen_testen( x[5], x[6], 0, 15 ) == 1 ) punkt( aktuelle_farbe );
		 if ( grenzen_testen( x[6], x[7], 0, 15 ) == 1 ) spray( aktuelle_farbe );
		 if ( grenzen_testen( x[7], x[8], 0, 15 ) == 1 ) aktuelle_farbe = farbe();
		 if ( grenzen_testen( x[8], 639, 0, 15 ) == 1 ) beenden = 1;
		 break;
	 case 0: auswahl_zeigen();
      }
   }
   maus( 0 );
   closegraph();
}


