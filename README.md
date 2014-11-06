ECE382_Lab5
===========

Lab 5: Interrupts -- Remote Control Decoding

C2C Jasper Arneberg  
T5 ECE 382  
Capt Trimble  

##Day 1 Lab

###Timer Counts

The test5.c program was initally tested for the push of the "enter" button. The following data was captured in the time0 and time1 arrays.

######Time0 Array for Enter Button
![alt text](https://github.com/JasperArneberg/ECE382_Lab5/blob/master/time0_array.png?raw=true "Time 0 Array")

######Time1 Array for Enter Button
![alt text](https://github.com/JasperArneberg/ECE382_Lab5/blob/master/time1_array.png?raw=true "Time 1 Array")

Here is the code that sets up Timer A:
```
TAR = 0x0000;				// time measurements are based at time 0
TA0CCR0 = 0xFFFF;			// create a 16mS roll-over period
TACTL &= ~TAIFG;			// clear flag before enabling interrupts = good practice
TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts
```

With a clock speed of 8 MHz, TACCR0 equal to 65,535, and a clock divider of 8, this means that TAR will roll over every 65.536 msec. This should be plenty of time to receive and decode the packet.

Assuming the clock speed is perfectly accurate, each timer count should last 1 microsecond. This is extremely convenient for comparing the signal waveform to the stored TAR counts.

Here is the code for lines 32-38:
```
32		TAR = 0;						// reset timer and
33		while(IR_DECODER_PIN==0);		// wait while IR is logic 0
34		time0[i] = TAR;					// and store timer A
35
36		TAR = 0;						// reset timer and
37		while(IR_DECODER_PIN != 0);		// wait while IR is logic 1
38		time1[i] = TAR;					// and store timer A
```

The following diagram shows where each line of code is executed in relation to the IR detector signal.

######Loop Diagram
![alt text](https://github.com/JasperArneberg/ECE382_Lab5/blob/master/loop_diagram.png?raw=true "Loop diagram")

###IR Data Packets
The signal pin of the IR detector was next hooked up to a logic analyzer. Below is the waveform after the "enter" button was pressed. The signal corresponds with the values in the time0 and time1 arrays shown above.

######Logic Analyzer signal for Enter Button
![alt text](https://github.com/JasperArneberg/ECE382_Lab5/blob/master/ir_packet.png?raw=true "IR packet when enter button is pressed")

The lengths of the signals were measured on the logic analyzer. The cursors were moved to the rising and falling edges of the half-pulses, and then the time difference was recorded.

######Example Measurement of Half-Pulse Duration
![alt text](https://github.com/JasperArneberg/ECE382_Lab5/blob/master/example_measurement.png?raw=true "Example measurement")

The following table compares the duration of the half-pulse versus the number of counts registered in Timer A. The values are close, but this table shows that Timer A is operating at a speed slightly slower than 8 MHz.

######Duration vs. Timer_A Counts
| Pulse                     | Duration (usec) | Timer A counts |
|---------------------------|-----------------|----------------|
| Start logic 0 half-pulse  | 9041            | 8940           |
| Start logic 1 half-pulse  | 4492            | 4398           |
| Data 1 logic 0 half-pulse | 602             | 589            |
| Data 1 logic 1 half-pulse | 1653            | 1631           |
| Data 0 logic 0 half-pulse | 650             | 644            |
| Data 0 logic 1 half-pulse | 484             | 470            |
| Stop logic 0 half-pulse   | 596             | 587            |
| Stop logic 1 half-pulse   | 43,047          | 42,467         |

In order to get a sense of the variance for the half-pulse signals, 8 samples of each type were collected. The mean and standard deviation of these samples were calculated as can be seen in the table below. The lower and upper limits were calculated as 5 standard deviations below and above the mean. Assuming a normal distribution, this means that 99.999943% of all samples will be within the range between the lower and upper limits.

######Mean and Standard Deviation of Half-Pulses
|              | Data 0, logic 0 half-pulse | Data 1, logic 1 half-pulse | Data 0, logic 1 half-pulse |
|--------------|----------------------------|----------------------------|----------------------------|
| Mean         | 610.5                      | 1608.875                   | 496.5                      |
| Standard Dev | 19.324                     | 12.710                     | 15.372                     |
| Lower Limit  | 513.878                    | 1545.323                   | 419.642                    |
| Upper Limit  | 707.122                    | 1672.427                   | 573.358                    |

As expected, pushing different buttons led to the creation of different packets. The packets were determined by observing the time1 array after pressing a button.

######Packets Generated for Different Button Presses
| Button | Packet (Binary)                     | Packet (Hex) |
|--------|-------------------------------------|--------------|
| 0      | 00100100 00011011 11010000 01101111 | 281BD06F     |
| 1      | 00000000 00111111 11000000 11111111 | 003FC0FF     |
| 2      | 00100000 00011111 11000000 01111111 | 201FC07F     |
| 3      | 00010000 00101111 11000000 10111111 | 102FC0BF     |
| Power  | 00111100 00000011 11110000 00001111 | 3C03F00F     |
| VOL +  | 00001100 00110011 11110000 11001111 | 0C33F0CF     |
| VOL -  | 00101100 00010011 11110000 01001111 | 2C13F08F     |
| CH +   | 00010100 00101011 11010000 10101111 | 182BD0AF     |
| CH -   | 01100001 10100000 11010000 00101111 | 61A0D02F     |

##Documentation
I used http://www.tablesgenerator.com/markdown_tables to generate markdown tables efficiently. 
