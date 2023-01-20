/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * cim.c
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
#include "cim.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <stdatomic.h>

/****************************************************************************/
#include <stdarg.h>

static void* c_calloc (size_t number, size_t size)
{
  void* mem = calloc (number, size);

  if (mem)
    return mem;

  perror (__PRETTY_FUNCTION__);
  abort ();
}

static void* c_realloc (void* ptr, size_t size)
{
  if (!size)
  {
    free (ptr);
    return NULL;
  }

  void* mem = realloc (ptr, size);

  if (mem)
    return mem;

  perror (__PRETTY_FUNCTION__);
  abort ();
}

static char* c_str_join (const char* str, ...)
{
  va_list ap;
  size_t  offset = 0;
  char*   result = NULL;

  va_start (ap, str);

  for (const char* s = str; s; s = va_arg (ap, const char*))
  {
    size_t len = strlen (s);
    result = c_realloc (result, offset + len + 1);
    memcpy (result + offset, s, len);
    offset = offset + len;
  }

  va_end (ap);

  result[offset] = 0;

  return result;
}

static uid_t c_get_loginuid ()
{
  uid_t loginuid = -1;

#ifdef __linux__
  FILE* file;

  file = fopen ("/proc/self/loginuid", "rt");

  if (file)
  {
    uid_t uid;

    if (fscanf (file, "%d", &uid) > 0)
      loginuid = uid;

    fclose (file);
  }
#else
  const char* name;

  if ((name = getlogin ()))
  {
    const struct passwd* info;

    if ((info = getpwnam (name)))
      loginuid = info->pw_uid;
  }
#endif

  if (loginuid == (uid_t) -1)
    loginuid = getuid ();

  return loginuid;
}

/* FIXME: It should be fixed so that it does not return NULL. */
static const char* c_get_user_home_dir ()
{
  return getpwuid (c_get_loginuid ())->pw_dir;
}

static char* c_get_user_config_dir ()
{
  const char* s1;
  const char* s2 = "/.config";

  s1 = c_get_user_home_dir ();

  if (!s1)
    return NULL;

  return c_str_join (s1, s2, NULL);
}

/****************************************************************************/

static void    *cim_plugin;
static CimIc* (*cim_plugin_new)  ();
static void   (*cim_plugin_free) (CimIc*);
static atomic_uint cim_ref_count;

static char* cim_get_cim_so_path ()
{
  char* path;
  char* conf_dir;

  conf_dir = c_get_user_config_dir ();
  path     = c_str_join (conf_dir, "/cim.so", NULL);

  free (conf_dir);

  return path;
}

static void print_dlerror ()
{
  const char* errstr = dlerror ();
  if (errstr)
    fprintf (stderr, "%s\n", errstr);
}

CimIc* cim_ic_new ()
{
  cim_ref_count++;

  if (cim_ref_count == 1)
  {
    char* path = cim_get_cim_so_path ();
    cim_plugin = dlopen (path, RTLD_LAZY | RTLD_LOCAL);
    free (path);

    if (!cim_plugin)
    {
      print_dlerror ();
      goto fail;
    }

    cim_plugin_new  = dlsym (cim_plugin, "cim_plugin_new");
    cim_plugin_free = dlsym (cim_plugin, "cim_plugin_free");

    if (!cim_plugin_new || !cim_plugin_free)
    {
      print_dlerror ();
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
}
