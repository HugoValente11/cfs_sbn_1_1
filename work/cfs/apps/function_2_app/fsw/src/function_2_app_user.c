#include "function_2_app_user.h"

void FUNCTION_2_User_Main(void)
{
    FUNCTION_2_User_Receive_PI_1();


    // status = OS_TaskDelay(period_ms);
}

void FUNCTION_2_User_Init(void)
{
    // User initialization here

}


void FUNCTION_2_User_Receive_PI_1() {
    // Receive message
    FUNCTION_2_Receive_PI_1();

    CFE_ES_WriteToSysLog("[FUNCTION_2] Message received: %ld", FUNCTION_2_Data.IN_p1);

}


