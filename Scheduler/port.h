/*
 * port.h
 *
 *  Created on: 17.01.2011
 *      Author: Yuriy
 */

#ifndef PORT_H_
#define PORT_H_

#define TIMER1_PREDIVISOR_1024 0x05
#define TIMER1_PREDIVISOR_256 0x04
#define TIMER1_PREDIVISOR_64 0x03
#define TIMER1_PREDIVISOR_8 0x02
#define TIMER1_PREDIVISOR_1 0x01
#define TICK_FREQUENCY 1000

void configureTickTimer (void);
void doIdleTask(void);
#endif /* PORT_H_ */
