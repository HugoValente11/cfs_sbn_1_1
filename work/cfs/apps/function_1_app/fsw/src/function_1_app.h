/**
 * @file
 *
 * Main header file for the function_1 application
 */

#ifndef FUNCTION_1_H
#define FUNCTION_1_H

/*
** Required header files.
*/
#include "cfe.h"
#include "cfe_error.h"
#include "cfe_evs.h"
#include "cfe_sb.h"
#include "cfe_es.h"
#include "cfe_es_module_all.h"
#include "dataview-uniq.h"

#include "function_1_app_perfids.h"
#include "function_1_app_msgids.h"
#include "function_1_app_msg.h"
#include "function_1_app_events.h"


/***********************************************************************/
#define FUNCTION_1_PIPE_DEPTH 32 /* Depth of the Command Pipe for Application */

#define FUNCTION_1_APP_NAME "FUNCTION_1_APP" /* App Name */



/************************************************************************
** Type Definitions
*************************************************************************/

/*
** Global Data
*/
typedef struct
{
    /*
    ** Command interface counters...
    */
    uint8 CmdCounter;
    uint8 ErrCounter;

    /*
    ** Housekeeping telemetry packet...
    */
    FUNCTION_1_HkTlm_t HkTlm;

    /*
    ** Run Status variable used in the main processing loop
    */
    uint32 RunStatus;

    /*
    ** App Id variable assigned to the app at startup
    */
    uint32 AppId;

    /*
    ** Operational data (not reported in housekeeping)...
    */
    CFE_SB_PipeId_t CommandPipe;

    /*
    ** Initialization data (not reported in housekeeping)...
    */
    char   PipeName[CFE_MISSION_MAX_API_LEN];
    uint16 PipeDepth;

    CFE_MSG_Message_t MsgPtr_PI_1;
    CFE_SB_MsgId_t MsgId_PI_1;
    CFE_MSG_Size_t Size_PI_1;
    // ASN structs
    
    asn1SccMyInteger OUT_p1;


    CFE_EVS_BinFilter_t EventFilters[FUNCTION_1_EVENT_COUNTS];

} FUNCTION_1_Data_t;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (FUNCTION_1_Main), these
**       functions are not called from any other source module.
*/
void  FUNCTION_1_APP_Main(void);
int32 FUNCTION_1_Init(void);
void  FUNCTION_1_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr);
void  FUNCTION_1_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr);
int32 FUNCTION_1_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg);
int32 FUNCTION_1_ResetCounters(const FUNCTION_1_ResetCountersCmd_t *Msg);
int32 FUNCTION_1_Noop(const FUNCTION_1_NoopCmd_t *Msg);
void  FUNCTION_1_FindAppId(void);


bool FUNCTION_1_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength);

void FUNCTION_1_Send_PI_1(void);




#endif /* FUNCTION_1_H */
