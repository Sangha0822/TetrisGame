#include "timerISR.h"
#include "helper.h"
#include "periph.h"
#include "irAVR.h"
#include "serialATmega.h"
#include "spiAVR.h"
#include "gameSong.h"
#include <stdint.h>
#ifndef MAX7219_h
#define MAX7219_h

#define ROWS 8  //column
#define COLS 32 //row

uint8_t matrixBuffer[ROWS][COLS];
uint8_t fallingBuffer[ROWS][COLS];

void startGameClearBuffer()
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j ++)
        {
            matrixBuffer[i][j] = 0;
            fallingBuffer[i][j] = 0;
        }
    }
}

void clearBuffer()
{
    for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j ++)
        {
            matrixBuffer[i][j] = 0;
        }
    }
}

void mergeBuffer()
{
for(int i = 0; i < ROWS; i++)
    {
        for(int j = 0; j < COLS; j ++)
        {
            if(matrixBuffer[i][j])
            {
                fallingBuffer[i][j]= 1;
                matrixBuffer[i][j] = 0;
            }
        }
    }
}
void updateMatrix() {
    for (int row = 0; row < ROWS; row++) {
        uint8_t data1 = 0, data2 = 0, data3 = 0, data4 = 0;

        for (int col = 0; col < 8; col++) {
            if (matrixBuffer[row][col] || fallingBuffer[row][col]) {
                data1 |= (1 << (col));
            }
        }
        for (int col = 8; col < 16; col++) {
            if (matrixBuffer[row][col] || fallingBuffer[row][col]) {
                data2 |= (1 << (col-8));
            }
        }
        for (int col = 16; col < 24; col++) {
            if (matrixBuffer[row][col] || fallingBuffer[row][col]) {
                data3 |= (1 << (col-16));
            }
        }
        for (int col = 24; col < 32; col++) {
            if (matrixBuffer[row][col] || fallingBuffer[row][col]) {
                data4 |= (1 << (col-24));
            }
        }

        PORTB = SetBit(PORTB, 2, 0);
        SPI_SEND(row + 1); SPI_SEND(data4);
        SPI_SEND(row + 1); SPI_SEND(data3);
        SPI_SEND(row + 1); SPI_SEND(data2);
        SPI_SEND(row + 1); SPI_SEND(data1);
        PORTB = SetBit(PORTB, 2, 1);
    }
}


void placeShape(unsigned int &x, unsigned int &y, const uint8_t shape[4][4], unsigned int cols, unsigned int rows)
{
    for(unsigned int i = 0; i < cols; i ++)
    {
        for(unsigned int j = 0; j < rows; j++)
        {
            if(shape[i][j] == 1)
            {
                if((x + i) < ROWS && (y + j) < COLS)
                {
                    matrixBuffer[x+i][y+j] = 1;
                }
            }
        }
    }
}

bool canMoveDown(unsigned int &x, unsigned int &y, const uint8_t shape[4][4], unsigned int cols, unsigned int rows) {
    for (unsigned int i = 0; i < cols; i++) {
        for (unsigned int j = 0; j < rows; j++) {
            if (shape[i][j] == 1) {
                if (y + j >= COLS - 1) { 
                    return false;
                }
                if (fallingBuffer[x + i][y + j + 1]) { 
                    return false;
                }
            }
        }
    }
    return true;
}


void mergeShape(unsigned int &x, unsigned int &y, const uint8_t shape[4][4], unsigned int cols, unsigned int rows) {
    for (unsigned int i = 0; i < cols; i++) {
        for (unsigned int j = 0; j < rows; j++) {
            if (shape[i][j] == 1) {
                matrixBuffer[x + i][y + j] = 1; 
            }
        }
    }
}

void fallSquare(unsigned int &x, unsigned int &y, const uint8_t shape[4][4], unsigned int cols, unsigned int rows, bool &isFalling) {
if(isFalling)
{
    if(isFalling)
    {
        clearBuffer();                 
        placeShape(x,y,shape,cols,rows);      
        updateMatrix();           

        y++; 
        if (y > COLS - rows) {
            y = 0;
        }
    }
    else
    {
        mergeShape(x,y,shape,cols,rows);
        updateMatrix();
        isFalling = 0;
    }
}
}

void fullLine(unsigned int &score)
{
    for (int row = COLS - 1; row >= 0; row--)
    {
        bool isFull = true;


        for (int col = 0; col < ROWS; col++) {
            if (fallingBuffer[col][row] == 0) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            score ++;
            for (int r = row; r > 0; r--) {
                for (int c = 0; c < ROWS; c++) {
                    fallingBuffer[c][r] = fallingBuffer[c][r - 1];
                }
            }

            for (int c = 0; c < ROWS; c++) {
                fallingBuffer[c][0] = 0;
            }

            row++;
        }
    }
}


void rotateShape(uint8_t shape[4][4], unsigned int &cols, unsigned int &rows, unsigned int shapeNum, unsigned int &shapeRotationNum) {
switch(shapeNum)
{
    case 0:
    if(shapeRotationNum < 3)
    {
        shapeRotationNum++;
    }
    else
    {
        shapeRotationNum = 0;
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = 0;
        }
    }
        switch(shapeRotationNum)
        {
            case 0:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            break;

            case 1:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            break;

            case 2:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            break;

            case 3:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            break;
        }
    break;
    
    case 1:
    if(shapeRotationNum < 3)
    {
        shapeRotationNum++;
    }
    else
    {
        shapeRotationNum = 0;
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = 0;
        }
    }
        switch(shapeRotationNum)
        {
            case 0:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[0][2] = 1;
            shape[1][2] = 1;
            rows = 3;
            cols = 2;
            break;

            case 1:
            shape[0][1] = 1;
            shape[1][1] = 1;
            shape[2][1] = 1;
            shape[2][0] = 1;
            rows = 2;
            cols = 3;
            break;

            case 2:
            shape[0][0] = 1;
            shape[1][2] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            rows = 3;
            cols = 2;
            break;

            case 3:
            shape[0][0] = 1;
            shape[1][0] = 1;
            shape[2][0] = 1;
            shape[0][1] = 1;
            rows = 2;
            cols = 3;
            break;
        }
    break;


    case 2:
        if(shapeRotationNum < 3)
    {
        shapeRotationNum++;
    }
    else
    {
        shapeRotationNum = 0;
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = 0;
        }
    }
        switch(shapeRotationNum)
        {
            case 0:
            shape[1][0] = 1;
            shape[1][1] = 1;
            shape[1][2] = 1;
            shape[0][2] = 1;
            rows = 3;
            cols = 2;
            break;

            case 1:
            shape[0][0] = 1;
            shape[1][0] = 1;
            shape[2][0] = 1;
            shape[2][1] = 1;
            rows = 2;
            cols = 3;
            break;

            case 2:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][0] = 1;
            shape[0][2] = 1;
            rows = 3;
            cols = 2;
            break;

            case 3:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[2][1] = 1;
            shape[1][1] = 1;
            rows = 2;
            cols = 3;
            break;
        }
    break;

    case 3:
     if(shapeRotationNum < 3)
    {
        shapeRotationNum++;
    }
    else
    {
        shapeRotationNum = 0;
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = 0;
        }
    }
        switch(shapeRotationNum)
        {
            case 0:
            shape[0][1] = 1;
            shape[1][1] = 1;
            shape[1][0] = 1;
            shape[2][1] = 1;
            rows = 2;
            cols = 3;
            break;

            case 1:
            shape[0][1] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            shape[1][2] = 1;
            rows = 3;
            cols = 2;
            break;

            case 2:
            shape[0][0] = 1;
            shape[1][1] = 1;
            shape[1][0] = 1;
            shape[2][0] = 1;
            rows = 2;
            cols = 3;
            break;

            case 3:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[0][2] = 1;
            shape[1][1] = 1;
            rows = 3;
            cols = 2;
            break;
        }
    break;

    case 4:
         if(shapeRotationNum < 3)
    {
        shapeRotationNum++;
    }
    else
    {
        shapeRotationNum = 0;
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = 0;
        }
    }
        switch(shapeRotationNum)
        {
            case 0:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[0][2] = 1;
            shape[0][3] = 1;
            rows = 4;
            cols = 1;
            break;

            case 1:
            shape[0][0] = 1;
            shape[1][0] = 1;
            shape[2][0] = 1;
            shape[3][0] = 1;
            rows = 1;
            cols = 4;
            break;

            case 2:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[0][2] = 1;
            shape[0][3] = 1;
            rows = 4;
            cols = 1;
            break;

            case 3:
            shape[0][0] = 1;
            shape[1][0] = 1;
            shape[2][0] = 1;
            shape[3][0] = 1;
            rows = 1;
            cols = 4;
            break;
        }
    break;

    case 5:
             if(shapeRotationNum < 3)
    {
        shapeRotationNum++;
    }
    else
    {
        shapeRotationNum = 0;
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = 0;
        }
    }
        switch(shapeRotationNum)
        {
            case 0:
            shape[1][0] = 1;
            shape[2][0] = 1;
            shape[1][1] = 1;
            shape[0][1] = 1;
            rows = 2;
            cols = 3;
            break;

            case 1:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][1] = 1;
            shape[1][2] = 1;
            rows = 3;
            cols = 2;
            break;

            case 2:
            shape[1][0] = 1;
            shape[2][0] = 1;
            shape[1][1] = 1;
            shape[0][1] = 1;
            rows = 2;
            cols = 3;
            break;

            case 3:
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][1] = 1;
            shape[1][2] = 1;
            rows = 3;
            cols = 2;
            break;
        }
    break;

    case 6:
    if(shapeRotationNum < 3)
    {
        shapeRotationNum++;
    }
    else
    {
        shapeRotationNum = 0;
    }
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            shape[i][j] = 0;
        }
    }
        switch(shapeRotationNum)
        {
            case 0:
            shape[1][0] = 1;
            shape[0][0] = 1;
            shape[1][1] = 1;
            shape[2][1] = 1;
            rows = 2;
            cols = 3;
            break;

            case 1:
            shape[1][0] = 1;
            shape[0][1] = 1;
            shape[1][1] = 1;
            shape[0][2] = 1;
            rows = 3;
            cols = 2;
            break;

            case 2:
            shape[1][0] = 1;
            shape[0][0] = 1;
            shape[1][1] = 1;
            shape[2][1] = 1;
            rows = 2;
            cols = 3;
            break;

            case 3:
            shape[1][0] = 1;
            shape[0][1] = 1;
            shape[1][1] = 1;
            shape[0][2] = 1;
            rows = 3;
            cols = 2;
            break;
        }
    break;
}
}

void gameOver(bool &gameDone) {
    for (unsigned int i = 0; i < ROWS; i++) {
        if (fallingBuffer[i][0]) {
            gameDone = 1;
            break;
        }
    }
}



void initializeMatrix()
{
    PORTB = SetBit(PORTB, 2, 0); 
    for(int i = 1; i < 5; i++)
    {
        SPI_SEND(0x09); SPI_SEND(0x00);
    }
    PORTB = SetBit(PORTB, 2, 1);

    PORTB = SetBit(PORTB, 2, 0); 
    for(int i = 1; i < 5; i++)
    {
        SPI_SEND(0x0A); SPI_SEND(0x02);
    }
    PORTB = SetBit(PORTB, 2, 1);

    PORTB = SetBit(PORTB, 2, 0); 
    for(int i = 1; i < 5; i++)
    {
        SPI_SEND(0x0B); SPI_SEND(0x07);
    }
    PORTB = SetBit(PORTB, 2, 1);

    PORTB = SetBit(PORTB, 2, 0); 
    for(int i = 1; i < 5; i++)
    {
        SPI_SEND(0x0C); SPI_SEND(0x01);
    }
    PORTB = SetBit(PORTB, 2, 1);

    PORTB = SetBit(PORTB, 2, 0); 
    for(int i = 1; i < 5; i++)
    {
        SPI_SEND(0x0F); SPI_SEND(0x00);
        
    }
    PORTB = SetBit(PORTB, 2, 1);
}

void clearMatrix()
{
    PORTB = SetBit(PORTB, 2, 0);
    for(int i = 1; i < 9; i ++)
    {
        for(int j = 0; j < 4; j++)
        {
            SPI_SEND(i); SPI_SEND(0x00);
        }
    }
    PORTB = SetBit(PORTB, 2, 1);
}

#endif