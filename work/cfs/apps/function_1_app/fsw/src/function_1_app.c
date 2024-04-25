/*******************************************************************************
**
**      GSC-18128-1, "Core Flight Executive Version 6.7"
**
**      Copyright (c) 2006-2019 United States Government as represented by
**      the Administrator of the National Aeronautics and Space Administration.
**      All Rights Reserved.
**
**      Licensed under the Apache License, Version 2.0 (the "License");
**      you may not use this file except in compliance with the License.
**      You may obtain a copy of the License at
**
**        http://www.apache.org/licenses/LICENSE-2.0
**
**      Unless required by applicable law or agreed to in writing, software
**      distributed under the License is distributed on an "AS IS" BASIS,
**      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**      See the License for the specific language governing permissions and
**      limitations under the License.
**
** File: function_1.c
**
** Purpose:
**   This file contains the source code for the Sample App.
**
*******************************************************************************/

/*
** Include Files:
*/
#include "function_1_app_version.h"
#include "function_1_app.h"
#include "function_1_app_user.h"
#include "dataview_init.h"


/* The sample_lib module provides the SAMPLE_LIB_Function() prototype */
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/* FUNCTION_1_AppMain() -- Application entry point and main process loop         */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void FUNCTION_1_APP_Main(void)
{
    CFE_SB_Buffer_t *SBBufPtr;

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(FUNCTION_1_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = FUNCTION_1_Init();
    if (status != CFE_SUCCESS)
    {
        FUNCTION_1_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }


    // ---------   Send messages  ------------
    // Initialize message PI_1
    FUNCTION_1_Data.MsgId_PI_1 = FUNCTION_1_APP_COMMUNICATION_MID_10;
    FUNCTION_1_Data.Size_PI_1 = asn1SccMyInteger_REQUIRED_BITS_FOR_ENCODING;

    status = CFE_MSG_Init(&FUNCTION_1_Data.MsgPtr_PI_1, FUNCTION_1_Data.MsgId_PI_1, FUNCTION_1_Data.Size_PI_1);

    if (status != CFE_SUCCESS) {
        FUNCTION_1_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error creating the message, RC = %lu\n", (unsigned long) status);
    }
    // --------- Receive messages ------------
    

    // Main app loop
    while (CFE_ES_RunLoop(&FUNCTION_1_Data.RunStatus) == true)
    {
        // Call user loop
        FUNCTION_1_User_Main();
    }



    // Delete pipe to free resources
    status = CFE_SB_DeletePipe (FUNCTION_1_Data.CommandPipe);

    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error Deleting the pipe, RC = 0x%08lX\n",
                             (unsigned long)status);
    }


    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(FUNCTION_1_PERF_ID);

    FUNCTION_1_Data.RunStatus = CFE_ES_RunStatus_APP_EXIT;

    CFE_ES_ExitApp(FUNCTION_1_Data.RunStatus);

} /* End of FUNCTION_1_APP_Main() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* FUNCTION_1_Init() --  initialization                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 FUNCTION_1_Init(void)
{
    int32 status = CFE_SUCCESS; // Initialize status to success;

    FUNCTION_1_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Initialize app command execution counters
    */
    FUNCTION_1_Data.CmdCounter = 0;
    FUNCTION_1_Data.ErrCounter = 0;

    /*
    ** Initialize app configuration data
    */
    FUNCTION_1_Data.PipeDepth = FUNCTION_1_PIPE_DEPTH;

    (void)strncpy(FUNCTION_1_Data.PipeName, "FUNCTION_1_CMD_PIPE", sizeof(FUNCTION_1_Data.PipeName));
    FUNCTION_1_Data.PipeName[sizeof(FUNCTION_1_Data.PipeName) - 1U] = 0;

    /*
    ** Initialize event filter table...
    */
    FUNCTION_1_Data.EventFilters[0].EventID = FUNCTION_1_STARTUP_INF_EID;
    FUNCTION_1_Data.EventFilters[0].Mask    = 0x0000;
    FUNCTION_1_Data.EventFilters[1].EventID = FUNCTION_1_COMMAND_ERR_EID;
    FUNCTION_1_Data.EventFilters[1].Mask    = 0x0000;
    FUNCTION_1_Data.EventFilters[2].EventID = FUNCTION_1_COMMANDNOP_INF_EID;
    FUNCTION_1_Data.EventFilters[2].Mask    = 0x0000;
    FUNCTION_1_Data.EventFilters[3].EventID = FUNCTION_1_COMMANDRST_INF_EID;
    FUNCTION_1_Data.EventFilters[3].Mask    = 0x0000;
    FUNCTION_1_Data.EventFilters[4].EventID = FUNCTION_1_INVALID_MSGID_ERR_EID;
    FUNCTION_1_Data.EventFilters[4].Mask    = 0x0000;
    FUNCTION_1_Data.EventFilters[5].EventID = FUNCTION_1_LEN_ERR_EID;
    FUNCTION_1_Data.EventFilters[5].Mask    = 0x0000;
    FUNCTION_1_Data.EventFilters[6].EventID = FUNCTION_1_PIPE_ERR_EID;
    FUNCTION_1_Data.EventFilters[6].Mask    = 0x0000;
    

    /* Register the events */
    if (status == CFE_SUCCESS)
    {
        status = CFE_EVS_Register(FUNCTION_1_Data.EventFilters, FUNCTION_1_EVENT_COUNTS, CFE_EVS_EventFilter_BINARY);
        if (status != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("[FUNCTION_1] Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        }
    }

    /*
    ** Initialize housekeeping packet (clear user data area).
    */
    if (status == CFE_SUCCESS)
    {
        status = CFE_MSG_Init(&FUNCTION_1_Data.HkTlm.TlmHeader.Msg, CFE_SB_ValueToMsgId(FUNCTION_1_HK_TLM_MID),
                 sizeof(FUNCTION_1_Data.HkTlm));

        if (status != CFE_SUCCESS)
        {
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error initializing housekeeping packets, RC = 0x%08lX\n", (unsigned long)status);
        }
    }

    /*
    ** Create Software Bus message pipe.
    */
    if (status == CFE_SUCCESS)
    {
        status = CFE_SB_CreatePipe(&FUNCTION_1_Data.CommandPipe, FUNCTION_1_Data.PipeDepth, FUNCTION_1_Data.PipeName);
        if (status != CFE_SUCCESS)
        {
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        }
    }


    /*
    ** Subscribe to Housekeeping request commands
    */
    if (status == CFE_SUCCESS)
    {
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(FUNCTION_1_SEND_HK_MID), FUNCTION_1_Data.CommandPipe);
        if (status != CFE_SUCCESS)
        {
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        }
    }

    /*
    ** Subscribe to ground command packets
    */
    if (status == CFE_SUCCESS)
    {
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(FUNCTION_1_CMD_MID), FUNCTION_1_Data.CommandPipe);
        if (status != CFE_SUCCESS)
        {
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long) status);
        }
    }



    CFE_EVS_SendEvent(FUNCTION_1_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION, "FUNCTION_1 App Initialized.%s",
                      FUNCTION_1_VERSION_STRING);

    /*
    ** Create Software Bus message pipe.
    */
    FUNCTION_1_Data.PipeDepth = FUNCTION_1_PIPE_DEPTH;

    (void)strcpy(FUNCTION_1_Data.PipeName, "FUNCTION_1_COM_PIPE");
    CFE_ES_WriteToSysLog("[FUNCTION_1] COM_PIPE Name = %s", FUNCTION_1_Data.PipeName);

    status = CFE_SB_CreatePipe(&FUNCTION_1_Data.CommandPipe, FUNCTION_1_Data.PipeDepth, FUNCTION_1_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error creating communication pipe,210, FUNCTION_1_app.c, RC = 0x%08lX\n",
                             (unsigned long)status);
    }
    CFE_ES_WriteToSysLog("[FUNCTION_1] Pipe creation successful");

    // Get App Id
    FUNCTION_1_FindAppId();

    // Call user init
    FUNCTION_1_User_Init();

    return status;

} /* End of FUNCTION_1_Init() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  FUNCTION_1_ProcessCommandPacket                                    */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the FUNCTION_1    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void FUNCTION_1_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case FUNCTION_1_CMD_MID:
            FUNCTION_1_ProcessGroundCommand(SBBufPtr);
            break;

        case FUNCTION_1_SEND_HK_MID:
            FUNCTION_1_ReportHousekeeping((CFE_MSG_CommandHeader_t *)SBBufPtr);
            break;

        default:
            CFE_EVS_SendEvent(FUNCTION_1_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "FUNCTION_1: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }

    return;

} /* End FUNCTION_1_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* FUNCTION_1_ProcessGroundCommand() -- FUNCTION_1 ground commands                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void FUNCTION_1_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /*
    ** Process "known" FUNCTION_1 app ground commands
    */
    switch (CommandCode)
    {
        case FUNCTION_1_NOOP_CC:
            if (FUNCTION_1_VerifyCmdLength(&SBBufPtr->Msg, sizeof(FUNCTION_1_NoopCmd_t)))
            {
                FUNCTION_1_Noop((FUNCTION_1_NoopCmd_t *)SBBufPtr);
            }

            break;

        case FUNCTION_1_RESET_COUNTERS_CC:
            if (FUNCTION_1_VerifyCmdLength(&SBBufPtr->Msg, sizeof(FUNCTION_1_ResetCountersCmd_t)))
            {
                FUNCTION_1_ResetCounters((FUNCTION_1_ResetCountersCmd_t *)SBBufPtr);
            }

            break;


        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(FUNCTION_1_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid ground command code: CC = %d", CommandCode);
            break;
    }

    return;

} /* End of FUNCTION_1_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  FUNCTION_1_ReportHousekeeping                                          */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 FUNCTION_1_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg)
{
    (void)Msg;

    /*
    ** Get command execution counters...
    */
    FUNCTION_1_Data.HkTlm.Payload.CommandErrorCounter = FUNCTION_1_Data.ErrCounter;
    FUNCTION_1_Data.HkTlm.Payload.CommandCounter      = FUNCTION_1_Data.CmdCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(&FUNCTION_1_Data.HkTlm.TlmHeader.Msg);
    CFE_SB_TransmitMsg(&FUNCTION_1_Data.HkTlm.TlmHeader.Msg, true);


    return CFE_SUCCESS;

} /* End of FUNCTION_1_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* FUNCTION_1_Noop -- FUNCTION_1 NOOP commands                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 FUNCTION_1_Noop(const FUNCTION_1_NoopCmd_t *Msg)
{
    (void)Msg;

    FUNCTION_1_Data.CmdCounter++;

    CFE_EVS_SendEvent(FUNCTION_1_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION, "FUNCTION_1: NOOP command %s",
                      FUNCTION_1_VERSION);

    return CFE_SUCCESS;

} /* End of FUNCTION_1_Noop */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  FUNCTION_1_ResetCounters                                               */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 FUNCTION_1_ResetCounters(const FUNCTION_1_ResetCountersCmd_t *Msg)
{
    (void)Msg;

    FUNCTION_1_Data.CmdCounter = 0;
    FUNCTION_1_Data.ErrCounter = 0;

    CFE_EVS_SendEvent(FUNCTION_1_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION, "FUNCTION_1: RESET command");

    return CFE_SUCCESS;

} /* End of FUNCTION_1_ResetCounters() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* FUNCTION_1_VerifyCmdLength() -- Verify command packet length                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool FUNCTION_1_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    bool              result       = true;
    size_t            ActualLength = 0;
    CFE_SB_MsgId_t    MsgId        = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t FcnCode      = 0;

    CFE_MSG_GetSize(MsgPtr, &ActualLength);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(MsgPtr, &MsgId);
        CFE_MSG_GetFcnCode(MsgPtr, &FcnCode);

        CFE_EVS_SendEvent(FUNCTION_1_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)FcnCode, (unsigned int)ActualLength,
                          (unsigned int)ExpectedLength);

        result = false;

        FUNCTION_1_Data.ErrCounter++;
    }

    return (result);

} /* End of FUNCTION_1_VerifyCmdLength() */




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FUNCTION_1_Send_PI_1 -- Send message         */
/*                                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FUNCTION_1_Send_PI_1() {
    // Define buffer where stream will be written to
    byte perBuffer[asn1SccMyInteger_REQUIRED_BYTES_FOR_ENCODING];

    // Define bitstream
    BitStream bitstream;

    // Initialize bitstream
    BitStream_Init(&bitstream, perBuffer, asn1SccMyInteger_REQUIRED_BYTES_FOR_ENCODING);

    // Initialize error code
    int errorCode;

    // Encode
    if (!asn1SccMyInteger_Encode(&FUNCTION_1_Data.OUT_p1, &bitstream, &errorCode, TRUE))
    {
        CFE_ES_WriteToSysLog("[FUNCTION_1] Encode failed. Error code %d.\n", errorCode);
    }


    status = CFE_PSP_MemCpy(CFE_SB_GetUserData(&FUNCTION_1_Data.MsgPtr_PI_1), &perBuffer, sizeof(perBuffer));

    if (status != CFE_SUCCESS) {
        FUNCTION_1_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error writing the struct, RC = %lu\n", (unsigned long) status);
    }

    status = CFE_SB_TransmitMsg(&FUNCTION_1_Data.MsgPtr_PI_1, false);

    if (status != CFE_SUCCESS) {
        FUNCTION_1_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        CFE_ES_WriteToSysLog("[FUNCTION_1] Error sending the message, RC = %lu\n", (unsigned long) status);
    } else {
        // CFE_ES_WriteToSysLog("[FUNCTION_1] Message sent successfully");
    }

}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FUNCTION_1_FindAppId -- Get App Id and set it in App data   */
/*                                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FUNCTION_1_FindAppId(void)
{
    /*
    ** Search the ES Application table for the name that matches this app
    */
    for (int i = 0; i < CFE_PLATFORM_ES_MAX_APPLICATIONS; i++ )
    {

        if (!strcmp(CFE_ES_Global.AppTable[i].AppName, FUNCTION_1_APP_NAME))
        {
            CFE_ES_GetAppIDByName(&FUNCTION_1_Data.AppId, CFE_ES_Global.AppTable[i].AppName);
        }

    } /* end for */
}


