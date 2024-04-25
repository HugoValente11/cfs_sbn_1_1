#include "function_1_app_user.h"

void FUNCTION_1_User_Main(void)
{
    FUNCTION_1_User_Send_PI_1();


    status = OS_TaskDelay(2000);
}

void FUNCTION_1_User_Init(void)
{
    // User initialization here

}

void FUNCTION_1_User_Send_PI_1() {
    static int counter = 1;

    // TODO Build structs here
    FUNCTION_1_Data.OUT_p1 = counter;

    // Send message
    FUNCTION_1_Send_PI_1();

    counter++;

    CFE_ES_WriteToSysLog("[FUNCTION_1] Message sent: %ld", FUNCTION_1_Data.OUT_p1);


}



