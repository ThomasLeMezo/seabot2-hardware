#ifndef CONFIG_H
#define CONFIG_H

#define CODE_VERSION 0xA0

extern volatile unsigned short is_init;

sbit ILS at PORTA.B2; //  entrée de l'ILS

sbit LED at PORTB.B7; // sortie LED
sbit ALIM at PORTA.B5; // sortie MOSFET de puissance, commande de l'alimentation

void init_io();

#endif