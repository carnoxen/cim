/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * cim.c
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
#include "cim.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <stdatomic.h>
#include "c-utils.h"
#include "c-str.h"
#include "c-mem.h"
#include "c-log.h"

static void    *cim_plugin;
static CimIc* (*cim_plugin_new)  ();
static void   (*cim_plugin_free) (CimIc*);
static atomic_uint cim_ref_count;

/*
 * Returns the newly allocated cim.so path string on success,
 * or NULL on failure.
 * Free it with free().
 */
char* cim_get_cim_so_path ()
{
  char* path;
  char* conf_dir;

  conf_dir = c_get_user_config_dir ();

  if (!conf_dir)
    return NULL;

  path = c_str_join (conf_dir, "/cim.so", NULL);

  free (conf_dir);

  return path;
}

/*
 * Returns a newly allocated CimIc.
 */
CimIc* cim_ic_new ()
{
  cim_ref_count++;

  if (cim_ref_count == 1)
  {
    char* path = cim_get_cim_so_path ();

    if (!path)
      goto fail;

    cim_plugin = dlopen (path, RTLD_LAZY | RTLD_LOCAL);
    free (path);

    if (!cim_plugin)
      goto fail;

    cim_plugin_new  = dlsym (cim_plugin, "cim_plugin_new");
    cim_plugin_free = dlsym (cim_plugin, "cim_plugin_free");

    if (!cim_plugin_new || !cim_plugin_free)
    {
      dlclose (cim_plugin);
      cim_plugin      = NULL;
      cim_plugin_new  = NULL;
      cim_plugin_free = NULL;

      goto fail;
    }
  }

  if (cim_plugin_new && cim_plugin_free)
    return cim_plugin_new ();

  fail:
/*
 * TODO: Fallback
 * Cannot open "/home/username/.config/cim.so"
 */
  return c_calloc (1, sizeof (CimIc));
}

void cim_ic_free (CimIc* ic)
{
  cim_ref_count--;

  if (cim_plugin_free)
    cim_plugin_free (ic);
  else
    free (ic);

  if (cim_ref_count == 0)
  {
    if (cim_plugin)
      dlclose (cim_plugin);

    cim_plugin      = NULL;
    cim_plugin_new  = NULL;
    cim_plugin_free = NULL;
  }
}

void cim_ic_focus_in (CimIc* ic)
{
  if (ic->focus_in)
    ic->focus_in (ic);
}

void cim_ic_focus_out (CimIc* ic)
{
  if (ic->focus_out)
    ic->focus_out (ic);
}

void cim_ic_reset (CimIc* ic)
{
  if (ic->reset)
    ic->reset (ic);
}

bool cim_ic_filter_event (CimIc* ic, const CimEvent* event)
{
  if (ic->filter_event)
    return ic->filter_event (ic, event);

  return false;
}

void cim_ic_set_cursor_pos (CimIc* ic, const CimRect* area)
{
  if (ic->set_cursor_pos)
    ic->set_cursor_pos (ic, area);
}

const CimPreedit* cim_ic_get_preedit (CimIc* ic)
{
  if (ic->get_preedit)
    return ic->get_preedit (ic);

  static const CimPreedit preedit = { "", NULL, 0, 0 };
  return &preedit;
}

const CimCandidate* cim_ic_get_candidate (CimIc* ic)
{
  if (ic->get_candidate)
    return ic->get_candidate (ic);

  static const CimCandidate candidate;
  return &candidate;
}

void cim_ic_set_callback (CimIc*    ic,
                          CimCbType type,
                          void*     callback,
                          void*     user_data)
{
  if (ic->set_callback)
    ic->set_callback (ic, type, callback, user_data);
  else
    c_log_critical ("set_callback() must be implemented in the IM plugin.");
}
