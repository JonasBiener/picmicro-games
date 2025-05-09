//##############################################################################
//  filename:      GLCD_library.c entspricht weitgehend GLCDnokia.c
//            
//  Funktionen zum Ansteuern des Nokia Graphikdisplays
//
//##############################################################################
//
//  Author:               V.SchK/RB
//  Company:              HS-Ulm
//
//  Revision:             1.0
//  Date:                 Mai 2018
//  Assembled using       MPLAB X 4.15, C18 3.47
//
//  todo    - Nokiadisplays stammen aus recycleten alten Handies... ja im Jahre 1999
//          - waren diese Handys der neuste Schrei... :p
//
//##############################################################################

#include "GLCD_library.h"


union v8bit
{
	char all;
	struct
	{
		unsigned LN :4;
		unsigned HN :4;
	};
	struct
	{
		unsigned bit0	:1;
		unsigned bit1	:1;
		unsigned bit2	:1;
		unsigned bit3	:1;
		unsigned bit4	:1;
		unsigned bit5	:1;
		unsigned bit6	:1;
		unsigned bit7	:1;
	};
};
union v8bit spiData;

//##############################################################################
void GLCD_Write(unsigned char data, unsigned char dc)
{
    spiData.all = data; // Zuweisung des übergebenen Bytes data in spiData, um dann über spiData.bitx auf einzelbe Bits zugreifen zu können

    GLCD_DC = dc;
    GLCD_nCS = 0;
    GLCD_CLK = 0; GLCD_DATA = spiData.bit7; GLCD_DLY(); GLCD_CLK = 1; GLCD_DLY();
    GLCD_CLK = 0; GLCD_DATA = spiData.bit6; GLCD_DLY(); GLCD_CLK = 1; GLCD_DLY();
    GLCD_CLK = 0; GLCD_DATA = spiData.bit5; GLCD_DLY(); GLCD_CLK = 1; GLCD_DLY();
    GLCD_CLK = 0; GLCD_DATA = spiData.bit4; GLCD_DLY(); GLCD_CLK = 1; GLCD_DLY();
    GLCD_CLK = 0; GLCD_DATA = spiData.bit3; GLCD_DLY(); GLCD_CLK = 1; GLCD_DLY();
    GLCD_CLK = 0; GLCD_DATA = spiData.bit2; GLCD_DLY(); GLCD_CLK = 1; GLCD_DLY();
    GLCD_CLK = 0; GLCD_DATA = spiData.bit1; GLCD_DLY(); GLCD_CLK = 1; GLCD_DLY();
    GLCD_CLK = 0; GLCD_DATA = spiData.bit0; GLCD_DLY(); GLCD_CLK = 1; GLCD_DLY();
    GLCD_nCS = 1;
}

//##############################################################################
void GLCD_Init(void)
{
    GLCD_DC_TRI = GLCD_nRES_TRI = GLCD_CS_TRI = GLCD_CLK_TRI = GLCE_DATA_TRI = 0;

    GLCD_nRES = 0;
    Nop();Nop();Nop();Nop();Nop();Nop(); // >100ns
    GLCD_nRES = 1;
    
    GLCD_Write(DSPL_FKT | FKT_EXT, CMD);     // Extended instructions enabled
    GLCD_Write(DSPL_VOP | 0x44, CMD);        // Set contrast 0 - 127
    GLCD_Write(DSPL_TEMP | 0x02, CMD);       // Temperature control
    GLCD_Write(DSPL_BIAS | 0x02, CMD);       // Set bias system
    GLCD_Write(DSPL_FKT | FKT_ON | FKT_HORZ | FKT_BASC, CMD);
    GLCD_Write(DSPL_CTRL | CTRL_NORML, CMD); // normal mode
    
    GLCD_Clear();
}

//##############################################################################
void GLCD_ClearRow(unsigned char row)
{
    unsigned char col;
    
    GLCD_SetCursor(row, 0);
    for(col = 0; col <= GLCD_MAXCOL; col++){
        GLCD_Write(0, DTA);
    }
}

//##############################################################################
void GLCD_Clear2Row(unsigned char row)
{
    GLCD_ClearRow(row*2);
    GLCD_ClearRow(row*2 +1);
}

//##############################################################################
void GLCD_Clear(void)
{
    unsigned char row;
    
    for(row = 0; row <= GLCD_MAXROW; row++){
        GLCD_ClearRow(row);
    }
}

//##############################################################################
void GLCD_WriteChar(unsigned char c)
{
    unsigned char i;
    const char* ptrCharBase;
    
    ptrCharBase = charSet8x5;
    ptrCharBase += (unsigned short) 5 * (c);
    
    for (i = 0; i < 5; i++){
        GLCD_Write(*ptrCharBase++, DTA);
    }
    GLCD_Write(0x00, DTA);
}

//##############################################################################
char GLCD_CharOut(unsigned char row, unsigned char col, unsigned char c)
{ 
    unsigned char i;
    const char* ptrCharBase;

    if((row > 5) || (col > 78)) return GLCD_ERROR;

    GLCD_SetCursor(row, col);

    ptrCharBase = charSet8x5;
    ptrCharBase += (unsigned short) 5 * (c);

    for (i = 0; i < 5; i++){
        GLCD_Write(*ptrCharBase++, DTA);
    }
    GLCD_Write(0x00, DTA);
    return GLCD_OK;
}

//##############################################################################
void GLCD_TextOut(unsigned char row, unsigned char col, unsigned char *text)
{
    GLCD_SetCursor(row, col*6);
    while (*text) {
        GLCD_WriteChar(*text);
        text++;
    }
}

//##############################################################################
void GLCD_ConstTextOut(unsigned char row, unsigned char col, const char *text)
{
    GLCD_SetCursor(row, col*6);
    while (*text) {
        GLCD_WriteChar(*text);
        text++;
    }
}

//##############################################################################
 char GLCD_Char2Out(unsigned char row, unsigned char col, unsigned char c)
{ 
    unsigned char i;
    const char* ptrCharBase;

    if((row > 2) || (col > 78)) return GLCD_ERROR;

    GLCD_SetCursor(row*2, col);

    ptrCharBase = charSet16x7;
    ptrCharBase += (unsigned short) 14 * (c);

    for (i = 0; i < 7; i++){
        GLCD_Write(*ptrCharBase++, DTA);
    }
    GLCD_Write(0x00, DTA);
    
    GLCD_SetCursor(row*2 + 1, col);
    
    for (i = 0; i < 7; i++){
        GLCD_Write(*ptrCharBase++, DTA);
    }
    GLCD_Write(0x00, DTA);
    
    return GLCD_OK;
}

//##############################################################################
void GLCD_Text2Out(unsigned char row, unsigned char col, unsigned char *text)
{
    col = 2 + col*8;
    while (*text) {
        GLCD_Char2Out(row,col,*text);
        col += 8;
        text++;
    }
}

//##############################################################################
void GLCD_ConstText2Out(unsigned char row, unsigned char col, const char *text)
{
    col = 2 + col*8;
    while (*text) {
        GLCD_Char2Out(row,col,*text);
        col += 8;
        text++;
    }
}

/* Value Out small*/
void GLCD_ValueOut_00(unsigned char row, unsigned char col, short value,
                     unsigned char min_dig)
{
    char chBuffer, leading_0;

    leading_0 = 1;
    col = 2 + col*6;

    if (value < 0){
        chBuffer = '-';
        GLCD_CharOut(row,col,chBuffer); col += 6;
        value = -value;
    }
    else if(!leading_0 &&(min_dig == 1)){
        chBuffer = ' ';
        GLCD_CharOut(row,col,chBuffer); col += 6;
    }
    chBuffer = value / 10000;
    if(leading_0){
        if((chBuffer != 0)||(min_dig == 5)) leading_0 = 0;
    }
    if(!leading_0){
        GLCD_CharOut(row,col,chBuffer +0x30); col += 6;
    }
    value = value - (10000 * chBuffer);
    chBuffer = value / 1000;
    if(leading_0){
        if((chBuffer != 0)||(min_dig == 4)) leading_0 = 0;
    }
    if(!leading_0){
        GLCD_CharOut(row,col,chBuffer +0x30); col += 6;
    }
    value = value - (1000 * chBuffer);
    chBuffer = value / 100;
    if(leading_0){
        if((chBuffer != 0)||(min_dig == 3)) leading_0 = 0;
    }
    if(!leading_0){
        GLCD_CharOut(row,col,chBuffer +0x30); col += 6;
    }
    value = value - ((short)100 * chBuffer);
    chBuffer = value  / 10;
    if(leading_0){
        if((chBuffer != 0)||(min_dig == 2)) leading_0 = 0;
    }
    if(!leading_0){
        GLCD_CharOut(row,col,chBuffer +0x30); col += 6;
    }
    value = value - ((short)10 * chBuffer);
    chBuffer = value;
    GLCD_CharOut(row,col,chBuffer +0x30); col += 6;
}


/* Value Out*/
void GLCD_Value2Out_00(unsigned char row, unsigned char col, short value,
                     unsigned char min_dig)
{
    char chBuffer, leading_0;

    leading_0 = 1;
    col = 2 + col*8;

    if (value < 0){
        chBuffer = '-';
        GLCD_Char2Out(row,col,chBuffer); col += 8;
        value = -value;
    }
    else if(!leading_0 &&(min_dig == 1)){
        chBuffer = ' ';
        GLCD_Char2Out(row,col,chBuffer); col += 8;
    }
    chBuffer = value / 10000;
    if(leading_0){
        if((chBuffer != 0)||(min_dig == 5)) leading_0 = 0;
    }
    if(!leading_0){
        GLCD_Char2Out(row,col,chBuffer +0x30); col += 8;
    }
    value = value - (10000 * chBuffer);
    chBuffer = value / 1000;
    if(leading_0){
        if((chBuffer != 0)||(min_dig == 4)) leading_0 = 0;
    }
    if(!leading_0){
        GLCD_Char2Out(row,col,chBuffer +0x30); col += 8;
    }
    value = value - (1000 * chBuffer);
    chBuffer = value / 100;
    if(leading_0){
        if((chBuffer != 0)||(min_dig == 3)) leading_0 = 0;
    }
    if(!leading_0){
        GLCD_Char2Out(row,col,chBuffer +0x30); col += 8;
    }
    value = value - ((short)100 * chBuffer);
    chBuffer = value  / 10;
    if(leading_0){
        if((chBuffer != 0)||(min_dig == 2)) leading_0 = 0;
    }
    if(!leading_0){
        GLCD_Char2Out(row,col,chBuffer +0x30); col += 8;
    }
    value = value - ((short)10 * chBuffer);
    chBuffer = value;
    GLCD_Char2Out(row,col,chBuffer +0x30); col += 8;
}

////////////////////////////////////////////////////////////////////////////////
//                           putrsGLCDxy()                                    //
////////////////////////////////////////////////////////////////////////////////
void GLCDputrbm_XY(unsigned char rows, unsigned char columns, const char* bm,
                    unsigned char row_top, unsigned char col_left)
{
    unsigned char r,c;
//    const rom char* base;
    for(r = 0; r < rows; r++){
        GLCD_SetCursor(row_top +r, col_left);
        for (c = 0; c < columns; c++){
            GLCD_Write(*bm++, DTA);
        }
    }
}