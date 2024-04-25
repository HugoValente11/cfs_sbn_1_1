/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * \file
 *
 * Provides a set of macros to facilitate generating stub code
 *
 * These macros are primarily used in the code generated by the "generate_stubs.pl" script
 */

#ifndef UTGENSTUB_H
#define UTGENSTUB_H

#include "common_types.h"
#include "utstubs.h"

/**
 * Helper macro to set up a return buffer in a generated stub
 *
 * Registers a buffer that the hook/handler can fill.  This works
 * with any return data type, not just int32.
 *
 * \param FuncName   The name of the function
 * \param ReturnType The type of return value
 */
#define UT_GenStub_SetupReturnBuffer(FuncName, ReturnType) \
    UT_Stub_RegisterReturnType(UT_KEY(FuncName), sizeof(ReturnType))

/**
 * Helper macro to get the return value from the handler
 *
 * Gets the value exported from the hook/handler
 *
 * \param FuncName   The name of the function
 * \param ReturnType The type of return value
 */
#define UT_GenStub_GetReturnValue(FuncName, ReturnType) \
    (*(ReturnType *)UT_Stub_GetReturnValuePtr(UT_KEY(FuncName), sizeof(ReturnType)))

/**
 * Helper macro to add a local parameter to the current context
 *
 * This makes the parameter accessible from hook/handler functions
 *
 * \param FuncName   The name of the function
 * \param ParamType  The type of the parameter
 * \param ParamName  The name of the parameter
 */
#define UT_GenStub_AddParam(FuncName, ParamType, ParamName)                                                           \
    UT_Stub_RegisterContextWithMetaData(UT_KEY(FuncName), #ParamName, UT_STUBCONTEXT_ARG_TYPE_INDIRECT, &(ParamName), \
                                        sizeof(ParamType))

/**
 * Helper macro to execute the actual stub handler
 *
 * Additional arguments are passed through
 *
 * \param FuncName   The name of the function
 * \param Type       The type of handler to execute (Va or Basic)
 */
#define UT_GenStub_Execute(FuncName, Type, ...) UT_Execute##Type##Handler(UT_KEY(FuncName), #FuncName, __VA_ARGS__);

#endif /* UTGENSTUB_H */
