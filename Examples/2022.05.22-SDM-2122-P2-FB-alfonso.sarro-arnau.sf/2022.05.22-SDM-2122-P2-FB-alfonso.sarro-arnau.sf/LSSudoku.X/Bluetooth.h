/* 
 * File:   Bluetooth.h
 * Author: alfon
 *
 * Created on 11 May 2022, 21:46
 */

#ifndef BLUETOOTH_H
#define	BLUETOOTH_H

void initBluetooth(void);

void SiSendChar(char my_byte);

char SiIsAvailable(void);

char SiHasReceived(void);

char SiGetChar(void);

#endif	/* BLUETOOTH_H */

