/**
 * @file
 *
 * Main header file for the function_2 application
 */

#ifndef FUNCTION_2_H
#define FUNCTION_2_H

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

#include "function_2_app_perfids.h"
#include "function_2_app_msgids.h"
#include "function_2_app_msg.h"
#include "function_2_app_events.h"


/***********************************************************************/
#define FUNCTION_2_PIPE_DEPTH 32 /* Depth of the Command Pipe for Application */

#define FUNCTION_2_APP_NAME "FUNCTION_2_APP" /* App Name */



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
    FUNCTION_2_HkTlm_t HkTlm;

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

    //For the message (line 104):
    CFE_SB_Buffer_t * Buffer;
    
    
    // ASN structs
    asn1SccMyInteger IN_p1;
    


    CFE_EVS_BinFilter_t EventFilters[FUNCTION_2_EVENT_COUNTS];

} FUNCTION_2_Data_t;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (FUNCTION_2_Main), these
**       functions are not called from any other source module.
*/
void  FUNCTION_2_APP_Main(void);
int32 FUNCTION_2_Init(void);
void  FUNCTION_2_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr);
void  FUNCTION_2_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr);
int32 FUNCTION_2_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg);
int32 FUNCTION_2_ResetCounters(const FUNCTION_2_ResetCountersCmd_t *Msg);
int32 FUNCTION_2_Noop(const FUNCTION_2_NoopCmd_t *Msg);
void  FUNCTION_2_FindAppId(void);


bool FUNCTION_2_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength);


void FUNCTION_2_Receive_PI_1(void);



#endif /* FUNCTION_2_H */
