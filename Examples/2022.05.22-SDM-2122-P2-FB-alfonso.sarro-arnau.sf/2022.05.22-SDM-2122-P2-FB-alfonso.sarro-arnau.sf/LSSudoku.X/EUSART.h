/* 
 * File:   EUSART.h
 * Author: alfon
 *
 * Created on 12 May 2022, 15:41
 */

#ifndef EUSART_H
#define	EUSART_H

void initEUSART(void);

void motorEUSART(void);

char EUIsAvailable(void);

void EUSendChar(char vit);

#endif	/* EUSART_H */

