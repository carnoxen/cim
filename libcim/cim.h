/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * cim.h
 * This file is part of Cim.
 *
 * Copyright (C) 2023 Hodong Kim <hodong@nimfsoft.art>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
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

#ifdef __cplusplus
extern "C" {
#endif

enum _CimEventType {
  CIM_EVENT_KEY_PRESS   = 0,
  CIM_EVENT_KEY_RELEASE = 1
};
typedef enum _CimEventType CimEventType;

enum _CimModifierType {
  CIM_SHIFT_MASK    = 1 << 0,  /* Shift */
  CIM_LOCK_MASK     = 1 << 1,  /* Lock */
  CIM_CONTROL_MASK  = 1 << 2,  /* Control */
  CIM_MOD1_MASK     = 1 << 3,  /* Mod1 */
  CIM_MOD2_MASK     = 1 << 4,  /* Mod2 */
  CIM_MOD3_MASK     = 1 << 5,  /* Mod3 */
  CIM_MOD4_MASK     = 1 << 6,  /* Mod4 */
  CIM_MOD5_MASK     = 1 << 7,  /* Mod5 */
  CIM_BUTTON1_MASK  = 1 << 8,  /* Button1 */
  CIM_BUTTON2_MASK  = 1 << 9,  /* Button2 */
  CIM_BUTTON3_MASK  = 1 << 10, /* Button3 */
  CIM_BUTTON4_MASK  = 1 << 11, /* Button4 */
  CIM_BUTTON5_MASK  = 1 << 12, /* Button5 */

  /* virtual modifiers */
  CIM_SUPER_MASK    = 1 << 26, /* Super */
  CIM_HYPER_MASK    = 1 << 27, /* Hyper */
  CIM_META_MASK     = 1 << 28, /* Meta */

  /* Combination of the above MASKs */
  CIM_MODIFIER_MASK = 0x1c001fff
};
typedef enum _CimModifierType CimModifierType;

enum _CimKeySym {
  CIM_KEY_space            = 0x020, /* space */
  CIM_KEY_exclam           = 0x021, /* exclam */
  CIM_KEY_quotedbl         = 0x022, /* quotedbl */
  CIM_KEY_numbersign       = 0x023, /* numbersign */
  CIM_KEY_dollar           = 0x024, /* dollar */
  CIM_KEY_percent          = 0x025, /* percent */
  CIM_KEY_ampersand        = 0x026, /* ampersand */
  CIM_KEY_apostrophe       = 0x027, /* apostrophe */
  CIM_KEY_parenleft        = 0x028, /* parenleft */
  CIM_KEY_parenright       = 0x029, /* parenright */
  CIM_KEY_asterisk         = 0x02a, /* asterisk */
  CIM_KEY_plus             = 0x02b, /* plus */
  CIM_KEY_comma            = 0x02c, /* comma */
  CIM_KEY_minus            = 0x02d, /* minus */
  CIM_KEY_period           = 0x02e, /* period */
  CIM_KEY_slash            = 0x02f, /* slash */
  CIM_KEY_0                = 0x030, /* 0 */
  CIM_KEY_1                = 0x031, /* 1 */
  CIM_KEY_2                = 0x032, /* 2 */
  CIM_KEY_3                = 0x033, /* 3 */
  CIM_KEY_4                = 0x034, /* 4 */
  CIM_KEY_5                = 0x035, /* 5 */
  CIM_KEY_6                = 0x036, /* 6 */
  CIM_KEY_7                = 0x037, /* 7 */
  CIM_KEY_8                = 0x038, /* 8 */
  CIM_KEY_9                = 0x039, /* 9 */
  CIM_KEY_colon            = 0x03a, /* colon */
  CIM_KEY_semicolon        = 0x03b, /* semicolon */
  CIM_KEY_less             = 0x03c, /* less */
  CIM_KEY_equal            = 0x03d, /* equal */
  CIM_KEY_greater          = 0x03e, /* greater */
  CIM_KEY_question         = 0x03f, /* question */
  CIM_KEY_at               = 0x040, /* at */
  CIM_KEY_A                = 0x041, /* A */
  CIM_KEY_B                = 0x042, /* B */
  CIM_KEY_C                = 0x043, /* C */
  CIM_KEY_D                = 0x044, /* D */
  CIM_KEY_E                = 0x045, /* E */
  CIM_KEY_F                = 0x046, /* F */
  CIM_KEY_G                = 0x047, /* G */
  CIM_KEY_H                = 0x048, /* H */
  CIM_KEY_I                = 0x049, /* I */
  CIM_KEY_J                = 0x04a, /* J */
  CIM_KEY_K                = 0x04b, /* K */
  CIM_KEY_L                = 0x04c, /* L */
  CIM_KEY_M                = 0x04d, /* M */
  CIM_KEY_N                = 0x04e, /* N */
  CIM_KEY_O                = 0x04f, /* O */
  CIM_KEY_P                = 0x050, /* P */
  CIM_KEY_Q                = 0x051, /* Q */
  CIM_KEY_R                = 0x052, /* R */
  CIM_KEY_S                = 0x053, /* S */
  CIM_KEY_T                = 0x054, /* T */
  CIM_KEY_U                = 0x055, /* U */
  CIM_KEY_V                = 0x056, /* V */
  CIM_KEY_W                = 0x057, /* W */
  CIM_KEY_X                = 0x058, /* X */
  CIM_KEY_Y                = 0x059, /* Y */
  CIM_KEY_Z                = 0x05a, /* Z */
  CIM_KEY_bracketleft      = 0x05b, /* bracketleft */
  CIM_KEY_backslash        = 0x05c, /* backslash */
  CIM_KEY_bracketright     = 0x05d, /* bracketright */
  CIM_KEY_asciicircum      = 0x05e, /* asciicircum */
  CIM_KEY_underscore       = 0x05f, /* underscore */
  CIM_KEY_grave            = 0x060, /* grave */
  CIM_KEY_a                = 0x061, /* a */
  CIM_KEY_b                = 0x062, /* b */
  CIM_KEY_c                = 0x063, /* c */
  CIM_KEY_d                = 0x064, /* d */
  CIM_KEY_e                = 0x065, /* e */
  CIM_KEY_f                = 0x066, /* f */
  CIM_KEY_g                = 0x067, /* g */
  CIM_KEY_h                = 0x068, /* h */
  CIM_KEY_i                = 0x069, /* i */
  CIM_KEY_j                = 0x06a, /* j */
  CIM_KEY_k                = 0x06b, /* k */
  CIM_KEY_l                = 0x06c, /* l */
  CIM_KEY_m                = 0x06d, /* m */
  CIM_KEY_n                = 0x06e, /* n */
  CIM_KEY_o                = 0x06f, /* o */
  CIM_KEY_p                = 0x070, /* p */
  CIM_KEY_q                = 0x071, /* q */
  CIM_KEY_r                = 0x072, /* r */
  CIM_KEY_s                = 0x073, /* s */
  CIM_KEY_t                = 0x074, /* t */
  CIM_KEY_u                = 0x075, /* u */
  CIM_KEY_v                = 0x076, /* v */
  CIM_KEY_w                = 0x077, /* w */
  CIM_KEY_x                = 0x078, /* x */
  CIM_KEY_y                = 0x079, /* y */
  CIM_KEY_z                = 0x07a, /* z */
  CIM_KEY_braceleft        = 0x07b, /* braceleft */
  CIM_KEY_bar              = 0x07c, /* bar */
  CIM_KEY_braceright       = 0x07d, /* braceright */
  CIM_KEY_asciitilde       = 0x07e, /* asciitilde */

  CIM_KEY_ISO_Level3_Shift = 0xfe03, /* ISO_Level3_Shift */
  CIM_KEY_ISO_Left_Tab     = 0xfe20, /* ISO_Left_Tab */

  CIM_KEY_BackSpace        = 0xff08, /* BackSpace */
  CIM_KEY_Tab              = 0xff09, /* Tab */

  CIM_KEY_Return           = 0xff0d, /* Return */

  CIM_KEY_Pause            = 0xff13, /* Pause */
  CIM_KEY_Scroll_Lock      = 0xff14, /* Scroll_Lock */
  CIM_KEY_Sys_Req          = 0xff15, /* Sys_Req */

  CIM_KEY_Escape           = 0xff1b, /* Escape */

  CIM_KEY_Multi_key        = 0xff20, /* Multi_key */
  CIM_KEY_Kanji            = 0xff21, /* Kanji */

  CIM_KEY_Kana_Shift       = 0xff2e, /* Kana_Shift */

  CIM_KEY_Hangul           = 0xff31, /* Hangul */

  CIM_KEY_Hangul_Hanja     = 0xff34, /* Hangul_Hanja */

  CIM_KEY_Home             = 0xff50, /* Home */
  CIM_KEY_Left             = 0xff51, /* Left */
  CIM_KEY_Up               = 0xff52, /* Up */
  CIM_KEY_Right            = 0xff53, /* Right */
  CIM_KEY_Down             = 0xff54, /* Down */
  CIM_KEY_Page_Up          = 0xff55, /* Page_Up */
  CIM_KEY_Page_Down        = 0xff56, /* Page_Down */
  CIM_KEY_End              = 0xff57, /* End */

  CIM_KEY_Print            = 0xff61, /* Print */
  CIM_KEY_Execute          = 0xff62, /* Execut */
  CIM_KEY_Insert           = 0xff63, /* Insert */

  CIM_KEY_Menu             = 0xff67, /* Menu */

  CIM_KEY_Break            = 0xff6b, /* Break */

  CIM_KEY_KP_Enter         = 0xff8d, /* KP_Enter */

  CIM_KEY_KP_Left          = 0xff96, /* KP_Left */
  CIM_KEY_KP_Up            = 0xff97, /* KP_Up */
  CIM_KEY_KP_Right         = 0xff98, /* KP_Right */
  CIM_KEY_KP_Down          = 0xff99, /* KP_Down */
  CIM_KEY_KP_Page_Up       = 0xff9a, /* KP_Page_Up */
  CIM_KEY_KP_Page_Down     = 0xff9b, /* KP_Page_Down */

  CIM_KEY_KP_Delete        = 0xff9f, /* KP_Delete */

  CIM_KEY_KP_Multiply      = 0xffaa, /* KP_Multiply */
  CIM_KEY_KP_Add           = 0xffab, /* KP_Add */

  CIM_KEY_KP_Subtract      = 0xffad, /* KP_Subtract */
  CIM_KEY_KP_Decimal       = 0xffae, /* KP_Decimal */
  CIM_KEY_KP_Divide        = 0xffaf, /* KP_Divide */
  CIM_KEY_KP_0             = 0xffb0, /* KP_0 */
  CIM_KEY_KP_1             = 0xffb1, /* KP_1 */
  CIM_KEY_KP_2             = 0xffb2, /* KP_2 */
  CIM_KEY_KP_3             = 0xffb3, /* KP_3 */
  CIM_KEY_KP_4             = 0xffb4, /* KP_4 */
  CIM_KEY_KP_5             = 0xffb5, /* KP_5 */
  CIM_KEY_KP_6             = 0xffb6, /* KP_6 */
  CIM_KEY_KP_7             = 0xffb7, /* KP_7 */
  CIM_KEY_KP_8             = 0xffb8, /* KP_8 */
  CIM_KEY_KP_9             = 0xffb9, /* KP_9 */

  CIM_KEY_F1               = 0xffbe, /* F1 */
  CIM_KEY_F2               = 0xffbf, /* F2 */
  CIM_KEY_F3               = 0xffc0, /* F3 */
  CIM_KEY_F4               = 0xffc1, /* F4 */
  CIM_KEY_F5               = 0xffc2, /* F5 */
  CIM_KEY_F6               = 0xffc3, /* F6 */
  CIM_KEY_F7               = 0xffc4, /* F7 */
  CIM_KEY_F8               = 0xffc5, /* F8 */
  CIM_KEY_F9               = 0xffc6, /* F9 */
  CIM_KEY_F10              = 0xffc7, /* F10 */
  CIM_KEY_F11              = 0xffc8, /* F11 */
  CIM_KEY_F12              = 0xffc9, /* F12 */

  CIM_KEY_Shift_L          = 0xffe1, /* Shift_L */
  CIM_KEY_Shift_R          = 0xffe2, /* Shift_R */
  CIM_KEY_Control_L        = 0xffe3, /* Control_L */
  CIM_KEY_Control_R        = 0xffe4, /* Control_R */
  CIM_KEY_Caps_Lock        = 0xffe5, /* Caps_Lock */
  CIM_KEY_Shift_Lock       = 0xffe6, /* Shift_Lock */
  CIM_KEY_Meta_L           = 0xffe7, /* Meta_L */
  CIM_KEY_Meta_R           = 0xffe8, /* Meta_R */
  CIM_KEY_Alt_L            = 0xffe9, /* Alt_L */
  CIM_KEY_Alt_R            = 0xffea, /* Alt_R */
  CIM_KEY_Super_L          = 0xffeb, /* Super_L */
  CIM_KEY_Super_R          = 0xffec, /* Super_R */
  CIM_KEY_Hyper_L          = 0xffed, /* Hyper_L */
  CIM_KEY_Hyper_R          = 0xffee, /* Hyper_R */

  CIM_KEY_Delete           = 0xffff, /* Delete */

  CIM_KEY_VoidSymbol       = 0xffffff,   /* VoidSymbol */

  CIM_KEY_Switch_VT_1      = 0x1008fe01, /* Switch_VT_1 */
  CIM_KEY_Switch_VT_2      = 0x1008fe02, /* Switch_VT_2 */
  CIM_KEY_Switch_VT_3      = 0x1008fe03, /* Switch_VT_3 */
  CIM_KEY_Switch_VT_4      = 0x1008fe04, /* Switch_VT_4 */
  CIM_KEY_Switch_VT_5      = 0x1008fe05, /* Switch_VT_5 */
  CIM_KEY_Switch_VT_6      = 0x1008fe06, /* Switch_VT_6 */
  CIM_KEY_Switch_VT_7      = 0x1008fe07, /* Switch_VT_7 */
  CIM_KEY_Switch_VT_8      = 0x1008fe08, /* Switch_VT_8 */
  CIM_KEY_Switch_VT_9      = 0x1008fe09, /* Switch_VT_9 */
  CIM_KEY_Switch_VT_10     = 0x1008fe0a, /* Switch_VT_10 */
  CIM_KEY_Switch_VT_11     = 0x1008fe0b, /* Switch_VT_11 */
  CIM_KEY_Switch_VT_12     = 0x1008fe0c, /* Switch_VT_12 */

  CIM_KEY_HomePage         = 0x1008ff18, /* HomePage */

  CIM_KEY_WakeUp           = 0x1008ff2b, /* WakeUp */

  CIM_KEY_WebCam           = 0x1008ff8f, /* WebCam */

  CIM_KEY_WLAN             = 0x1008ff95  /* WLAN */
};
typedef enum _CimKeySym CimKeySym;

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
  CIM_ITEM_STRING,
  CIM_ITEM_N_TYPES
};
typedef enum _CimItemType CimItemType;

typedef struct _CimItem CimItem;
struct _CimItem {
  CimItemType type;
  void* data;
};

typedef struct _CimCandidate CimCandidate;
struct _CimCandidate {
  /* Page index means the current page. Page index starts at 0. */
  int       page_index;
  int       n_pages;
  CimItem** table;
  int       n_rows;
  int       n_cols;
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
  /* Do not free CimSurround and its text */
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
/* utility functions */
char* cim_get_cim_so_path ();

#ifdef __cplusplus
}
#endif

#endif /* __CIM_H__ */
