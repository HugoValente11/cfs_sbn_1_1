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
*************************************************************************/

/**
 * @file
 *
 * Define Sample App Message IDs
 *
 * \note The Sample App assumes default configuration which uses V1 of message id implementation
 */

#ifndef FUNCTION_1_MSGIDS_H
#define FUNCTION_1_MSGIDS_H

/* V1 Command Message IDs must be 0x18xx */
#define FUNCTION_1_CMD_MID     0x1882
#define FUNCTION_1_SEND_HK_MID 0x1883
/* V1 Telemetry Message IDs must be 0x08xx */
#define FUNCTION_1_HK_TLM_MID 0x0883

#define FUNCTION_1_APP_COMMUNICATION_MID_10		0x010


#endif /* FUNCTION_1_MSGIDS_H */
