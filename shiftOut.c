#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h> 
  
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
        ; 
} 

#define LSBFIRST 1

int latchPin = 127;        //CON3-P12 PH2  shift register latch pini
int clockPin = 172;       //CON3-P11 PI19  shift register clock pini
int dataPin = 171;	  //CON3-P13 PI18 shift register data pini


void shiftOut(int dataPin, int clockPin, int bitOrder, int val)
{
    uint8_t i;
    char syscmd[50]; 
    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST){
            int bitval = !!(val & (1 << i));
            strcpy(syscmd  ,"");
            sprintf(syscmd, "gpioctl %d %d", dataPin, bitval);
            system(syscmd);
            }
        else{
            int bitval1 = !!(val & (1 << (7 - i)));
            strcpy(syscmd  ,"");
            sprintf(syscmd, "gpioctl %d %d", dataPin, bitval1);
            system(syscmd);    
            }
        
        strcpy(syscmd  ,"");
        sprintf(syscmd, "gpioctl %d %d", clockPin, 1);
        system(syscmd);

        strcpy(syscmd  ,"");
        sprintf(syscmd, "gpioctl %d %d", clockPin, 0);
        system(syscmd);      
    }
}

void registerWrite(int value){
    char syscmd[50]; 
    strcpy(syscmd  ,"");
    sprintf(syscmd, "gpioctl %d %d", latchPin, 0);
    system(syscmd);

    shiftOut(dataPin , clockPin, LSBFIRST,  value);

    strcpy(syscmd  ,"");
    sprintf(syscmd, "gpioctl %d %d", latchPin, 1);
    system(syscmd);
}


int main(int argc, char **argv){
    //for(int i = 0; i < 255; i++)
    //{
        registerWrite(atoi(argv[1]));
    //}
    
    
}

