#include "timerISR.h"
#include "helper.h"
#include "periph.h"
#include "irAVR.h"
#include "serialATmega.h"
#include "spiAVR.h"
#include "MAX7219.h"
#include "time.h"
#include"gamesong.h"
void TimerISR(void);

unsigned char systemOn = 0;

unsigned int x = 3;
unsigned int y = 0;
bool isFalling = 1;
bool canRotate = 0;
unsigned int currentShapeRows = 0;
unsigned int currentShapeCols = 0; 
unsigned int shapeNum = 0;
unsigned int shapeRotationNum = 0;
bool playIntro = 0;
bool isGameOver = 0;

unsigned int score = 0;


uint8_t shapeHolder[4][4] 
{
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
};

void selectRandomShape(uint8_t shape[][4], unsigned int &currentShapeCols, unsigned int &currentShapeRows) {
    int shapeIndex = rand()%7;
    //int shapeIndex = 7;
    switch (shapeIndex) {
        case 0: //ㅁ
            shapeHolder[0][0] = 1; shapeHolder[0][1] = 1; shapeHolder[0][2] = 0; shapeHolder[0][3] = 0;
            shapeHolder[1][0] = 1; shapeHolder[1][1] = 1; shapeHolder[1][2] = 0; shapeHolder[1][3] = 0;
            shapeHolder[2][0] = 0; shapeHolder[2][1] = 0; shapeHolder[2][2] = 0; shapeHolder[2][3] = 0;
            shapeHolder[3][0] = 0; shapeHolder[3][1] = 0; shapeHolder[3][2] = 0; shapeHolder[3][3] = 0;

            currentShapeRows = 2;
            currentShapeCols = 2;
            shapeNum = 0;
            break;
        case 1: //L
            shapeHolder[0][0] = 1; shapeHolder[0][1] = 1; shapeHolder[0][2] = 1; shapeHolder[0][3] = 0;
            shapeHolder[1][0] = 0; shapeHolder[1][1] = 0; shapeHolder[1][2] = 1; shapeHolder[1][3] = 0;
            shapeHolder[2][0] = 0; shapeHolder[2][1] = 0; shapeHolder[2][2] = 0; shapeHolder[2][3] = 0;
            shapeHolder[3][0] = 0; shapeHolder[3][1] = 0; shapeHolder[3][2] = 0; shapeHolder[3][3] = 0;
            currentShapeRows = 3;
            currentShapeCols = 2;
            shapeNum = 1;
            break;
        case 2: //reverse L
            shapeHolder[0][0] = 0; shapeHolder[0][1] = 0; shapeHolder[0][2] = 1; shapeHolder[0][3] = 0;
            shapeHolder[1][0] = 1; shapeHolder[1][1] = 1; shapeHolder[1][2] = 1; shapeHolder[1][3] = 0;
            shapeHolder[2][0] = 0; shapeHolder[2][1] = 0; shapeHolder[2][2] = 0; shapeHolder[2][3] = 0;
            shapeHolder[3][0] = 0; shapeHolder[3][1] = 0; shapeHolder[3][2] = 0; shapeHolder[3][3] = 0;
            currentShapeRows = 3;
            currentShapeCols = 2;
            shapeNum = 2;
            break;
        case 3: //ㅗ
            shapeHolder[0][0] = 0; shapeHolder[0][1] = 1; shapeHolder[0][2] = 0; shapeHolder[0][3] = 0;
            shapeHolder[1][0] = 1; shapeHolder[1][1] = 1; shapeHolder[1][2] = 0; shapeHolder[1][3] = 0;
            shapeHolder[2][0] = 0; shapeHolder[2][1] = 1; shapeHolder[2][2] = 0; shapeHolder[2][3] = 0;
            shapeHolder[3][0] = 0; shapeHolder[3][1] = 0; shapeHolder[3][2] = 0; shapeHolder[3][3] = 0;
            currentShapeRows = 2;
            currentShapeCols = 3;
            shapeNum = 3;
            break;

        case 4: // l
            shapeHolder[0][0] = 1; shapeHolder[0][1] = 1; shapeHolder[0][2] = 1; shapeHolder[0][3] = 1;
            shapeHolder[1][0] = 0; shapeHolder[1][1] = 0; shapeHolder[1][2] = 0; shapeHolder[1][3] = 0;
            shapeHolder[2][0] = 0; shapeHolder[2][1] = 0; shapeHolder[2][2] = 0; shapeHolder[2][3] = 0;
            shapeHolder[3][0] = 0; shapeHolder[3][1] = 0; shapeHolder[3][2] = 0; shapeHolder[3][3] = 0;
            currentShapeRows = 4;
            currentShapeCols = 1;
            shapeNum = 4;
            break;
        case 5: //z
            shapeHolder[0][0] = 0; shapeHolder[0][1] = 1; shapeHolder[0][2] = 0; shapeHolder[0][3] = 0;
            shapeHolder[1][0] = 1; shapeHolder[1][1] = 1; shapeHolder[1][2] = 0; shapeHolder[1][3] = 0;
            shapeHolder[2][0] = 1; shapeHolder[2][1] = 0; shapeHolder[2][2] = 0; shapeHolder[2][3] = 0;
            shapeHolder[3][0] = 0; shapeHolder[3][1] = 0; shapeHolder[3][2] = 0; shapeHolder[3][3] = 0;
            currentShapeRows = 2;
            currentShapeCols = 3;
            shapeNum = 5;
            break;

         case 6: // reverse z
            shapeHolder[0][0] = 1; shapeHolder[0][1] = 0; shapeHolder[0][2] = 0; shapeHolder[0][3] = 0;
            shapeHolder[1][0] = 1; shapeHolder[1][1] = 1; shapeHolder[1][2] = 0; shapeHolder[1][3] = 0;
            shapeHolder[2][0] = 0; shapeHolder[2][1] = 1; shapeHolder[2][2] = 0; shapeHolder[2][3] = 0;
            shapeHolder[3][0] = 0; shapeHolder[3][1] = 0; shapeHolder[3][2] = 0; shapeHolder[3][3] = 0;
            currentShapeRows = 2;
            currentShapeCols = 3;
            shapeNum = 6;
            break;
         case 7: // reverse z
            shapeHolder[0][0] = 1; shapeHolder[0][1] = 1; shapeHolder[0][2] = 1; shapeHolder[0][3] = 1;
            shapeHolder[1][0] = 1; shapeHolder[1][1] = 1; shapeHolder[1][2] = 1; shapeHolder[1][3] = 1;
            shapeHolder[2][0] = 1; shapeHolder[2][1] = 1; shapeHolder[2][2] = 1; shapeHolder[2][3] = 1;
            shapeHolder[3][0] = 1; shapeHolder[3][1] = 1; shapeHolder[3][2] = 1; shapeHolder[3][3] = 1;
            currentShapeRows = 4;
            currentShapeCols = 4;
            shapeNum = 7;
            break;
    }
}

decode_results results;

#define NUM_TASKS 5 // TODO: Change to the number of tasks being used
// Task struct for concurrent synchSMs implmentations

typedef struct _task
{
  signed char state;         // Task's current state
  unsigned long period;      // Task period
  unsigned long elapsedTime; // Time elapsed since last task tick
  int (*TickFct)(int);       // Task tick function
} task;

// TODO: Define Periods for each task
const unsigned int buttonl_PERIOD = 100;
const unsigned int buttonr_PERIOD = 100;
const unsigned int buttonRotate_PERIOD = 100;
const unsigned int system_PERIOD = 25;
const unsigned int display_PERIOD = 200;

// e.g. const unsined long TASK1_PERIOD = <PERIOD>
const unsigned long GCD_PERIOD = 5; // TODO:Set the GCD Period
task tasks[NUM_TASKS];              // declared task array with 5 tasks
// TODO: Declare your tasks' function and their states here
enum buttonl_States
{
  buttonl_INIT,
  buttonl_off,
  buttonl_onHold
} buttonl_State;
int TickFct_Buttonl(int state)
{
  static unsigned int cnt = 0;
  switch (state)
  {
  case buttonl_INIT:
    state = buttonl_off;
    break;

  case buttonl_off:
    if (GetBit(PINC, 0))
    {
      state = buttonl_onHold;
      break;
    }
    state = buttonl_off;
    break;

  case buttonl_onHold:
    if (!(GetBit(PINC, 0)))
    {
      state = buttonl_off;
      break;
    }
    state = buttonl_onHold;
    break;

  default:
    state = buttonl_INIT;
    break;
  }
  switch (state)
  {
  case buttonl_INIT:
    break;

  case buttonl_off:
    cnt = 0;
    break;

  case buttonl_onHold:
    cnt++;
    if (cnt % 3 == 0)
    {
      if(x >= 1)
      {
        x--;
      }
    }
    break;

  default:
    break;
  }
  return state;
}

enum buttonr_States
{
  buttonr_INIT,
  buttonr_off,
  buttonr_onHold
} buttonr_State;
int TickFct_Buttonr(int state)
{
  static unsigned int cnt = 0;
  switch (state)
  {
  case buttonr_INIT:
    state = buttonr_off;
    break;

  case buttonr_off:
    if (GetBit(PINC, 1))
    {
      state = buttonr_onHold;
      break;
    }
    state = buttonr_off;
    break;

  case buttonr_onHold:
    if (!(GetBit(PINC, 1)))
    {
      state = buttonr_off;
      break;
    }
    state = buttonr_onHold;
    break;

  default:
    state = buttonr_INIT;
    break;
  }
  switch (state)
  {
  case buttonr_INIT:
    break;

  case buttonr_off:
    cnt = 0;
    break;

  case buttonr_onHold:
    cnt++;
    if (cnt % 3 == 0)
    {
      if(x < 7)
      {
        x++;
      }
    }
    break;

  default:
    break;
  }
  return state;
}

enum buttonRotate_States
{
  buttonRotate_INIT,
  buttonRotate_off,
  buttonRotate_onHold
} buttonRotate_State;

int TickFct_ButtonRotate(int state)
{
  switch (state)
  {
  case buttonRotate_INIT:
    state = buttonRotate_off;
    break;

  case buttonRotate_off:
    if (GetBit(PINC, 2))
    {
      canRotate = 1;
      state = buttonRotate_onHold;
      break;
    }
    state = buttonRotate_off;
    break;

  case buttonRotate_onHold:
    if (!(GetBit(PINC, 2)))
    {
      canRotate = 0;
      state = buttonRotate_off;
      break;
    }
    state = buttonRotate_onHold;
    break;

  default:
    state = buttonRotate_INIT;
    break;
  }
  switch (state)
  {
  case buttonRotate_INIT:
    canRotate = 0;
    break;

  case buttonRotate_off:
    break;

  case buttonRotate_onHold:
    break;

  default:
    break;
  }
  return state;
}

enum system_States
{
  system_INIT,
  system_off,
  system_on,
} system_State;
int TickFct_System(int state)
{
  switch (state)
  {
  case system_INIT:
    state = system_off;
    systemOn = 0;
    break;

  case system_off:
    if (IRdecode(&results))
    {
      if (results.value == 16753245)
      {
        systemOn = 1;
        playIntro = 1;
        
        state = system_on;
        
        IRresume();
        break;
      }
      IRresume();
    }

    playTone(SILENCE);
    state = system_off;
    break;

  case system_on:
    if (IRdecode(&results) || isGameOver || score >=10)
    {
      if (results.value == 16753245 || isGameOver || score >= 10)
      {
            if(score >=10)
            {
              playMelody(winEffectNotes, winEffectDurations, winEffectLength);
            }
            if(isGameOver)
            {
              playMelody(loseEffectNotes, loseEffectDurations, loseEffectLength);
            }
        systemOn = 0;
        isGameOver = 0;
        score = 0;
        playTone(SILENCE);
        state = system_off;
        IRresume();
        break;
      }
      IRresume();
    }
    state = system_on;
    break;

  default:
    state = system_INIT;
    break;
  }
  switch (state)
  {
  case system_INIT:
    break;

  case system_off:
    break;

  case system_on:
    break;

  default:
    break;
  }
  return state;
}


enum game_States
{
    game_INIT,
    game_off,
    game_on
} game_State;

int TickFct_Game(int state)
{
    switch (state)
    {
    case game_INIT:
        initializeMatrix();
        clearMatrix();
        startGameClearBuffer();
        updateMatrix();
        state = game_off;    
        break;

    case game_off:
        if (systemOn)           
        {
            playMelody(maryNotes, maryDurations, maryLength);
            selectRandomShape(shapeHolder,currentShapeCols,currentShapeRows);
            clearBuffer();     
            placeShape(x,y,shapeHolder,currentShapeCols,currentShapeRows);
            updateMatrix();
            state = game_on; 
            break;
        }
        break;

    case game_on:
        if (!systemOn || score >= 10 || isGameOver)       
        {
          serial_println(isGameOver);
            clearMatrix();
            startGameClearBuffer();
            updateMatrix();
            state = game_off; 
            break;
        }
        break;


    default:
        state = game_INIT; 
        break;
    }

    switch(state)
    {
      case game_INIT:
      y = 0;
      x = 3;
      break;

      case game_off:
      y = 0;
      x = 3;
      isGameOver = 0;
      shapeRotationNum = 0;
      score = 0;
      outNum(10);
      break;

      case game_on:

      outNum(score);
      if(!isFalling)
      {
        shapeNum = 0;
        mergeBuffer();
        unsigned int temp = score;
        fullLine(score);
        if(temp != score)
        {
          playMelody(scoreEffectNotes, scoreEffectDurations, scoreEffectLength);
        }
        gameOver(isGameOver);
        selectRandomShape(shapeHolder,currentShapeCols,currentShapeRows);
        x = 3;
        y = 0;
        isFalling = 1;
        break;
      }
      
      if(canRotate)
      {
        rotateShape(shapeHolder, currentShapeCols, currentShapeRows, shapeNum, shapeRotationNum);
        canRotate = 0;
      }
    
      isFalling = canMoveDown(x,y,shapeHolder,currentShapeCols,currentShapeRows);
      fallSquare(x,y,shapeHolder,currentShapeCols,currentShapeRows, isFalling); 
      clearBuffer();
      placeShape(x,y,shapeHolder,currentShapeCols,currentShapeRows); 
      updateMatrix();

      default:
      break;
    }
    

    return state;
}




void TimerISR()
{
  TimerFlag = 1;
}

// TODO: Create your tick functions for each task
int main(void)
{

  srand(time(NULL));
  // TODO: initialize all your inputs and ouputs
  DDRD = 0xFF;
  PORTD = 0x00;

  DDRC = 0b00100000;
  PORTC = 0b11011111;

  DDRB = 0xFF;
  PORTB = 0x00;

  ADC_init();
  serial_init(9600);
  IRinit(&DDRC, &PINC, 4);
  SPI_INIT();
  initializeMatrix();
  clearMatrix();
  
  DDRB |= (1 << PB1);
  TCCR1A |= (1 << WGM11) | (1 << COM1A1); 
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); 
  ICR1 = SILENCE; 
  OCR1A =  ICR1/2; 


  tasks[0].state = buttonl_State;
  tasks[0].period = buttonl_PERIOD;
  tasks[0].elapsedTime = tasks[0].state;
  tasks[0].TickFct = &TickFct_Buttonl;

  tasks[1].state = buttonr_State;
  tasks[1].period = buttonr_PERIOD;
  tasks[1].elapsedTime = tasks[1].state;
  tasks[1].TickFct = &TickFct_Buttonr;

  tasks[2].state = buttonRotate_State;
  tasks[2].period = buttonRotate_PERIOD;
  tasks[2].elapsedTime = tasks[2].state;
  tasks[2].TickFct = &TickFct_ButtonRotate;

  tasks[3].state = system_State;
  tasks[3].period = system_PERIOD;
  tasks[3].elapsedTime = tasks[3].state;
  tasks[3].TickFct = &TickFct_System;

  tasks[4].state = game_State;
  tasks[4].period = display_PERIOD;
  tasks[4].elapsedTime = tasks[4].state;
  tasks[4].TickFct = &TickFct_Game;

  TimerSet(GCD_PERIOD);
  TimerOn();
  while (1)
  {
    if (TimerFlag)
    {
      TimerFlag = 0;
      for (unsigned int i = 0; i < NUM_TASKS; i++)
      { 
        if (tasks[i].elapsedTime == tasks[i].period)
        {                                                    
          tasks[i].state = tasks[i].TickFct(tasks[i].state); 
          tasks[i].elapsedTime = 0;                         
        }
        tasks[i].elapsedTime += GCD_PERIOD; 
      }
    }
  }
  return 0;
}

