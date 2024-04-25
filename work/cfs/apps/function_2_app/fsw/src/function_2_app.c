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
** File: function_2.c
**
** Purpose:
**   This file contains the source code for the Sample App.
**
*******************************************************************************/

/*
** Include Files:
*/
#include "function_2_app_version.h"
#include "function_2_app.h"
#include "function_2_app_user.h"
#include "dataview_init.h"


/* The sample_lib module provides the SAMPLE_LIB_Function() prototype */
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/* FUNCTION_2_AppMain() -- Application entry point and main process loop         */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void FUNCTION_2_APP_Main(void)
{

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(FUNCTION_2_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = FUNCTION_2_Init();
    if (status != CFE_SUCCESS)
    {
        FUNCTION_2_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }


    // ---------   Send messages  ------------
    
    // --------- Receive messages ------------
    // Initialize pipe PI_1
    // Subscribe to PI_1     0x010 msg id
    status = CFE_SB_Subscribe(0x010,
                              FUNCTION_2_Data.CommandPipe);

    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("[FUNCTION_2] Error Subscribing to communication pipe, RC = 0x%08lX\n",
                             (unsigned long) status);
    }

    // Main app loop
    while (CFE_ES_RunLoop(&FUNCTION_2_Data.RunStatus) == true)
    {
        // Call user loop
        FUNCTION_2_User_Main();
    }



    // Delete pipe to free resources
    status = CFE_SB_DeletePipe (FUNCTION_2_Data.CommandPipe);

    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("[FUNCTION_2] Error Deleting the pipe, RC = 0x%08lX\n",
                             (unsigned long)status);
    }


    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(FUNCTION_2_PERF_ID);

    FUNCTION_2_Data.RunStatus = CFE_ES_RunStatus_APP_EXIT;

    CFE_ES_ExitApp(FUNCTION_2_Data.RunStatus);

} /* End of FUNCTION_2_APP_Main() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* FUNCTION_2_Init() --  initialization                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 FUNCTION_2_Init(void)
{
    int32 status = CFE_SUCCESS; // Initialize status to success;

    FUNCTION_2_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Initialize app command execution counters
    */
    FUNCTION_2_Data.CmdCounter = 0;
    FUNCTION_2_Data.ErrCounter = 0;

    /*
    ** Initialize app configuration data
    */
    FUNCTION_2_Data.PipeDepth = FUNCTION_2_PIPE_DEPTH;

    (void)strncpy(FUNCTION_2_Data.PipeName, "FUNCTION_2_CMD_PIPE", sizeof(FUNCTION_2_Data.PipeName));
    FUNCTION_2_Data.PipeName[sizeof(FUNCTION_2_Data.PipeName) - 1U] = 0;

    /*
    ** Initialize event filter table...
    */
    FUNCTION_2_Data.EventFilters[0].EventID = FUNCTION_2_STARTUP_INF_EID;
    FUNCTION_2_Data.EventFilters[0].Mask    = 0x0000;
    FUNCTION_2_Data.EventFilters[1].EventID = FUNCTION_2_COMMAND_ERR_EID;
    FUNCTION_2_Data.EventFilters[1].Mask    = 0x0000;
    FUNCTION_2_Data.EventFilters[2].EventID = FUNCTION_2_COMMANDNOP_INF_EID;
    FUNCTION_2_Data.EventFilters[2].Mask    = 0x0000;
    FUNCTION_2_Data.EventFilters[3].EventID = FUNCTION_2_COMMANDRST_INF_EID;
    FUNCTION_2_Data.EventFilters[3].Mask    = 0x0000;
    FUNCTION_2_Data.EventFilters[4].EventID = FUNCTION_2_INVALID_MSGID_ERR_EID;
    FUNCTION_2_Data.EventFilters[4].Mask    = 0x0000;
    FUNCTION_2_Data.EventFilters[5].EventID = FUNCTION_2_LEN_ERR_EID;
    FUNCTION_2_Data.EventFilters[5].Mask    = 0x0000;
    FUNCTION_2_Data.EventFilters[6].EventID = FUNCTION_2_PIPE_ERR_EID;
    FUNCTION_2_Data.EventFilters[6].Mask    = 0x0000;
    

    /* Register the events */
    if (status == CFE_SUCCESS)
    {
        status = CFE_EVS_Register(FUNCTION_2_Data.EventFilters, FUNCTION_2_EVENT_COUNTS, CFE_EVS_EventFilter_BINARY);
        if (status != CFE_SUCCESS)
        {
            CFE_ES_WriteToSysLog("[FUNCTION_2] Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        }
    }

    /*
    ** Initialize housekeeping packet (clear user data area).
    */
    if (status == CFE_SUCCESS)
    {
        status = CFE_MSG_Init(&FUNCTION_2_Data.HkTlm.TlmHeader.Msg, CFE_SB_ValueToMsgId(FUNCTION_2_HK_TLM_MID),
                 sizeof(FUNCTION_2_Data.HkTlm));

        if (status != CFE_SUCCESS)
        {
        CFE_ES_WriteToSysLog("[FUNCTION_2] Error initializing housekeeping packets, RC = 0x%08lX\n", (unsigned long)status);
        }
    }

    /*
    ** Create Software Bus message pipe.
    */
    if (status == CFE_SUCCESS)
    {
        status = CFE_SB_CreatePipe(&FUNCTION_2_Data.CommandPipe, FUNCTION_2_Data.PipeDepth, FUNCTION_2_Data.PipeName);
        if (status != CFE_SUCCESS)
        {
        CFE_ES_WriteToSysLog("[FUNCTION_2] Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        }
    }


    /*
    ** Subscribe to Housekeeping request commands
    */
    if (status == CFE_SUCCESS)
    {
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(FUNCTION_2_SEND_HK_MID), FUNCTION_2_Data.CommandPipe);
        if (status != CFE_SUCCESS)
        {
        CFE_ES_WriteToSysLog("[FUNCTION_2] Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        }
    }

    /*
    ** Subscribe to ground command packets
    */
    if (status == CFE_SUCCESS)
    {
        status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(FUNCTION_2_CMD_MID), FUNCTION_2_Data.CommandPipe);
        if (status != CFE_SUCCESS)
        {
        CFE_ES_WriteToSysLog("[FUNCTION_2] Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long) status);
        }
    }



    CFE_EVS_SendEvent(FUNCTION_2_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION, "FUNCTION_2 App Initialized.%s",
                      FUNCTION_2_VERSION_STRING);

    /*
    ** Create Software Bus message pipe.
    */
    FUNCTION_2_Data.PipeDepth = FUNCTION_2_PIPE_DEPTH;

    (void)strcpy(FUNCTION_2_Data.PipeName, "FUNCTION_2_COM_PIPE");
    CFE_ES_WriteToSysLog("[FUNCTION_2] COM_PIPE Name = %s", FUNCTION_2_Data.PipeName);

    status = CFE_SB_CreatePipe(&FUNCTION_2_Data.CommandPipe, FUNCTION_2_Data.PipeDepth, FUNCTION_2_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("[FUNCTION_2] Error creating communication pipe,210, FUNCTION_2_app.c, RC = 0x%08lX\n",
                             (unsigned long)status);
    }
    CFE_ES_WriteToSysLog("[FUNCTION_2] Pipe creation successful");

    // Get App Id
    FUNCTION_2_FindAppId();

    // Call user init
    FUNCTION_2_User_Init();

    return status;

} /* End of FUNCTION_2_Init() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  FUNCTION_2_ProcessCommandPacket                                    */
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the FUNCTION_2    */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void FUNCTION_2_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case FUNCTION_2_CMD_MID:
            FUNCTION_2_ProcessGroundCommand(SBBufPtr);
            break;

        case FUNCTION_2_SEND_HK_MID:
            FUNCTION_2_ReportHousekeeping((CFE_MSG_CommandHeader_t *)SBBufPtr);
            break;

        default:
            CFE_EVS_SendEvent(FUNCTION_2_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "FUNCTION_2: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }

    return;

} /* End FUNCTION_2_ProcessCommandPacket */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* FUNCTION_2_ProcessGroundCommand() -- FUNCTION_2 ground commands                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void FUNCTION_2_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /*
    ** Process "known" FUNCTION_2 app ground commands
    */
    switch (CommandCode)
    {
        case FUNCTION_2_NOOP_CC:
            if (FUNCTION_2_VerifyCmdLength(&SBBufPtr->Msg, sizeof(FUNCTION_2_NoopCmd_t)))
            {
                FUNCTION_2_Noop((FUNCTION_2_NoopCmd_t *)SBBufPtr);
            }

            break;

        case FUNCTION_2_RESET_COUNTERS_CC:
            if (FUNCTION_2_VerifyCmdLength(&SBBufPtr->Msg, sizeof(FUNCTION_2_ResetCountersCmd_t)))
            {
                FUNCTION_2_ResetCounters((FUNCTION_2_ResetCountersCmd_t *)SBBufPtr);
            }

            break;


        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(FUNCTION_2_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid ground command code: CC = %d", CommandCode);
            break;
    }

    return;

} /* End of FUNCTION_2_ProcessGroundCommand() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  FUNCTION_2_ReportHousekeeping                                          */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 FUNCTION_2_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg)
{
    (void)Msg;

    /*
    ** Get command execution counters...
    */
    FUNCTION_2_Data.HkTlm.Payload.CommandErrorCounter = FUNCTION_2_Data.ErrCounter;
    FUNCTION_2_Data.HkTlm.Payload.CommandCounter      = FUNCTION_2_Data.CmdCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(&FUNCTION_2_Data.HkTlm.TlmHeader.Msg);
    CFE_SB_TransmitMsg(&FUNCTION_2_Data.HkTlm.TlmHeader.Msg, true);


    return CFE_SUCCESS;

} /* End of FUNCTION_2_ReportHousekeeping() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* FUNCTION_2_Noop -- FUNCTION_2 NOOP commands                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 FUNCTION_2_Noop(const FUNCTION_2_NoopCmd_t *Msg)
{
    (void)Msg;

    FUNCTION_2_Data.CmdCounter++;

    CFE_EVS_SendEvent(FUNCTION_2_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION, "FUNCTION_2: NOOP command %s",
                      FUNCTION_2_VERSION);

    return CFE_SUCCESS;

} /* End of FUNCTION_2_Noop */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*  Name:  FUNCTION_2_ResetCounters                                               */
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 FUNCTION_2_ResetCounters(const FUNCTION_2_ResetCountersCmd_t *Msg)
{
    (void)Msg;

    FUNCTION_2_Data.CmdCounter = 0;
    FUNCTION_2_Data.ErrCounter = 0;

    CFE_EVS_SendEvent(FUNCTION_2_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION, "FUNCTION_2: RESET command");

    return CFE_SUCCESS;

} /* End of FUNCTION_2_ResetCounters() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* FUNCTION_2_VerifyCmdLength() -- Verify command packet length                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool FUNCTION_2_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
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

        CFE_EVS_SendEvent(FUNCTION_2_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)FcnCode, (unsigned int)ActualLength,
                          (unsigned int)ExpectedLength);

        result = false;

        FUNCTION_2_Data.ErrCounter++;
    }

    return (result);

} /* End of FUNCTION_2_VerifyCmdLength() */





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FUNCTION_2_Receive_PI_1 -- Receive message   */
/*                                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FUNCTION_2_Receive_PI_1() {
    // CFE_ES_WriteToSysLog("[FUNCTION_2] Try number %i of receiving message", i);
    status = CFE_SB_ReceiveBuffer(&FUNCTION_2_Data.Buffer,
                           FUNCTION_2_Data.CommandPipe,
                           CFE_SB_PEND_FOREVER);

    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("[FUNCTION_2] Error ReceiveBuffer, RC = 0x%08lX\n",
                             (unsigned long)status);
    } else
    {
        // Get buffer data
        byte *data = CFE_SB_GetUserData(&FUNCTION_2_Data.Buffer->Msg);

        // Declare bit stream
        BitStream bitstream;

        // Initialize bitstream
        BitStream_AttachBuffer(&bitstream, data, asn1SccMyInteger_REQUIRED_BITS_FOR_ENCODING);

        // Declare errorCode
        int errorCode;

        // Decode data
        if (!asn1SccMyInteger_Decode(&FUNCTION_2_Data.IN_p1, &bitstream, &errorCode))
        {
            CFE_ES_WriteToSysLog("[FUNCTION_2] Decode failed. Error code: %d", errorCode);
        }




    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* FUNCTION_2_FindAppId -- Get App Id and set it in App data   */
/*                                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void FUNCTION_2_FindAppId(void)
{
    /*
    ** Search the ES Application table for the name that matches this app
    */
    for (int i = 0; i < CFE_PLATFORM_ES_MAX_APPLICATIONS; i++ )
    {

        if (!strcmp(CFE_ES_Global.AppTable[i].AppName, FUNCTION_2_APP_NAME))
        {
            CFE_ES_GetAppIDByName(&FUNCTION_2_Data.AppId, CFE_ES_Global.AppTable[i].AppName);
        }

    } /* end for */
}


