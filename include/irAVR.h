#ifndef IRAVR_h
#define IRAVR_h

/*IRinit() initializes timer0 to work with the pin you have connected to the IR sensor.
To use it, you need to pass in the address to the DDR and PIN registers you are using and the the pin number being used.
EX: IRinit(&DDRD, &PIND, 7); for PD7
To get a value from the remote, you need to create a decode_results variable. After that, you can use
IRdecode() and pass in the variable's address. The function will return a 0 if it fails to decode a value or 
a 1 if it is successfully decode.
EX: IRdecode(&results)
After decoding, you will have the decoded value in results.value. To be able to get a new value after a
successful or failed decode, you will need to run IRresume().*/
 
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "helper.h"

#define LTOL (1.0 - 25/100.) 
#define UTOL (1.0 + 25/100.) 

#define TICKS_LOW(us) (int) (((us)*LTOL/50))
#define TICKS_HIGH(us) (int) (((us)*UTOL/50 + 1))

int MATCH(int measured, int desired) {return measured >= TICKS_LOW(desired) && measured <= TICKS_HIGH(desired);}
int MATCH_MARK(int measured_ticks, int desired_us) {return MATCH(measured_ticks, (desired_us + 100));}
int MATCH_SPACE(int measured_ticks, int desired_us) {return MATCH(measured_ticks, (desired_us - 100));}


typedef struct {
  uint8_t recvpin;           // pin for IR data from detector
  uint8_t rcvstate;          // state machine
  volatile uint8_t * ddr;
  volatile uint8_t * pin;
  unsigned int timer;     // state timer, counts 50uS ticks.
  unsigned int rawbuf[100]; // raw data
  uint8_t rawlen;         // counter of entries in rawbuf
} 
irparams_t;

// Results returned from the decoder
class decode_results {
public:
    int decode_type; // NEC
    unsigned long value; // Decoded value
    int bits; // Number of bits in decoded value
    volatile unsigned int *rawbuf; // Raw intervals in .5 us ticks
    int rawlen; // Number of records in rawbuf.
};



enum IRsates {STATE_IDLE, STATE_MARK, STATE_SPACE, STATE_STOP};

volatile irparams_t irparams;

void IRinit(volatile uint8_t* ddr, volatile uint8_t* pin, int recvpin)
{
  irparams.recvpin = recvpin;
  irparams.ddr = ddr;
  irparams.pin = pin;

  cli();
    // setup pulse clock timer interrupt
    //Prescale /8 (16M/8 = 0.5 microseconds per tick)
    // Therefore, the timer interval can range from 0.5 to 128 microseconds
    // depending on the reset value (255 to 0)
    TCCR0A = _BV(WGM01); 
    TCCR0B = _BV(CS01); 
    OCR0A = (16000000 * 50 / 1000000) / 8; 
    TCNT0 = 0; 

    //Timer0 Overflow Interrupt Enable
    TIMSK0 = _BV(OCIE0A);

    sei();  // enable interrupts

    // initialize state machine variables
    irparams.rcvstate = STATE_IDLE;
    irparams.rawlen = 0;

    *irparams.ddr &= ~(1<<irparams.recvpin);
}

void IRresume() {
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;
}

long decodeNEC(decode_results *results) {
  long data = 0;
  int offset = 1; // Skip first space
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], 9000)) {
    return 0;
  }
  offset++;
  // Check for repeat
  if (irparams.rawlen == 4 &&
    MATCH_SPACE(results->rawbuf[offset], 2250) &&
    MATCH_MARK(results->rawbuf[offset+1], 560)) {
    results->bits = 0;
    results->value = 0xFFFFFFFF;
    results->decode_type = 1;
    return 1;
  }
  if (irparams.rawlen < 2 * 32 + 4) {
    return 0;
  }
  // Initial space  
  if (!MATCH_SPACE(results->rawbuf[offset], 4500)) {
    return 0;
  }
  offset++;
  for (int i = 0; i < 32; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], 560)) {
      return 0;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], 1600)) {
      data = (data << 1) | 1;
    } 
    else if (MATCH_SPACE(results->rawbuf[offset], 560)) {
      data <<= 1;
    } 
    else {
      return 0;
    }
    offset++;
  }
  // Success
  results->bits = 32;
  results->value = data;
  results->decode_type = 0;
  return 1;
}

int IRdecode(decode_results *results) {
  results->rawbuf = irparams.rawbuf;
  results->rawlen = irparams.rawlen;
  if (irparams.rcvstate != STATE_STOP) {
    return 0;
  }
  if (decodeNEC(results)) {
    return 1;
  }

  // Throw away and start over
  IRresume();
  return 0;
}

ISR(TIMER0_COMPA_vect)
{
  //TIMER_RESET;

  uint8_t irdata = (*(irparams.pin) >> irparams.recvpin) & 0x01;

  irparams.timer++; // One more 50us tick
  if (irparams.rawlen >= 100) {
    // Buffer overflow
    irparams.rcvstate = STATE_STOP;
  }
  switch(irparams.rcvstate) {
  case STATE_IDLE: // In the middle of a gap
    if (irdata == 0) {
      if (irparams.timer < 5000/50) {
        // Not big enough to be a gap.
        irparams.timer = 0;
      } 
      else {
        // gap just ended, record duration and start recording transmission
        irparams.rawlen = 0;
        irparams.rawbuf[irparams.rawlen++] = irparams.timer;
        irparams.timer = 0;
        irparams.rcvstate = STATE_MARK;
      }
    }
    break;
  case STATE_MARK: // timing MARK
    if (irdata == 1) {   // MARK ended, record time
      irparams.rawbuf[irparams.rawlen++] = irparams.timer;
      irparams.timer = 0;
      irparams.rcvstate = STATE_SPACE;
    }
    break;
  case STATE_SPACE: // timing SPACE
    if (irdata == 0) { // SPACE just ended, record it
      irparams.rawbuf[irparams.rawlen++] = irparams.timer;
      irparams.timer = 0;
      irparams.rcvstate = STATE_MARK;
    } 
    else { // SPACE
      if (irparams.timer > 5000/50) {
        // big SPACE, indicates gap between codes
        // Mark current code as ready for processing
        // Switch to STOP
        // Don't reset timer; keep counting space width
        irparams.rcvstate = STATE_STOP;
      } 
    }
    break;
  case STATE_STOP: // waiting, measuring gap
    if (irdata == 0) { // reset gap timer
      irparams.timer = 0;
    }
    break;
  }

}

#endif
