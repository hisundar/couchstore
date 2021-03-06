/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#include "view_tests.h"


static view_btree_value_t *test_view_btree_value_decoding(const char *value_bin,
                                                          size_t len)
{
    view_btree_value_t *v = NULL;

    assert(decode_view_btree_value(value_bin, len, &v) == COUCHSTORE_SUCCESS);
    assert(v != NULL);

    assert(v->partition == 10);
    assert(v->num_values == 2);

    assert(v->values[0].size == 4);
    assert(memcmp(v->values[0].buf, "6155", v->values[0].size) == 0);
    assert(v->values[1].size == 4);
    assert(memcmp(v->values[1].buf, "6154", v->values[0].size) == 0);

    return v;
}

static view_id_btree_value_t *test_view_id_btree_value_decoding(const char *id_btree_value_bin,
                                                                size_t len)
{
    view_id_btree_value_t *v = NULL;

    assert(decode_view_id_btree_value(id_btree_value_bin, len, &v) == COUCHSTORE_SUCCESS);
    assert(v != NULL);

    assert(v->partition == 67);
    assert(v->num_view_keys_map == 2);

    assert(v->view_keys_map[0].view_id == 0);
    assert(v->view_keys_map[0].num_keys == 2);
    assert(v->view_keys_map[0].json_keys[0].size == 14);
    assert(memcmp(v->view_keys_map[0].json_keys[0].buf,
                  "[123,\"foobar\"]",
                  v->view_keys_map[0].json_keys[0].size) == 0);
    assert(v->view_keys_map[0].json_keys[1].size == 4);
    assert(memcmp(v->view_keys_map[0].json_keys[1].buf,
                  "-321",
                  v->view_keys_map[0].json_keys[1].size) == 0);

    assert(v->view_keys_map[1].view_id == 1);
    assert(v->view_keys_map[1].num_keys == 1);
    assert(v->view_keys_map[1].json_keys[0].size == 7);
    assert(memcmp(v->view_keys_map[1].json_keys[0].buf,
                  "[5,6,7]",
                  v->view_keys_map[1].json_keys[0].size) == 0);

    return v;
}

static void test_view_btree_value_encoding(const view_btree_value_t *v,
                                           char **buffer,
                                           size_t *size)
{
    couchstore_error_t res;

    res = encode_view_btree_value(v, buffer, size);
    assert(res == COUCHSTORE_SUCCESS);
}


static void test_view_id_btree_value_encoding(const view_id_btree_value_t *v,
                                              char **buffer,
                                              size_t *size)
{
    couchstore_error_t res;

    res = encode_view_id_btree_value(v, buffer, size);
    assert(res == COUCHSTORE_SUCCESS);
}

void test_values()
{
    char value_bin[] = {
        0,10,0,0,4,54,49,53,53,0,0,4,54,49,53,52
    };
    char id_btree_value_bin[] = {
        0,67,0,0,2,0,14,91,49,50,51,44,34,102,111,111,98,97,114,
        34,93,0,4,45,51,50,49,1,0,1,0,7,91,53,44,54,44,55,93
    };
    char *v_bin2 = NULL;
    size_t v_bin2_size = 0;
    char *id_btree_v_bin2 = NULL;
    size_t id_btree_v_bin2_size = 0;
    char *v_bin3 = NULL;
    size_t v_bin3_size = 0;
    char *id_btree_v_bin3 = NULL;
    size_t id_btree_v_bin3_size = 0;

    TPRINT("Decoding a view btree value ...\n");
    view_btree_value_t *v = test_view_btree_value_decoding(value_bin, sizeof(value_bin));

    TPRINT("Decoding a view id btree value ...\n");
    view_id_btree_value_t *id_btree_v;
    id_btree_v = test_view_id_btree_value_decoding(id_btree_value_bin,
                                                   sizeof(id_btree_value_bin));

    TPRINT("Encoding the previously decoded view btree value ...\n");
    test_view_btree_value_encoding(v, &v_bin2, &v_bin2_size);

    assert(v_bin2_size == sizeof(value_bin));
    assert(memcmp(v_bin2, value_bin, v_bin2_size) == 0);

    TPRINT("Encoding the previously decoded view id btree value ...\n");
    test_view_id_btree_value_encoding(id_btree_v, &id_btree_v_bin2, &id_btree_v_bin2_size);

    assert(id_btree_v_bin2_size == sizeof(id_btree_value_bin));
    assert(memcmp(id_btree_v_bin2, id_btree_value_bin, id_btree_v_bin2_size) == 0);

    TPRINT("Decoding the previously encoded view btree value ...\n");
    view_btree_value_t *v2 = test_view_btree_value_decoding(v_bin2, v_bin2_size);

    TPRINT("Decoding the previously encoded view id btree value ...\n");
    view_id_btree_value_t *id_btree_v2;
    id_btree_v2 = test_view_id_btree_value_decoding(id_btree_v_bin2,
                                                    id_btree_v_bin2_size);

    TPRINT("Encoding the previously decoded view btree value ...\n");
    test_view_btree_value_encoding(v2, &v_bin3, &v_bin3_size);

    assert(v_bin3_size == sizeof(value_bin));
    assert(memcmp(v_bin3, value_bin, v_bin3_size) == 0);

    TPRINT("Encoding the previously decoded view id btree value ...\n");
    test_view_id_btree_value_encoding(id_btree_v2, &id_btree_v_bin3, &id_btree_v_bin3_size);

    assert(id_btree_v_bin3_size == sizeof(id_btree_value_bin));
    assert(memcmp(id_btree_v_bin3, id_btree_value_bin, id_btree_v_bin3_size) == 0);

    free_view_btree_value(v);
    free_view_btree_value(v2);
    free(v_bin2);
    free(v_bin3);

    free_view_id_btree_value(id_btree_v);
    free_view_id_btree_value(id_btree_v2);
    free(id_btree_v_bin2);
    free(id_btree_v_bin3);
}
