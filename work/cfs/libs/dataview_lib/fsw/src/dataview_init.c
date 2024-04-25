/*************************************************************************
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
** File: sample_lib.c
**
** Purpose:
**   Sample CFS library
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "dataview_init.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Library Initialization Routine                                  */
/* cFE requires that a library have an initialization routine      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 DATAVIEW_Init(void)
{
    /*
     * Prints message to screen.
     */

    CFE_ES_WriteToSysLog("[......................]");
    CFE_ES_WriteToSysLog("[DATAVIEW LIB Initialized]");
    CFE_ES_WriteToSysLog("[......................]");

    return CFE_SUCCESS;

} /* End DATAVIEW_Init */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Dataview Lib function                                             */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 DATAVIEW_LIB_Function(void)
{
    CFE_ES_WriteToSysLog("[......................]");
    OS_printf("SAMPLE_LIB_Function called\'\n");
    CFE_ES_WriteToSysLog("[......................]");


    return (CFE_SUCCESS);

} /* End DATAVIEW_LIB_Function */

/************************/
/*  End of File Comment */
/************************/
