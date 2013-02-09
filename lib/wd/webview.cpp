
#include <QtWebKit>

#include "wd.h"
#include "webview.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Webview::Webview(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="webview";
  QWebView *w=new QWebView;
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html"));
}

// ---------------------------------------------------------------------
void Webview::set(string p)
{
  ((QWebView *)widget)->load(QUrl(s2q(p)));
  ((QWebView *)widget)->show();
}

// ---------------------------------------------------------------------
void Webview::setp(string p,string v)
{
  if (p=="baseurl") {
    baseUrl = QUrl(s2q(v));
  } else if (p=="html") {
    ((QWebView *)widget)->setHtml(s2q(v), baseUrl);
    ((QWebView *)widget)->show();
  } else Child::setp(p,v);
}
