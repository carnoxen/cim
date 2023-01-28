/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * libim-cim-gtk4.c
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
#include <gtk/gtkimmodule.h>
#include "cim.h"
#ifdef GDK_WINDOWING_X11
#include <gdk/x11/gdkx.h>
#endif

#define CIM_GIC(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), cim_gic_get_type (), CimGic))

typedef struct _CimGic      CimGic;
typedef struct _CimGicClass CimGicClass;

struct _CimGic
{
  GtkIMContext  parent_instance;

  CimIc*        ic;
  GtkIMContext* simple;
  GtkWidget*    client_widget;
  CimSurround   surround;
};

struct _CimGicClass
{
  GtkIMContextClass parent_class;
};

G_DEFINE_DYNAMIC_TYPE (CimGic, cim_gic, GTK_TYPE_IM_CONTEXT);

static gboolean cim_gic_filter_keypress (GtkIMContext* context, GdkEvent* event)
{
  gboolean retval;
  CimEvent cevent;

  if (gdk_event_get_event_type (event) == GDK_KEY_PRESS)
    cevent.type = CIM_EVENT_KEY_PRESS;
  else
    cevent.type = CIM_EVENT_KEY_RELEASE;

  cevent.state   = gdk_event_get_modifier_state (event);
  cevent.keyval  = gdk_key_event_get_keyval  (event);
  cevent.keycode = gdk_key_event_get_keycode (event);

  retval = cim_ic_filter_event (CIM_GIC (context)->ic, &cevent);

  if (!retval)
    return gtk_im_context_filter_keypress (CIM_GIC (context)->simple, event);

  return retval;
}

static void cim_gic_reset (GtkIMContext* context)
{
  cim_ic_reset (CIM_GIC (context)->ic);
  gtk_im_context_reset (CIM_GIC (context)->simple);
}

static void cim_gic_set_client_widget (GtkIMContext* context, GtkWidget* widget)
{
  CimGic* gic = CIM_GIC (context);

  if (gic->client_widget)
  {
    g_object_unref (gic->client_widget);
    gic->client_widget = NULL;
  }

  if (widget)
    gic->client_widget = g_object_ref (widget);
}

static void cim_gic_get_preedit_string (GtkIMContext*   context,
                                        char**          text,
                                        PangoAttrList** attrs,
                                        int*            cursor_pos)
{
  const CimPreedit* preedit;

  preedit = cim_ic_get_preedit (CIM_GIC (context)->ic);

  if (text)
    *text = g_strdup (preedit->text);

  if (cursor_pos)
    *cursor_pos = preedit->cursor_pos;

  if (attrs)
  {
    PangoAttribute* attr;
    char* p1;
    char* p2;

    *attrs = pango_attr_list_new ();

    for (int i = 0; i < preedit->attrs_len; i++)
    {
      p1 = g_utf8_offset_to_pointer (preedit->text, preedit->attrs[i].start_index);
      p2 = g_utf8_offset_to_pointer (preedit->text, preedit->attrs[i].end_index);

      switch (preedit->attrs[i].type)
      {
        case CIM_PREEDIT_ATTR_UNDERLINE:
          attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
          break;
        case CIM_PREEDIT_ATTR_HIGHLIGHT:
          attr = pango_attr_background_new (0, 0xffff, 0);
          attr->start_index = p1 - preedit->text;
          attr->end_index   = p2 - preedit->text;
          pango_attr_list_insert (*attrs, attr);

          attr = pango_attr_foreground_new (0, 0, 0);
          break;
        default:
          attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
          break;
      }

      attr->start_index = p1 - preedit->text;
      attr->end_index   = p2 - preedit->text;
      pango_attr_list_insert (*attrs, attr);
    }
  }
}

static void cim_gic_focus_in (GtkIMContext* context)
{
  cim_ic_focus_in (CIM_GIC (context)->ic);
}

static void cim_gic_focus_out (GtkIMContext* context)
{
  cim_ic_focus_out (CIM_GIC (context)->ic);
}

static void cim_gic_set_cursor_pos (GtkIMContext* context, GdkRectangle* area)
{
  CimGic* gic = CIM_GIC (context);

  GdkRectangle root_area = *area;

  if (gic->client_widget)
  {
    GtkNative* native = gtk_widget_get_native (gic->client_widget);  /* do not free */

    if (native)
    {
#ifdef GDK_WINDOWING_X11

      GdkSurface* surface = gtk_native_get_surface (native); /* do not free */
      GdkDisplay* display = gdk_surface_get_display (surface); /* do not free */

      if (GDK_IS_X11_DISPLAY (display))
      {
        Window            child;
        XWindowAttributes attr;
        Display* xdisplay = gdk_x11_display_get_xdisplay (display);
        Window   xwin     = gdk_x11_surface_get_xid (surface);

        XGetWindowAttributes (xdisplay, xwin, &attr);

        if (area->x < 0 && area->y < 0)
          XTranslateCoordinates (xdisplay, xwin, attr.root, 0, attr.height,
                                 &root_area.x, &root_area.y, &child);
        else
          XTranslateCoordinates (xdisplay, xwin, attr.root, area->x, area->y,
                                 &root_area.x, &root_area.y, &child);
      }
#endif
    }
  }

  cim_ic_set_cursor_pos (gic->ic, (const CimRect*) &root_area);
}

static void cb_preedit_start (CimIc* unused, CimGic* gic)
{
  g_signal_emit_by_name (gic, "preedit-start");
}

static void cb_preedit_end (CimIc* unused, CimGic* gic)
{
  g_signal_emit_by_name (gic, "preedit-end");
}

static void cb_preedit_changed (CimIc*            unused1,
                                const CimPreedit* unused2,
                                CimGic*           gic)
{
  g_signal_emit_by_name (gic, "preedit-changed");
}

static void cim_gic_set_use_preedit (GtkIMContext* context,
                                     gboolean      use_preedit)
{
  CimGic* gic = CIM_GIC (context);

  if (use_preedit)
  {
    cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_START,   NULL, NULL);
    cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_END,     NULL, NULL);
    cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_CHANGED, NULL, NULL);
  }
  else
  {
    cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_START, cb_preedit_start, gic);
    cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_END, cb_preedit_end, gic);
    cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_CHANGED,
                         cb_preedit_changed, gic);
  }
}

static void cim_gic_set_surround (GtkIMContext* context,
                                  const char* text,
                                  int len,
                                  int cursor_index_in_bytes)
{
  CimGic* gic = CIM_GIC (context);
  gic->surround.text = (char*) text;
  gic->surround.len  = len;
  gic->surround.cursor_pos = g_utf8_strlen (text, cursor_index_in_bytes);
  gic->surround.anchor_pos = gic->surround.cursor_pos;
}

static void cim_gic_set_surround_with_selection (GtkIMContext* context,
                                                 const char* text,
                                                 int len,
                                                 int cursor_index_in_bytes,
                                                 int anchor_index_in_bytes)
{
  CimGic* gic = CIM_GIC (context);
  gic->surround.text = (char*) text;
  gic->surround.len  = len;
  gic->surround.cursor_pos = g_utf8_strlen (text, cursor_index_in_bytes);
  gic->surround.anchor_pos = g_utf8_strlen (text, anchor_index_in_bytes);
}

GtkIMContext* cim_gic_new ()
{
  return g_object_new (cim_gic_get_type (), NULL);
}

static void cb_commit (CimIc* unused, const char* text, CimGic* gic)
{
  g_signal_emit_by_name (gic, "commit", text);
}

static gboolean cb_delete_surround (CimIc*  unused,
                                    int     offset,
                                    int     n_chars,
                                    CimGic* gic)
{
  gboolean retval;
  g_signal_emit_by_name (gic, "delete-surrounding", offset, n_chars, &retval);
  return retval;
}

static gboolean cb_retrieve_surround (CimIc* unused, CimGic* gic)
{
  gboolean retval;
  g_signal_emit_by_name (gic, "retrieve-surrounding", &retval);

  return retval;
}

static const CimSurround* cb_get_surround (CimIc* unused, CimGic* gic)
{
  gboolean retval;
  g_signal_emit_by_name (gic, "retrieve-surrounding", &retval);

  if (retval)
    return &gic->surround;

  return NULL;
}

static void cim_gic_init (CimGic* gic)
{
  gic->ic     = cim_ic_new ();
  gic->simple = gtk_im_context_simple_new ();

  cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_START, cb_preedit_start, gic);
  cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_END, cb_preedit_end, gic);
  cim_ic_set_callback (gic->ic, CIM_CB_PREEDIT_CHANGED,
                       cb_preedit_changed, gic);
  cim_ic_set_callback (gic->ic, CIM_CB_COMMIT, cb_commit, gic);
  cim_ic_set_callback (gic->ic, CIM_CB_GET_SURROUND, cb_get_surround, gic);
  cim_ic_set_callback (gic->ic, CIM_CB_DELETE_SURROUND,
                       cb_delete_surround, gic);

  g_signal_connect (gic->simple, "commit", G_CALLBACK (cb_commit), gic);
  g_signal_connect (gic->simple, "delete-surrounding",
                    G_CALLBACK (cb_delete_surround), gic);
  g_signal_connect (gic->simple, "preedit-changed",
                    G_CALLBACK (cb_preedit_changed), gic);
  g_signal_connect (gic->simple, "preedit-end",
                    G_CALLBACK (cb_preedit_end), gic);
  g_signal_connect (gic->simple, "preedit-start",
                    G_CALLBACK (cb_preedit_start), gic);
  g_signal_connect (gic->simple, "retrieve-surrounding",
                    G_CALLBACK (cb_retrieve_surround), gic);
}

static void cim_gic_finalize (GObject* object)
{
  CimGic* gic = CIM_GIC (object);

  cim_ic_free   (gic->ic);
  g_object_unref (gic->simple);

  if (gic->client_widget)
    g_object_unref (gic->client_widget);

  G_OBJECT_CLASS (cim_gic_parent_class)->finalize (object);
}

static void cim_gic_class_init (CimGicClass* class)
{
  GObjectClass* object_class  = G_OBJECT_CLASS (class);
  GtkIMContextClass* ic_class = GTK_IM_CONTEXT_CLASS (class);

  ic_class->set_client_widget   = cim_gic_set_client_widget;
  ic_class->get_preedit_string  = cim_gic_get_preedit_string;
  ic_class->filter_keypress     = cim_gic_filter_keypress;
  ic_class->focus_in            = cim_gic_focus_in;
  ic_class->focus_out           = cim_gic_focus_out;
  ic_class->reset               = cim_gic_reset;
  ic_class->set_cursor_location = cim_gic_set_cursor_pos;
  ic_class->set_use_preedit     = cim_gic_set_use_preedit;
  ic_class->set_surrounding     = cim_gic_set_surround;
  ic_class->set_surrounding_with_selection
                                = cim_gic_set_surround_with_selection;

  object_class->finalize = cim_gic_finalize;
}

static void cim_gic_class_finalize (CimGicClass* class)
{
}

void g_io_module_load (GIOModule* module)
{
  g_type_module_use (G_TYPE_MODULE (module));
  cim_gic_register_type (G_TYPE_MODULE (module));
  g_io_extension_point_implement (GTK_IM_MODULE_EXTENSION_POINT_NAME,
                                  cim_gic_get_type (), "cim", 10);
}

void g_io_module_unload (GIOModule* module)
{
}

char** g_io_module_query (void)
{
  char** strv = g_malloc (2 *  sizeof (char*));
  strv[0] = g_strdup (GTK_IM_MODULE_EXTENSION_POINT_NAME);
  strv[1] = NULL;
  return strv;
}
