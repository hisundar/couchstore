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

#ifndef COUCHSTORE_VIEW_GROUP_H
#define COUCHSTORE_VIEW_GROUP_H

#include "config.h"
#include <stdio.h>
#include <libcouchstore/couch_db.h>
#include "index_header.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        const char *view_name;
        const char *error_msg;
    } view_error_t;

    typedef struct {
        int           num_reducers;
        const char  **names;
        const char  **reducers;
    } view_btree_info_t;

    typedef struct {
        const char         *filepath;
        uint64_t            header_pos;
        int                 num_btrees;
        view_btree_info_t  *btree_infos;
        tree_file           file;
    } view_group_info_t;


    /* Read a view group definition from an input stream, and write any
       errors to the optional error stream. */
    LIBCOUCHSTORE_API
    view_group_info_t *couchstore_read_view_group_info(FILE *in_stream,
                                                       FILE *error_stream);

    LIBCOUCHSTORE_API
    void couchstore_free_view_group_info(view_group_info_t *info);

    LIBCOUCHSTORE_API
    couchstore_error_t couchstore_build_view_group(view_group_info_t *info,
                                                   const char *id_records_file,
                                                   const char *kv_records_files[],
                                                   const char *dst_file,
                                                   uint64_t *header_pos,
                                                   view_error_t *error_info);

    couchstore_error_t read_view_group_header(const view_group_info_t *info,
                                              index_header_t **header);

    couchstore_error_t write_view_group_header(tree_file *file,
                                               uint64_t *pos,
                                               const index_header_t *header);

#ifdef __cplusplus
}
#endif

#endif
