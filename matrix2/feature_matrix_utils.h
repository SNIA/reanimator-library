/*
 * Copyright (c) 2011-2012 Santhosh Kumar Koundinya
 * Copyright (c) 2011-2012 Vasily Tarasov
 * Copyright (c) 2011-2012 Erez Zadok
 * Copyright (c) 2011-2012 Geoff Kuenning
 * Copyright (c) 2011-2012 Stony Brook University
 * Copyright (c) 2011-2012 Harvey Mudd College
 * Copyright (c) 2011-2012 The Research Foundation of SUNY
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef FEATURE_MATRIX_UTILS_H_
#define FEATURE_MATRIX_UTILS_H_

#include "feature_matrix.h"

inline void set_errmsg(t2mfm *fmh, const char *errmsg);

inline t2mfm_dll_node *list_add_tail(t2mfm_dll_node *head, void *data);

inline t2mfm_dll_node *list_free_node(t2mfm_dll_node *head, void *data,
        t2mfm_bool deep);

inline size_t list_size(t2mfm_dll_node *head);

inline void list_free(t2mfm_dll_node *head, t2mfm_bool deep_free);

inline void vec_free(t2mfm_vec *vec, t2mfm_bool deep_free);

void get_dim_names(t2mfm *fmh, char *sqlbuf, const size_t sqlbuf_size,
        int *chars_written);

#endif /* FEATURE_MATRIX_UTILS_H_ */