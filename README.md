ECE382_Lab5
===========

Lab 5: Interrupts -- Remote Control Decoding

C2C Jasper Arneberg  
T5 ECE 382  
Capt Trimble  

##Day 1 Lab

![alt text](https://github.com/JasperArneberg/ECE382_Lab5/blob/master/ir_packet.png?raw=true "IR packet when power button is pressed")

![alt text](https://github.com/JasperArneberg/ECE382_Lab5/blob/master/example_measurement.png?raw=true "Example measurement")



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




##Documentation
I used http://www.tablesgenerator.com/markdown_tables to generate markdown tables efficiently. 
