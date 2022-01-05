
#include <QDateEdit>

#include <math.h>

#include "cmd.h"
#include "dateedit.h"
#include "form.h"
#include "pane.h"
#include "wd.h"

// optional parms are:
// minimum
// single step
// maximum
// value

/*
display format:

These expressions may be used for the date:

Expression                                           Output
d          the day as number without a leading zero (1 to 31)
dd         the day as number with a leading zero (01 to 31)
ddd        the abbreviated localized day name (e.g. 'Mon' to 'Sun'). Uses QDate::shortDayName().
dddd       the long localized day name (e.g. 'Monday' to 'Qt::Sunday'). Uses QDate::longDayName().
M          the month as number without a leading zero (1-12)
MM         the month as number with a leading zero (01-12)
MMM        the abbreviated localized month name (e.g. 'Jan' to 'Dec'). Uses QDate::shortMonthName().
MMMM       the long localized month name (e.g. 'January' to 'December'). Uses QDate::longMonthName().
yy         the year as two digit number (00-99)
yyyy       the year as four digit number

These expressions may be used for the time:

Expression                                 Output
h          the hour without a leading zero (0 to 23 or 1 to 12 if AM/PM display)
hh         the hour with a leading zero (00 to 23 or 01 to 12 if AM/PM display)
m          the minute without a leading zero (0 to 59)
mm         the minute with a leading zero (00 to 59)
s          the second without a leading zero (0 to 59)
ss         the second with a leading zero (00 to 59)
z          the milliseconds without leading zeroes (0 to 999)
zzz        the milliseconds with leading zeroes (000 to 999)
AP         use AM/PM display. AP will be replaced by either "AM" or "PM".
ap         use am/pm display. ap will be replaced by either "am" or "pm".

All other input characters will be ignored. Any sequence of characters that are enclosed in singlequotes will be treated as text and not be
used as an expression. Two consecutive singlequotes ("''") are replaced by a singlequote in the output.

Example format std::strings (assumed that the QDateTime is 21 May 2001 14:13:09):

    Format         Result
dd.MM.yyyy     21.05.2001
ddd MMMM d yy  Tue May 21 01
hh:mm:ss.zzz   14:13:09.042
h:m:s ap       2:13:9 pm
*/

static void toymd(int v, int *y, int *m, int *d);

// ---------------------------------------------------------------------
DateEdit::DateEdit(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="dateedit";
  QDateEdit *w=new QDateEdit;
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);

  w->setCalendarPopup(true);

  int i=0;
  int v,y,m,d;
  if (i<opt.size()) {
    v=c_strtoi(q2s(opt.at(i)));
    toymd(v, &y, &m, &d);
    w->setMinimumDate(QDate(y,m,d));
    i++;
  }
  if (i<opt.size()) {
    v=c_strtoi(q2s(opt.at(i)));
    toymd(v, &y, &m, &d);
    w->setMaximumDate(QDate(y,m,d));
    i++;
  }
  if (i<opt.size()) {
    v=c_strtoi(q2s(opt.at(i)));
    if (v) {
      toymd(v, &y, &m, &d);
      w->setDate(QDate(y,m,d));
    } else w->setDate(QDate());
    i++;
  }
  connect(w,SIGNAL(dateChanged(QDate)),
          this,SLOT(valueChanged()));
}

// ---------------------------------------------------------------------
void DateEdit::valueChanged()
{
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
std::string DateEdit::get(std::string p,std::string v)
{
  QDateEdit *w=(QDateEdit*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("format")+"\012"+ "max"+"\012"+ "min"+"\012"+ "readonly"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="format")
    r=q2s(w->displayFormat());
  else if (p=="max") {
    QDate q=w->maximumDate();
    if (q.isNull())
      r=std::string("0");
    else r=i2s((10000*q.year())+(100*q.month())+q.day());
  } else if (p=="min") {
    QDate q=w->minimumDate();
    if (q.isNull())
      r=std::string("0");
    else r=i2s((10000*q.year())+(100*q.month())+q.day());
  } else if (p=="readonly")
    r=i2s(w->isReadOnly());
  else if (p=="value") {
    QDate q=w->date();
    if (q.isNull())
      r=std::string("0");
    else r=i2s((10000*q.year())+(100*q.month())+q.day());
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void DateEdit::set(std::string p,std::string v)
{
  QDateEdit *w=(QDateEdit*) widget;
  QString cmd=s2q(p);
  QStringList arg=qsplit(v);
  if (arg.isEmpty()) {
    Child::set(p,v);
    return;
  }
  int i,y,m,d;
  if (cmd=="format") {
    w->setDisplayFormat(s2q(remquotes(v)));
  } else if (cmd=="min") {
    i=c_strtoi(q2s(arg.at(0)));
    toymd(i, &y, &m, &d);
    w->setMinimumDate(QDate(y,m,d));
  } else if (cmd=="max") {
    i=c_strtoi(q2s(arg.at(0)));
    toymd(i, &y, &m, &d);
    w->setMaximumDate(QDate(y,m,d));
  } else if (p=="readonly") {
    w->setReadOnly(remquotes(v)!="0");
  } else if (cmd=="value") {
// TODO actually null date does not work because of input mask
    i=c_strtoi(q2s(arg.at(0)));
    if (i) {
      toymd(i, &y, &m, &d);
      w->setDate(QDate(y,m,d));
    } else w->setDate(QDate());
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string DateEdit::state()
{
  QDateEdit *w=(QDateEdit*) widget;
  QDate q=w->date();
  if (q.isNull())
    return spair(id,i2s(0));
  else return spair(id,i2s((10000*q.year())+(100*q.month())+q.day()));
}

// ---------------------------------------------------------------------
void toymd(int v, int *y, int *m, int *d)
{
  *y=(int)floor((float)v/10000);
  v=v%10000;
  *m=(int)floor((float)v/100);
  *d=v%100;
}
