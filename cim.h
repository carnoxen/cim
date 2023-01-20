/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * cim.h
 * This file is part of CIM.
 *
 * Copyright (C) 2023 by Hodong Kim <hodong@nimfsoft.art>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.

 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef __CIM_H__
#define __CIM_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

enum _CimEventType {
  CIM_EVENT_KEY_PRESS   = 0,
  CIM_EVENT_KEY_RELEASE = 1
};
typedef enum _CimEventType CimEventType;

typedef struct _CimEvent CimEvent;
struct _CimEvent {
  CimEventType type;
  uint32_t     state;
  uint32_t     keyval;
  uint32_t     keycode;
};

enum _CimPreeditAttrType
{
  CIM_PREEDIT_ATTR_UNDERLINE,
  CIM_PREEDIT_ATTR_HIGHLIGHT
};
typedef enum _CimPreeditAttrType CimPreeditAttrType;

typedef struct _CimPreeditAttr CimPreeditAttr;
struct _CimPreeditAttr {
  CimPreeditAttrType type;
  int start_index; /* in characters */
  int end_index; /* in characters. The character at this index is not included */
};

typedef struct _CimPreedit CimPreedit;
struct _CimPreedit {
  char* text;
  CimPreeditAttr* attrs;
  int attrs_len;
  int cursor_pos;
};

typedef struct _CimSurround CimSurround;
struct _CimSurround {
  char* text;
  int   len;
  int   cursor_pos; /* cursor position in characters */
  int   anchor_pos; /* anchor position in characters */
};

enum _CimItemType {
  CIM_ITEM_STR,
  CIM_ITEM_STRV,
  CIM_ITEM_N_TYPES,
};
typedef enum _CimItemType CimItemType;

typedef struct _CimItem CimItem;
struct _CimItem {
  CimItemType type;
  void* data;
};

typedef struct _CimCandidate CimCandidate;
struct _CimCandidate {
  int      page_index;
  int      n_pages;
  CimItem* items;
  int      n_items;
};

typedef struct _CimRect CimRect;
struct _CimRect {
  int x;
  int y;
  int width;
  int height;
};

enum _CimCbType {
  CIM_CB_PREEDIT_START,
  CIM_CB_PREEDIT_END,
  CIM_CB_PREEDIT_CHANGED,
  CIM_CB_COMMIT,
  CIM_CB_GET_SURROUND,
  CIM_CB_DELETE_SURROUND,
  CIM_CB_CANDIDATE_START,
  CIM_CB_CANDIDATE_END,
  CIM_CB_CANDIDATE_CHANGED,
  CIM_CB_N_TYPES
};
typedef enum _CimCbType CimCbType;

typedef struct _CimIc CimIc;
typedef struct _CimCallbacks CimCallbacks;
struct _CimCallbacks {
  void (*preedit_start)     (CimIc* ic, void* user_data);
  void (*preedit_end)       (CimIc* ic, void* user_data);
  void (*preedit_changed)   (CimIc* ic,
                             const CimPreedit* preedit,
                             void* user_data);
  void (*commit)            (CimIc* ic,
                             const char* text,
                             void* user_data);
  const CimSurround* (*get_surround) (CimIc* ic, void* user_data);
  bool (*delete_surround)   (CimIc* ic,
                             int    offset,
                             int    n_chars,
                             void*  user_data);
  void (*candidate_start)   (CimIc* ic, void* user_data);
  void (*candidate_end)     (CimIc* ic, void* user_data);
  void (*candidate_changed) (CimIc* ic,
                             const CimCandidate* candidate,
                             void* user_data);
};

struct _CimIc {
  void (*focus_in)       (CimIc* ic);
  void (*focus_out)      (CimIc* ic);
  void (*reset)          (CimIc* ic);
  bool (*filter_event)   (CimIc* ic, const CimEvent* event);
  void (*set_cursor_pos) (CimIc* ic, const CimRect*  area);
  const CimPreedit*   (*get_preedit)   (CimIc* ic);
  const CimCandidate* (*get_candidate) (CimIc* ic);
  void (*set_callback)   (CimIc* ic,
                          CimCbType type,
                          void* callback,
                          void* user_data);
};

CimIc* cim_ic_new ();
void   cim_ic_free           (CimIc* ic);
void   cim_ic_focus_in       (CimIc* ic);
void   cim_ic_focus_out      (CimIc* ic);
void   cim_ic_reset          (CimIc* ic);
bool   cim_ic_filter_event   (CimIc* ic, const CimEvent* event);
void   cim_ic_set_cursor_pos (CimIc* ic, const CimRect*  area);
void   cim_ic_set_callback   (CimIc* ic,
                              CimCbType type,
                              void* callback,
                              void* user_data);
const CimPreedit*   cim_ic_get_preedit   (CimIc* ic);
const CimCandidate* cim_ic_get_candidate (CimIc* ic);

#ifdef  __cplusplus
}
#endif

#endif /* __CIM_H__ */
