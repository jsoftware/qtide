
#include <QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>
#if defined(QT60)
#include <QRegularExpressionValidator>
#else
#include <QRegExpValidator>
#endif

#include "cmd.h"
#include "edit.h"
#include "form.h"
#include "lineedit.h"
#include "pane.h"
#include "wd.h"

// ---------------------------------------------------------------------
Edit::Edit(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="edit";
  LineEdit *w=new LineEdit(this);
  widget=(QWidget*) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"password readonly left right center uppercase")) return;
  if (1<(opt.contains("left")?1:0) + (opt.contains("right")?1:0) + (opt.contains("center")?1:0)) {
    error("conflicting child style: " + n + " " + q2s(opt.join(" ")));
    return;
  }
  w->setObjectName(qn);
  focusSelect=false;
  childStyle(opt);

  uppercase=opt.contains("uppercase");

  if (opt.contains("password"))
    w->setEchoMode(LineEdit::Password);

  if (opt.contains("readonly"))
    w->setReadOnly(true);

  if (opt.contains("left"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  else if (opt.contains("right"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  else if (opt.contains("center"))
    w->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

  connect(w,SIGNAL(returnPressed()),
          this,SLOT(returnPressed()));
  connect(w,SIGNAL(textChanged(QString)),
          this,SLOT(textChanged()));
}

// ---------------------------------------------------------------------
void Edit::returnPressed()
{
  event="button";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Edit::textChanged()
{
  LineEdit *w=(LineEdit*) widget;
  if(uppercase) {
    w->setText(w->text().toUpper());
  }
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
std::string Edit::get(std::string p,std::string v)
{
  LineEdit *w=(LineEdit*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("alignment")+"\012"+ "focusselect"+"\012"+ "inputmask"+"\012"+ "limit"+"\012"+ "readonly"+"\012"+ "select"+"\012"+ "text"+"\012";
    r+=Child::get(p,v);
  } else if (p=="alignment") {
    if ((w->alignment())&Qt::AlignRight)
      r=std::string("right");
    else if ((w->alignment())&Qt::AlignHCenter)
      r=std::string("center");
    else
      r=std::string("left");
  } else if (p=="focusselect")
    r=i2s(focusSelect);
  else if (p=="inputmask")
    r=q2s(w->inputMask());
  else if (p=="limit")
    r=i2s(w->maxLength());
  else if (p=="readonly")
    r=i2s(w->isReadOnly());
  else if (p=="text")
    r=q2s(w->text());
  else if (p=="select") {
    int b,e;
    b=w->selectionStart();
    if (b<0)
      b=e=w->cursorPosition();
    else
      e=b+w->selectedText().size();
    r=i2s(b)+" "+i2s(e);
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Edit::set(std::string p,std::string v)
{
  LineEdit *w = (LineEdit *)widget;
  QStringList opt=qsplit(v);

  if (p=="text")
    w->setText(s2q(remquotes(v)));
  else if (p=="cursorposition") {
    if (opt.isEmpty()) {
      error("set cursorposition requires 1 number: " + id + " " + p);
      return;
    }
    int p=c_strtoi(q2s(opt.at(0)));
    p=qMax(0,qMin(p,w->text().length()));
    w->setCursorPosition(p);
  } else if (p=="limit") {
    if (opt.isEmpty()) {
      error("set limit requires 1 number: " + id + " " + p);
      return;
    }
    w->setMaxLength(c_strtoi(q2s(opt.at(0))));
  } else if (p=="focusselect") {
    focusSelect=remquotes(v)!="0";
  } else if (p=="focus") {
    w->setFocus();
    if (focusSelect) w->selectAll();
  } else if (p =="placeholder") {
#ifdef QT50
    w->setClearButtonEnabled(true);
#endif
    w->setPlaceholderText(s2q(remquotes(v)));
  } else if (p=="readonly") {
    w->setReadOnly(remquotes(v)!="0");
  } else if (p=="select") {
    w->selectAll();
  } else if (p=="alignment") {
    if (opt.isEmpty()) {
      error("set alignment requires 1 argument: " + id + " " + p);
      return;
    }
    if (opt.at(0)=="left")
      w->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
    else if (opt.at(0)=="right")
      w->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    else if (opt.at(0)=="center")
      w->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
    else {
      error("set alignment requires left, right or center: " + id + " " + p);
      return;
    }
  } else if (p=="inputmask") {
// see http://qt-project.org/doc/qt-4.8/qlineedit.html#inputMask-prop
    if (opt.isEmpty())
      w->setInputMask("");
    else
      w->setInputMask(opt.at(0));
  } else if (p=="intvalidator") {
    if (opt.isEmpty())
      w->setValidator(0);
    else if (2>opt.size()) {
      error("set intvalidator requires 2 numbers: " + id + " " + p);
      return;
    } else {
      w->setLocale(QLocale::C);
      QIntValidator *validator=new QIntValidator(c_strtoi(q2s(opt.at(0))),c_strtoi(q2s(opt.at(1))),w);
      validator->setLocale(QLocale::C);
      w->setValidator(validator);
    }
  } else if (p=="doublevalidator") {
    if (opt.isEmpty())
      w->setValidator(0);
    else if (3>opt.size()) {
      error("set doublevalidator requires 3 numbers: " + id + " " + p);
      return;
    } else {
      w->setLocale(QLocale::C);
      QDoubleValidator *validator=new QDoubleValidator(c_strtod(q2s(opt.at(0))),c_strtod(q2s(opt.at(1))),c_strtoi(q2s(opt.at(2))),w);
      validator->setLocale(QLocale::C);
      w->setValidator(validator);
    }
  } else if (p=="regexpvalidator") {
// see http://qt-project.org/doc/qt-4.8/qregexp.html
    if (opt.isEmpty())
      w->setValidator(0);
    else {
#if defined(QT60)
      QRegularExpression rx(opt.at(0));
      w->setValidator(new QRegularExpressionValidator(rx, w));
#else
      QRegExp rx(opt.at(0));
      w->setValidator(new QRegExpValidator(rx,w));
#endif
    }
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Edit::state()
{
  LineEdit *w=(LineEdit*) widget;
  int b,e;
  b=w->selectionStart();
  if (b<0)
    b=e=w->cursorPosition();
  else
    e=b+w->selectedText().size();
  std::string r;
  r+=spair(id,q2s(w->text()));
  r+=spair(id+"_select",i2s(b)+" "+i2s(e));
  return r;
}
