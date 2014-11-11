#ifndef LCD_HD44780_H
#define LCD_HD44780_H

/*******************
* Definition des ports utilis�s pour utiliser l'afficheur LCD
********************/

/*
#ifdef PROG_32
    #define RW_LCD             BIT_3
    #define RS_LCD             BIT_1
    #define E_LCD              BIT_0
    #define D0_LCD             BIT_0
    #define D1_LCD             BIT_1
    #define D2_LCD             BIT_2
    #define D3_LCD             BIT_3
    #define D4_LCD             BIT_4
    #define D5_LCD             BIT_5
    #define D6_LCD             BIT_6
    #define D7_LCD             BIT_7
#else
*/
    #define RS_LCD             LATBbits.LATB4
    #define RW_LCD             LATBbits.LATB5
    #define E_LCD              LATBbits.LATB8
    
    #define D4_LCD             LATBbits.LATB9
    #define D5_LCD             LATBbits.LATB10
    #define D6_LCD             LATBbits.LATB11
    #define D7_LCD             LATBbits.LATB12
    
    #define TRIS_RS_LCD         TRISBbits.TRISB4
    #define TRIS_RW_LCD     TRISBbits.TRISB5
    #define TRIS_E_LCD         TRISBbits.TRISB8
    
    #define TRIS_D4_LCD     TRISBbits.TRISB9
    #define TRIS_D5_LCD     TRISBbits.TRISB10
    #define TRIS_D6_LCD     TRISBbits.TRISB11
    #define TRIS_D7_LCD        TRISBbits.TRISB12
// #endif
    
/**
* Initialise l'afficheur LCD
* 
*/
void hd44780_initLcd (void);

/**
* Envoie une commande � l'afficheur sur 8bits
*/
void hd44780_sendComLcd (char value);


/**
* Envoie une commande � l'afficheur lors de l'initialisation sur 4bits
*/
void hd44780_sendIniComLcd (char D7, char D6, char D5, char D4);

/**
* Envoie une data � l'afficheur
*/
void hd44780_sendDataLcd (char DataLcd );

/**
* Affiche une chaine de texte
* @param texte � transmettre
*/
void hd44780_writeString ( char *texte );

/**
* Affiche une chaine de texte
* @param texte � transmettre
* @param colonne
* @param ligne
*/
void hd44780_affTexteLcdXY( char *texte, char colonneLcd,char ligneLcd );

/**
* Set the cursor at the specified row and column.
* @param row row index between 1 and 4 = N� de la ligne de 1 � 4
* @param column column index de 1 � 20
*/
void hd44780_setCursorRowAndColumn(char row, char column);

/**
* Hide the cursor.
*/
void hd44780_hideCursor (void);

/**
* Set an underline to see the cursor.
*/
void hd44780_showUnderlineCursor (void);

/**
* Use blink cursor.
*/
void hd44780_setBlinkCursor(void);

/**
* Set the cursor at the top and left of the screen.
*/
void hd44780_setCursorAtHome(void);

/**
* Efface l'�cran
*/
void hd44780_clearScreen ( void );

/**
* Affiche un caractere � la position actuel du curseur et d�cal de 1 le curseur
* @param Caract�re � afficher
*/
void hd44780_writeChar ( char carac );

#endif
