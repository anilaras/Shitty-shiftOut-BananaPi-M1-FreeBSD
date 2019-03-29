//
// Created by Anıl ARAS on 2019-03-29.
//

#include <stdint.h>
#include <string.h>
#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");
#include <fcntl.h>
#include <getopt.h>
#include <paths.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <libgpio.h>

#define LSBFIRST 1

int latchPin = 127;        //CON3-P12 PH2  shift register latch pini
int clockPin = 172;       //CON3-P11 PI19  shift register clock pini
int dataPin = 171;	  //CON3-P13 PI18 shift register data pini


void settings(){
    gpio_config_t pin;
    gpio_handle_t handle;
    handle = gpio_open(0);

    gpio_pin_output(handle, latchPin); //(handle, 127);
    gpio_pin_output(handle, clockPin); //(handle, 172);
    gpio_pin_output(handle, dataPin); //(handle, 171);

    gpio_close(handle);
}


void shiftOut(int dataPin, int clockPin, int bitOrder, int val)
{
    gpio_handle_t handle;
    handle = gpio_open(0);
    uint8_t i;
    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST){
            int bitval = !!(val & (1 << i));
            gpio_pin_set(handle,dataPin, bitval);
        }
        else{
            int bitval1 = !!(val & (1 << (7 - i)));
            gpio_pin_set(handle,dataPin, bitval1);

        }
        gpio_pin_set(handle, clockPin,1);
        gpio_pin_set(handle,clockPin,0);
    }

    gpio_close(handle);

}

void registerWrite(int value){
    gpio_handle_t handle;
    handle = gpio_open(0);

    gpio_pin_set(handle,latchPin,1);
    shiftOut(dataPin , clockPin, LSBFIRST,  value);

    gpio_pin_set(handle,latchPin,0);
    gpio_close(handle);

}

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf("\n terminated \n");
    fflush(stdout);
    for (int i = 0; i < 2; i++) {
        registerWrite(0);
    }
    exit(1);
}

int main(int argc, char **argv){
    signal(SIGINT, sigintHandler);
    for (int c = 0; c < 256; c++) {
        registerWrite(c);
        usleep(50000);
    }
    usleep(50000);
    for (int i = 0; i < 2; i++) {
        registerWrite(0);
    }

    exit(0);
}
