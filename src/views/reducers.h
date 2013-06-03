/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

/**
 * @copyright 2013 Couchbase, Inc.
 *
 * @author Filipe Manana  <filipe@couchbase.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 **/

#ifndef _VIEW_REDUCERS_H
#define _VIEW_REDUCERS_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        VIEW_REDUCER_SUCCESS,
        VIEW_REDUCER_ERROR_NOT_A_NUMBER,
        VIEW_REDUCER_ERROR_BAD_STATS_OBJECT
    } view_reducer_error_t;

    typedef struct {
        /* For reduce operations that fail, this is the doc ID of the KV pair
           that caused the failure. For example, for a _sum reducer, if there's
           a value that is not a number, the reducer will abort and populate the
           field 'error_doc_id' with the doc ID of the corresponding document,
           and set 'error' to VIEW_REDUCER_ERROR_NOT_A_NUMBER. */
        const char           *error_doc_id;
        view_reducer_error_t  error;
    } view_reducer_ctx_t;

#ifdef __cplusplus
}
#endif

#endif