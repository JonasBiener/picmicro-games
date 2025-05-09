/* 
 * File:   GLCD_library.h entspricht im Wesentlichen GLCDnokia.h
 * Author: vschilli, modified ronaldb
 *
 * Created on 27. Juli 2017, 16:30
 */

#ifndef GLCD_LIBRARY_H
#define GLCD_LIBRARY_H

#include "GLCD_config.h"

extern const char charSet8x5[];
extern const char charSet16x7[];



// Hier werden wichtige Register und Konstanten definiert, damit anschaulich darauf zugegriffen werden kann
// Beispiel um einen Punkt im Graphikdisplay zu setzen, muss sowohl die x- als auch y-Koordinate gesendet werden, die aber
// nur in einem bestimmten Bereich liegen dürfen. Dieser ist durch GLCD_MAXCOL und GLCD_MAXROW definiert
// Schauen Sie in der Funktion void GLCD_Clear(void) in der Datei GLCD_library.c
// Zum setzen des Cursors an eine bestimmte Stelle wird die Funktion (Makro) GLCD_SetCursor(row, column) verwendet
// Schauen Sie in diesem Dokument in Zeile 100. Dort werden die x- und y-Werte über die gesetzten Bits DSPL_X/DSPL_Y kodiert 
// (über eine ODER-Maske realisiert: DSPL_Y | row)

#define GLCD_MAXCOL 83
#define GLCD_MAXROW  5

#define CMD         0
#define DTA         1

// intruction set
#define DSPL_NOP    0x00
#define DSPL_FKT    0x20
// basic instruction set (H = 0)
#define DSPL_CTRL   0x08
#define DSPL_Y      0x40
#define DSPL_X      0x80
// extended instruction set (H = 1)
#define DSPL_TEMP   0x04
#define DSPL_BIAS   0x10
#define DSPL_VOP    0x80

#define FKT_ON      0x00
#define FKT_PD      0x04
#define FKT_HORZ    0x00
#define FKT_VERT    0x02
#define FKT_BASC    0x00
#define FKT_EXT     0x01

#define CTRL_BLANK  0x00
#define CTRL_NORML  0x04
#define CTRL_ALLON  0x01
#define CTRL_IVM    0x05

#define GLCD_ERROR   0x00
#define GLCD_OK      0x01

// Prototypen der Funktionen, die in der Datei GLCD_library.c implementiert sind
extern void GLCD_Init(void);
extern void GLCD_ClearRow(unsigned char row);
extern void GLCD_Clear2Row(unsigned char row);
extern void GLCD_Clear(void);
extern void GLCD_Write(unsigned char data, unsigned char dc);
extern void GLCD_WriteChar(unsigned char c);

extern char GLCD_CharOut(unsigned char row, unsigned char column, unsigned char c);
extern void GLCD_TextOut(unsigned char row, unsigned char col, unsigned char *text);
extern void GLCD_ConstTextOut(unsigned char row, unsigned char col, const char *text);

extern char GLCD_Char2Out(unsigned char row, unsigned char column, unsigned char c);
extern void GLCD_Text2Out(unsigned char row, unsigned char col, unsigned char *text);
extern void GLCD_ConstText2Out(unsigned char row, unsigned char col, const char *text);

extern void GLCD_ValueOut_00(unsigned char row, unsigned char col, short value, unsigned char min_dig);
#define GLCD_ValueOut(r,c,v) GLCD_ValueOut_00(r,c,v,1)

extern void GLCD_Value2Out_00(unsigned char row, unsigned char col, short value, unsigned char min_dig);
#define GLCD_Value2Out(r,c,v) GLCD_Value2Out_00(r,c,v,1)

#define GLCD_SetCursor(row, column) GLCD_Write(DSPL_X | column, CMD); GLCD_Write(DSPL_Y | row, CMD);

extern void GLCDputrbm_XY(unsigned char rows, unsigned char columns,
                const char* bm, unsigned char row_top, unsigned char col_left);
// ein easteregg:
#define lol() if(adcresult==0){GLCD_ConstTextOut(2,0,"Nicht auf 0   drehen!!      Geht gar nicht...");}else{if(adcresult>4990){GLCD_ConstTextOut(2,0,"Wer hat gesagtvoll aufdrehen????");}else{if(adcresult>2450 && adcresult<2550 ){GLCD_ConstTextOut(2,0,"...jaaaaa,    bei halber    Spannung      chillen");}else{GLCD_ConstTextOut(2,0,"");}}}

#endif	/* GLCD_LIBRARY_H */
