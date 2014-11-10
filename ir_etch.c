//-------------------------------------------------------------------------------
//	Name:		C2C Jasper Arneberg
//	Term:		Fall 2014
//	MCU:		MSP430G2553
//	Lecture:	22
//	Date:		21 October 2014
//	Note:		This code was developed from Dr. Coulston's original example of how
//				to combine C and assembly.
//-------------------------------------------------------------------------------
#include <msp430g2553.h>
#include "lab5.h"

int8	newIrPacket = FALSE;
int16	packetData[32];
int8	packetIndex = 0;
int32	irPacket = 0;

int8	x, y, color;

void main() {
	// === Initialize system ================================================
	initMSP430();
	init();
	initNokia();

	color=BLACK;
	clearDisplay(color);
	x=4;
	y=4;
	drawBlock(y,x,color);
	packetIndex=0;
	initMSP430();

	while(1) {

		if (newIrPacket==TRUE) {
			newIrPacket = FALSE;		//reset flag
			_disable_interrupt();		//disable while drawing

			if (irPacket == UP) {
				if (y>=1) y=y-1;
			} else if (irPacket == DOWN) {
				if (y<=6) y=y+1;
			} else if (irPacket == RIGHT) {
				if (x<=10) x=x+1;
			} else if (irPacket == LEFT) {
				if (x>=1) x=x-1;
			} else if (irPacket == ENTER) {
				if (color==WHITE) {		//toggle the color
					color = BLACK;
				} else {
					color = WHITE;
				}
			}

			init();
			initNokia();				//reinitialize nokia before lcd can display
			drawBlock(y,x,color);
			//__delay_cycles(2000000);
			initMSP430();				//prepare for next interrupt
		}
	}
}

// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 							// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 				// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;					// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	P1OUT &= ~(BIT0 | BIT6);			// And turn the LED off

	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}


/**
 * Pin change vector
 */
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;				// The timer is 16-bits

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {						// read the current pin level
		case 0:							// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;

			irPacket <<= 1; 			//shift one bit to the left
			if (pulseDuration > minLogic1Pulse) {
				irPacket += 1;			//logical 1
			}
			packetData[packetIndex++] = pulseDuration;
			TACTL |= MC_0;				//disable timerA
			LOW_2_HIGH; 				// Setup pin interrupt on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;				// time measurements are based at time 0
			HIGH_2_LOW; 				// Setup pin interrupt on positive edge
			TACTL |= MC_1;				//enable timerA
			TACTL |= TAIE;				//enable timer A interrupt
			break;
	} // end switch

	P2IFG &= ~BIT6;						// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR

// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------

#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {
	TACTL &= ~TAIE;							//disable Timer A interrupt
	packetIndex = 0;						//reset packet index
	newIrPacket = TRUE;						//new packet must have arrived
	TACTL |= MC_0;							//disable timerA
	TACTL &= ~TAIFG;
}
