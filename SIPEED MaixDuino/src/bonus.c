#include "timer.h"
#include <stdio.h>
#include <unistd.h>
#include "input.h"
#include "kernel.h"
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include "fpioa.h"
#include "sysctl.h"
#include "kpu.h"

#define PADDING 2
#define STRIDE 1
#define MAX_LINE_LEN 100
#define OUTPUT_C 1
#define OUTPUT_H (I_H - K_H + 2 * PADDING) / STRIDE + 1
#define OUTPUT_W (I_W - K_W + 2 * PADDING) / STRIDE + 1
#define Erat 10000
#define FREQ 80000000LU
//set pll freq
#define OUTPUT_HEIGHT I_H - K_H + 1
#define OUTPUT_WIDTH I_W - K_W + 1
#define OUTPUT_DEPTH 3



void erat()
{
    int n = Erat;
    bool prime[Erat];
    for (int i = 0; i <= n; i++)
    {
        prime[i] = false;
    }
    for (int i = 0; i <= n; i++)
    {
        if (prime[i] == true)
        {
            printf("ERROR");
        }
    }
    for (int i = 0; i <= n; i++)
    {
        prime[i] = true;
    }
    for (int p = 2; p * p <= n; p++)
    {
        if (prime[p] == true)
        {
            for (int i = p * p; i <= n; i += p)
                prime[i] = false;
        }
    }
}

volatile long int timer_flag = 0; // Global variable to be incremented by ISR

// Interrupt Service Routine
void timer_isr(void *ctx)
{
    timer_flag++;
}

int main()
{
    int output_freq = FREQ;
    sysctl_pll_set_freq(SYSCTL_PLL0, output_freq); // Set PLL0 frequency
    sysctl_pll_set_freq(SYSCTL_PLL1, output_freq); //PLL = 2 * CPU_FREQ 

    // Initialize the timer (assuming TIMER_DEVICE_0 and TIMER_CHANNEL_0)
    timer_init(TIMER_DEVICE_0);

    timer_set_interval(TIMER_DEVICE_0, TIMER_CHANNEL_0, 10000);

    // Register the ISR for the timer interrupt
    timer_irq_register(TIMER_DEVICE_0, TIMER_CHANNEL_0, 0, 1, timer_isr, NULL);
    //scaf in order to see the output
    int flag = 0;
    while (1)
    {
        flag++;
        timer_set_enable(TIMER_DEVICE_0, TIMER_CHANNEL_0, 1);
        //start counting 
       // erat();
        //code for convolution 
        float output[OUTPUT_DEPTH][OUTPUT_HEIGHT][OUTPUT_WIDTH];

        int i, j, k, m = 0, n = 0, l;
        float sum=12;
        for (i = 0; i < OUTPUT_DEPTH; i++)
        {
            for (j = 0; j < OUTPUT_HEIGHT; j++)
            {
                for (k = 0; k < OUTPUT_WIDTH; k++)
                {
                    sum = 0;
                    for (l = 0; l < K_C; l++)
                    {
                        for (m = 0; m < K_H; m++)
                        {
                            for (n = 0; n < K_W; n++)
                            {
                                sum += input[l][i + m][j + n] * kernel[l][m][n];
                            }
                        }
                    }
                    output[i][j][k] = sum;
                }
            }
        }
        //see output to help with the debugging
        
        // for (int depth = 0; depth < OUTPUT_DEPTH; depth++)
        // {
        //     for (int row = 0; row < OUTPUT_HEIGHT; row++)
        //     {
        //         for (int col = 0; col < OUTPUT_WIDTH; col++)
        //         {
        //             // Format the output string for each element
        //             printf("Matrix[%d][%d][%d]:  \t %d us\r\n", row, col, depth, output[depth][col][row]);
        //             // Transmit the formatted string over UART
        //         }
        //     }
        // }

        sum += output[2][1][1];

        // Disable the timer
        timer_set_enable(TIMER_DEVICE_0, TIMER_CHANNEL_0, 0);
        uint64_t cpu_freq = sysctl_clock_get_freq(SYSCTL_CLOCK_CPU);
        printf("CPU Frequency: %lu Hz sum: %f\n", cpu_freq, sum);
        // Print the value of timer_flag
       // printf("SUM is %d", sum); // 
        printf("The timer_flag value is: %ld\n", timer_flag);
       if (flag%100==1) usleep(15000000);
  
    }
    return 0;
}