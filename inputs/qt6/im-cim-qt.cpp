/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 2; tab-width: 2 -*- */
/*
 * im-cim-qt.cpp
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
#include <QTextFormat>
#include <QInputMethodEvent>
#include <QtGui/qpa/qplatforminputcontext.h>
#include <QtGui/qpa/qplatforminputcontextplugin_p.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "cim.h"

class CimEventHandler : public QObject
{
  Q_OBJECT

public:
  CimEventHandler(CimIc* ic)
  {
    m_ic = ic;
  };

  ~CimEventHandler()
  {
  };

private:
  CimIc* m_ic;
};

class CimQic : public QPlatformInputContext
{
  Q_OBJECT
public:
   CimQic ();
  ~CimQic ();

  virtual bool isValid () const;
  virtual void reset ();
  virtual void commit ();
  virtual void update (Qt::InputMethodQueries);
  virtual void invokeAction (QInputMethod::Action, int cursorPosition);
  virtual bool filterEvent (const QEvent* event);
  virtual QRectF keyboardRect () const;
  virtual bool isAnimating () const;
  virtual void showInputPanel ();
  virtual void hideInputPanel ();
  virtual bool isInputPanelVisible () const;
  virtual QLocale locale () const;
  virtual Qt::LayoutDirection inputDirection() const;
  virtual void setFocusObject (QObject* object);

  // cim signal callbacks
  static void cb_preedit_start   (CimIc* ic, void* user_data);
  static void cb_preedit_end     (CimIc* ic, void* user_data);
  static void cb_preedit_changed (CimIc* ic,
                                  const CimPreedit* preedit,
                                  void* user_data);
  static void cb_commit          (CimIc* ic,
                                  const char* text,
                                  void* user_data);
  static const CimSurround* cb_get_surround (CimIc* ic, void* user_data);
  static bool cb_delete_surround (CimIc* ic,
                                  int    offset,
                                  int    n_chars,
                                  void*  user_data);
private:
  CimIc*      m_ic;
  CimRect     m_cursor_area;
  CimSurround m_surround;
};

/* cim signal callbacks */
void CimQic::cb_preedit_start (CimIc* ic, void* user_data)
{
}

void CimQic::cb_preedit_end (CimIc* ic, void* user_data)
{
}

void CimQic::cb_preedit_changed (CimIc*            ic,
                                 const CimPreedit* preedit,
                                 void*             user_data)
{
  int offset = 0;
  int len;

  QString preedit_text = QString::fromUtf8 (preedit->text);

  QList <QInputMethodEvent::Attribute> attrs;

  /* qsizetype is signed */
  for (qsizetype i = 0; i < preedit_text.size(); i++)
  {
    if (preedit_text.at(i).isLowSurrogate())
    {
      offset++;
      continue;
    }

    QTextCharFormat format;

    for (int j = 0; preedit->attrs_len < j; j++)
    {
      switch (preedit->attrs[j].type)
      {
        case CIM_PREEDIT_ATTR_HIGHLIGHT:
          if (preedit->attrs[j].start_index <= i - offset &&
              preedit->attrs[j].end_index   >  i - offset)
          {
            format.setBackground(Qt::green);
            format.setForeground(Qt::black);
          }
          break;
        case CIM_PREEDIT_ATTR_UNDERLINE:
          if (preedit->attrs[j].start_index <= i - offset &&
              preedit->attrs[j].end_index   >  i - offset)
            format.setUnderlineStyle(QTextCharFormat::DashUnderline);
          break;
        default:
          break;
      }
    }

    preedit_text.at(i).isHighSurrogate() ? len = 2 : len = 1;
    QInputMethodEvent::Attribute attr (QInputMethodEvent::TextFormat,
                                       i, len, format);
    attrs << attr;
  }

  // cursor attribute
  attrs << QInputMethodEvent::Attribute (QInputMethodEvent::Cursor,
                                         preedit->cursor_pos + offset, true, 0);

  QInputMethodEvent event (preedit_text, attrs);
  QObject* object = qApp->focusObject ();

  if (object)
    QCoreApplication::sendEvent (object, &event);
}

void CimQic::cb_commit (CimIc* ic, const char* text, void* user_data)
{
  QString str = QString::fromUtf8 (text);
  QInputMethodEvent event;
  event.setCommitString (str);

  QObject* obj = qApp->focusObject();

  if (obj)
    QCoreApplication::sendEvent (obj, &event);
}

const CimSurround* CimQic::cb_get_surround (CimIc* ic, void* user_data)
{
  QObject* object = qApp->focusObject();

  if (!object)
    return NULL;

  CimQic* context = static_cast<CimQic*>(user_data);

  QInputMethodQueryEvent surround_query (Qt::ImSurroundingText);
  QInputMethodQueryEvent cursor_query   (Qt::ImCursorPosition);
  QInputMethodQueryEvent anchor_query   (Qt::ImAnchorPosition);

  QCoreApplication::sendEvent (object, &surround_query);
  QCoreApplication::sendEvent (object, &cursor_query);
  QCoreApplication::sendEvent (object, &anchor_query);

  QString string  = surround_query.value (Qt::ImSurroundingText).toString();
  uint cursor_pos = cursor_query.value   (Qt::ImCursorPosition).toUInt();
  uint anchor_pos = cursor_query.value   (Qt::ImAnchorPosition).toUInt();

  context->m_surround.text = (char*) string.toUtf8().constData();
  context->m_surround.len  = -1;
  context->m_surround.cursor_pos = cursor_pos;
  context->m_surround.anchor_pos = anchor_pos;

  return &context->m_surround;
}

bool CimQic::cb_delete_surround (CimIc* ic,
                                 int     offset,
                                 int     n_chars,
                                 void*   user_data)
{
  QObject* object = qApp->focusObject();

  if (!object)
    return false;

  QInputMethodEvent event;
  event.setCommitString ("", offset, n_chars);
  QCoreApplication::sendEvent (object, &event);

  return true;
}

CimQic::CimQic ()
{
  m_ic                  = NULL;
  m_cursor_area.x       = 0;
  m_cursor_area.y       = 0;
  m_cursor_area.width   = 0;
  m_cursor_area.height  = 0;
  m_surround.text       = NULL;
  m_surround.len        = 0;
  m_surround.cursor_pos = 0;

  m_ic = cim_ic_new ();
  cim_ic_set_callback (m_ic, CIM_CB_PREEDIT_START,
                       (void*) cb_preedit_start, this);
  cim_ic_set_callback (m_ic, CIM_CB_PREEDIT_END, (void*) cb_preedit_end, this);
  cim_ic_set_callback (m_ic, CIM_CB_PREEDIT_CHANGED,
                       (void*) cb_preedit_changed, this);
  cim_ic_set_callback (m_ic, CIM_CB_COMMIT, (void*) cb_commit, this);
  cim_ic_set_callback (m_ic, CIM_CB_GET_SURROUND,
                       (void*) cb_get_surround, this);
  cim_ic_set_callback (m_ic, CIM_CB_DELETE_SURROUND,
                       (void*) cb_delete_surround, this);
}

CimQic::~CimQic ()
{
  if (m_ic)
    cim_ic_free (m_ic);
}

bool CimQic::isValid () const
{
  if (m_ic == NULL)
    return false;

  return true;
}

void CimQic::reset ()
{
  cim_ic_reset (m_ic);
}

void CimQic::commit ()
{
  cim_ic_reset (m_ic);
}

void CimQic::update (Qt::InputMethodQueries queries)
{
  if (queries & Qt::ImCursorRectangle)
  {
    QWidget* widget = qApp->focusWidget ();

    if (widget == NULL)
      return;

    QRect  rect  = widget->inputMethodQuery(Qt::ImCursorRectangle).toRect();
    QPoint point = widget->mapToGlobal (QPoint (0, 0));
    rect.translate (point);

    if (m_cursor_area.x      != rect.x ()     ||
        m_cursor_area.y      != rect.y ()     ||
        m_cursor_area.width  != rect.width () ||
        m_cursor_area.height != rect.height ())
    {
      m_cursor_area.x      = rect.x ();
      m_cursor_area.y      = rect.y ();
      m_cursor_area.width  = rect.width ();
      m_cursor_area.height = rect.height ();

      cim_ic_set_cursor_pos (m_ic, &m_cursor_area);
    }
  }
}

void CimQic::invokeAction (QInputMethod::Action, int cursorPosition)
{
}

bool CimQic::filterEvent (const QEvent* event)
{
  if (!qApp->focusObject() || !inputMethodAccepted())
    return false;

  bool  retval;
  const QKeyEvent* key_event = static_cast<const QKeyEvent*>(event);
  CimEvent cevent;

  switch (event->type ())
  {
#undef KeyPress
    case QEvent::KeyPress:
      cevent.type = CIM_EVENT_KEY_PRESS;
      break;
#undef KeyRelease
    case QEvent::KeyRelease:
      cevent.type = CIM_EVENT_KEY_RELEASE;
      break;
    default:
      return false;
  }

  cevent.state   = key_event->nativeModifiers  ();
  cevent.keyval  = key_event->nativeVirtualKey ();
  cevent.keycode = key_event->nativeScanCode   ();

  retval = cim_ic_filter_event (m_ic, &cevent);

  return retval;
}

QRectF CimQic::keyboardRect() const
{
  return QRectF ();
}

bool CimQic::isAnimating() const
{
  return false;
}

void CimQic::showInputPanel()
{
}

void CimQic::hideInputPanel()
{
}

bool CimQic::isInputPanelVisible() const
{
  return false;
}

QLocale CimQic::locale() const
{
  return QLocale ();
}

Qt::LayoutDirection CimQic::inputDirection() const
{
  return Qt::LayoutDirection ();
}

void CimQic::setFocusObject (QObject* object)
{
  if (!object || !inputMethodAccepted())
    cim_ic_focus_out (m_ic);

  QPlatformInputContext::setFocusObject (object);

  if (object && inputMethodAccepted())
    cim_ic_focus_in (m_ic);

  update (Qt::ImCursorRectangle);
}

/*
 * class CimQicPlugin
 */
class CimQicPlugin : public QPlatformInputContextPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID
    QPlatformInputContextFactoryInterface_iid
    FILE "./cim.json")

public:
  CimQicPlugin ()
  {
  }

  ~CimQicPlugin ()
  {
  }

  virtual QStringList keys () const
  {
    return QStringList () << "cim";
  }

  virtual QPlatformInputContext* create (const QString     &key,
                                         const QStringList &paramList)
  {
    return new CimQic ();
  }
};

#include "im-cim-qt.moc"
