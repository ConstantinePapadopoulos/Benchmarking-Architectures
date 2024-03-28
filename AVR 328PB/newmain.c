#include <avr/io.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "input.h"
#include "kernel.h"
#define OUTPUT_HEIGHT I_H-K_H+1
#define OUTPUT_WIDTH I_W-K_W+1
#define OUTPUT_DEPTH 1
#define F_CPU 16000000UL
#define PADDING 2
#define STRIDE 1
#define MAX_LINE_LEN 100
#define Erat 2000
#include <avr/interrupt.h>
#include<util/delay.h>
#include <avr/io.h>
#include <util/delay.h>
volatile int execution_time = 0;

ISR(TIMER1_COMPA_vect) {
    execution_time++;
}

void setup() {
    // Setup timer
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS10); // CTC mode, prescaler 1
    OCR1A = F_CPU / 10000 - 1; // Compare value for 1ms !! be careful to match the cpu freq
    TIMSK1 = (1 << OCIE1A); // Enable interrupt
    sei(); // Enable global interrupts

}

void eratost() {
    int n = Erat;
    bool prime[Erat];
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i <= n; i++) {
            prime[i] = false;
        }
        for (int i = 0; i <= n; i++) {
            if (prime[i] == true) {
                PINC = 0xFF;
            }
        }
        for (int i = 0; i <= n; i++) {
            prime[i] = true;
        }
        for (int p = 2; p * p <= n; p++) {
            if (prime[p] == true) {
                for (int i = p * p; i <= n; i += p)
                    prime[i] = false;
            }
        }

    }
}

int main(void) {

    setup();
    //initialize the counter, 
    //!!WARNING we need to match the CPU frequency correctly 


    while (1) {
        //    _delay_ms(50);
        
        execution_time = 0;// start counting
        //erat(); //maybe pass it as a macro bcause many compilers optimize it
        double output[OUTPUT_DEPTH][OUTPUT_HEIGHT][OUTPUT_WIDTH];

        int i, j, k, m = 0, n = 0, l;
        double sum;
        for (i = 0; i < OUTPUT_DEPTH; i++) {
            for (j = 0; j < OUTPUT_HEIGHT; j++) {
                for (k = 0; k < OUTPUT_WIDTH; k++) {
                    sum = 0;
                    for (l = 0; l < K_C; l++) {
                        for (m = 0; m < K_H; m++) {
                            for (n = 0; n < K_W; n++) {
                                sum += input[l][i + m][j + n] * kernel[l][m][n];
                            }
                        }
                    }
                    output[i][j][k] = sum;
                }
            }
        }
        

        sum += output[0][10][19]; 
        // Your application logic here
        DDRB = 0xFF; //use this as a breakpoint for debugging
          if (execution_time > 10 && sum < 2.15) { //we use this if to overcome the compiler optimizations]
        PORTD = output[0][1][2];
        PORTB = sum ;
       for(int i=0; i<50; i++) PORTC = output[0][12][10]; //also for debbuging
        execution_time = 0;
         }
    }

    return 0;
}

