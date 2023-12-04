 /******************************************************************************
 *
 * Module: Os
 *
 * File Name: Os.c
 *
 * Description: Source file for Os Scheduler.
 *
 * Author: michael samir
 ******************************************************************************/

#include "Os.h"
#include "App.h"
#include "Button.h"
#include "Led.h"
#include "Gpt.h"
#include "Dio.h"
#include "portf.h"
#include "App.h"


/* Enable IRQ Interrupts ... This Macro enables IRQ interrupts by clearing the I-bit in the PRIMASK. */
#define Enable_Interrupts()    __asm("CPSIE I")

/* Disable IRQ Interrupts ... This Macro disables IRQ interrupts by setting the I-bit in the PRIMASK. */
#define Disable_Interrupts()   __asm("CPSID I")

/* Global variable store the Os Time */
static uint32 g_Time_Tick_Count = 0;

/* Global variable to indicate the the timer has a new tick */
static uint8 g_New_Time_Tick_Flag = 0;

extern  uint8 btn_count;


/*********************************************************************************************/
void Os_start(void)
{
    /* Global Interrupts Enable */
    Enable_Interrupts();
    
    /* 
     * Set the Call Back function to call Os_NewTimerTick
     * this function will be called every SysTick Interrupt (20ms)
     */
    SysTick_SetCallBack(Os_NewTimerTick);

    /* Start SysTickTimer to generate interrupt every 20ms */
  SysTick_Start(OS_BASE_TIME);

    /* Execute the Init Task */
    Init_Task();

    /* Start the Os Scheduler */
    Os_Scheduler();
}

/*********************************************************************************************/
void Os_NewTimerTick(void)
{
    /* Increment the Os time by OS_BASE_TIME */
    g_Time_Tick_Count   += OS_BASE_TIME;

    /* Set the flag to 1 to indicate that there is a new timer tick */
    g_New_Time_Tick_Flag = 1;
}

/*********************************************************************************************/
void Os_Scheduler(void)
{
    /*turn red led on*/
          Red_state();       
    while(1)
    {
        /*check if 10 ms is finished */
        if(g_New_Time_Tick_Flag == 1)
	{
            /*receive every 10ms*/
            if(g_Time_Tick_Count %10==0){     
            
              /*receive state*/
              Receive_state(); 
          /*check if the 1000ms has passed*/
            if(g_Time_Tick_Count == 1000){
            /*exceute app*/
            App_Task();
            /*reset btn count and timer */
            btn_count=0;
            g_Time_Tick_Count=0;
          
          }
          /*erase flag*/
          g_New_Time_Tick_Flag=0;
          }
        }
    }
}