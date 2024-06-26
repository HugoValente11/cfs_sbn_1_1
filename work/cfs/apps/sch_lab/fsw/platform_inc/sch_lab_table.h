/************************************************************************
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
** File: sch_lab_table.h
**
** Purpose:
**  This file contains the schedule tables for the SCH Lab app.
**  It is intended to go in the platform include directory so the SCH_LAB
**  app source code does not have to be modified.
**
** Notes:
**
*************************************************************************/
#ifndef sch_lab_sched_tab_h_
#define sch_lab_sched_tab_h_

#include "cfe_sb_extern_typedefs.h" /* for CFE_SB_MsgId_t */
#include "cfe_msg_api_typedefs.h"   /* For CFE_MSG_FcnCode_t */
#include "cfe_msgids.h"

/*
** Defines
*/
#define SCH_LAB_END_OF_TABLE         0
#define SCH_LAB_MAX_SCHEDULE_ENTRIES 32
#define SCH_TBL_DEFAULT_FILE         "/cf/sch_lab_table.tbl"

/*
** Typedefs
*/
typedef struct
{
    CFE_SB_MsgId_t    MessageID;  /* Message ID for the table entry */
    uint32            PacketRate; /* Rate: Send packet every N seconds */
    CFE_MSG_FcnCode_t FcnCode;    /* Command/Function code to set */
} SCH_LAB_ScheduleTableEntry_t;

typedef struct
{
    SCH_LAB_ScheduleTableEntry_t Config[SCH_LAB_MAX_SCHEDULE_ENTRIES];
} SCH_LAB_ScheduleTable_t;

#endif /* sch_lab_sched_tab_h_ */
