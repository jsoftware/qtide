// xl.cpp
//
// uses github.com/QtExcel/QXlsx
// for Qt types, see https://doc.qt.io/qt-6/qmetatype.html

#include "cmd.h"
#include "form.h"
//#include "pane.h"
#include "wd.h"
#include "../base/state.h"

#include "xlsxdocument.h"
#include "xlsxcellrange.h"
#include "xlsxworkbook.h"
#include "xlsxrichstring.h"

using namespace QXlsx;

extern int rc;

std::string xl(std::string p);

static std::string xlalign();
static std::string xlblock();
static std::string xlclose();
static std::string xlcolor();
static std::string xlcolwidth();
static std::string xldata();
static std::string xlformat();
static std::string xlmergecells();
static std::string xlnew();
static std::string xlopenfile();
static std::string xlopensheet();
static std::string xlread();
static std::string xlsave();
static std::string xlsheet();
static std::string xlstyle();

static bool defblock(QStringList);
static int gethalign(int);
static std::string errorlen(QString, int, bool);
static std::string qsl2s(QStringList);
static void setcommon();

static Document *xlsx;

static QStringList arg;
static Cmd cmd;

static QString filename;
static QString sheetname;
static QStringList sheetnames;
static QList<int> xyrs;

static QStringList data;
static QList<int> align;
static QStringList format;
static QList<QXlsx::Format> formats;
static QList<bool> ifnum;

static int row, col, rws, cls, blen;

// bool, int, double, date, time, datetime, long
static QList<int> numTypes = {1,2,6,14,15,16,32};

// ---------------------------------------------------------------------
// define block
// if rws,cls not given, use 1,1
// if rws or cls is -1, use end of data
static bool defblock(QStringList blk)
{
  int n = blk.count();
  if ((n != 2) && (n != 4)) {
    error("expecting block of row,col or row,col,rws,cls");
    return false;
  }
  QList<int> p = qsl2intlist(blk);
  row = p[0];
  col = p[1];
  if (n == 2)     rws = cls = 1;
  else {
    rws = p[2];
    cls = p[3];
    if ((rws == -1) || cls == -1) {
      Worksheet* s = xlsx->currentWorksheet();
      CellRange d = s->dimension();
      if (rws == -1) rws = d.lastRow() - row;
      if (cls == -1) cls = d.lastColumn() - col;
    }
  }
  blen = rws*cls;
  return true;
}

// ---------------------------------------------------------------------
std::string errorlen(QString name, int n, bool ifcls=false)
{
  QString msg, txt;
  QString c1 = QString::number(n);
  QString c2 = QString::number(cls);
  QString c3 = QString::number(blen);
  if (blen == 1)
    msg = "only one " + name + " should be given";
  else if (ifcls || blen == cls) {
    txt = name + " count is %1, but should be 1 or %2";
    msg = QString(txt).arg(c1).arg(c2);
  } else {
    txt = name + " count is %1, but should be 1, %2 or %3";
    msg = QString(txt).arg(c1).arg(c2).arg(c3);
  }
  error(q2s(msg));
  return "";
}

// ---------------------------------------------------------------------
// convert J 0 1 2 3 to QXlsx 1 2 3 0
static int gethalign(int a)
{
  return (1 + a) % 4;
}

// ---------------------------------------------------------------------
std::string qsl2s(QStringList q)
{
  std::string r;
  int len = q.count();
  for (int i=0; i<len; i++) {
    r.append(q2s(q[i]));
    r.push_back('\0');
  }
  return r;
}

// ---------------------------------------------------------------------
// common settings for xlsx
void setcommon()
{
  xlsx->workbook()->setHtmlToRichStringEnabled();
}

// ---------------------------------------------------------------------
std::string xl(std::string p)
{
  if (p.size() == 0) {
    error("missing xl command");
    return "";
  }

  cmd.init(p);
  std::string type=cmd.getid();

  if (type=="close") return xlclose();
  if (type=="save") return xlsave();

  arg=cmd.qsplits();

  if (arg.size() == 0) {
    error("missing xl command parameter");
    return "";
  }

  if (type=="align") return xlalign();
  if (type=="block") return xlblock();
  if (type=="color") return xlcolor();
  if (type=="colwidth") return xlcolwidth();
  if (type=="data") return xldata();
  if (type=="format") return xlformat();
  if (type=="mergecells") return xlmergecells();
  if (type=="new") return xlnew();
  if (type=="openfile") return xlopenfile();
  if (type=="opensheet") return xlopensheet();
  if (type=="read") return xlread();
  if (type=="sheet") return xlsheet();
  if (type=="style") return xlstyle();
  return "";
}

// ---------------------------------------------------------------------
// 0 1 2 = left, center, right
// 3 = horizontal general (text left, numeric right)
std::string xlalign()
{
  int n = arg.count();
  if (!(n == 1 || n == cls || n == blen))
    return errorlen("align",n);

  QList<Format::HorizontalAlignment> halign;
  for (int i=0; i<n; i++) {
    int a = gethalign(arg[i].toInt());
    halign << static_cast<Format::HorizontalAlignment>(a);
  }

  for (int i=0; i<blen; i++)
    formats[i].setHorizontalAlignment(halign[i % n]);
  return "";
}

// ---------------------------------------------------------------------
std::string xlblock()
{
  if (!defblock(arg)) return "";
  formats.clear();
  formats.resize(blen);
  ifnum.clear();
  ifnum.resize(blen);
  return "";
}

// ---------------------------------------------------------------------
// needed?
std::string xlclose()
{
  filename="";
  sheetname="";
  formats.clear();
  ifnum.clear();
  return "";
}

// ---------------------------------------------------------------------
std::string xlcolor()
{
  int n = arg.count();
  if (!(n == 1 || n == cls || n == blen))
    return errorlen("color",n);
  for (int i=0; i<blen; i++)
    formats[i].setFontColor(arg[i % n]);
  return "";
}

// ---------------------------------------------------------------------
std::string xlcolwidth()
{
  int n = arg.count();
  if (!(n == 1 || n == cls))
    return errorlen("colwidth", n, true);

  for (int i=0; i<cls; i++) {
    QString w = (n == 1) ? arg[0] : arg[i];
    xlsx->setColumnWidth(col+1+i, w.toInt());
  }

  return "";
}

// ---------------------------------------------------------------------
std::string xldata()
{
  int n = arg.count();
  if (n != blen) {
    QString txt = "data count is %1, but should be %2";
    QString c1 = QString::number(n);
    QString c2 = QString::number(blen);
    QString msg = QString(txt).arg(c1).arg(c2);
    error(q2s(msg));
    return "";
  }
  data = arg;

  int r,c;
  for (int i=0; i<n; i++) {
    r = 1 + row + i / cls;
    c = 1 + col + i % cls;
    if (data[i].length()) {
      if (ifnum[i]) {
        xlsx->write(r,c,data[i].toDouble(),formats[i]);
      } else {
        xlsx->write(r,c,data[i], formats[i]);
      }
    }
  }
  return "";
}

// ---------------------------------------------------------------------
std::string xlformat()
{
  int n = arg.count();
  if (!(n == 1 || n == cls || n == blen)) {
    QString c1 = QString::number(n);
    QString c2 = QString::number(cls);
    QString c3 = QString::number(blen);
    QString txt = "format count is %1, but should be 1, %2 or %3";
    QString msg = QString(txt).arg(c1).arg(c2).arg(c3);
    error(q2s(msg));
    return "";
  }

  for (int i=0; i<blen; i++) {
    int ndx = i % n;
    if ( 0 < arg[ndx].size()) {
      formats[i].setNumberFormat(arg[ndx]);
      ifnum[i] = true;
    }
  }
  return "";
}

// ---------------------------------------------------------------------
// arg is row,col,rws,cls [,horizontal align] [,vertical align]
// default align is left, top
std::string xlmergecells()
{
  int n = arg.count();
  if ((n < 4) || n > 6) {
    std::string s = "expecting block of row,col,rws,cls";
    s += ", optionally followed by horizontal, vertical alignments";
    error(s);
    return "";
  }

  QList<int> p = qsl2intlist(arg);
  int row = p[0];
  int col = p[1];
  int rws = p[2];
  int cls = p[3];

// CellRange top left and bottom right
  CellRange r(row+1,col+1,row+rws,col+cls);

  int halign = (n == 5) ? p[4] : 1;
  int valign = (n == 6) ? p[5] : 0;
  Format f;
  f.setHorizontalAlignment(static_cast<Format::HorizontalAlignment>(halign));
  f.setVerticalAlignment(static_cast<Format::VerticalAlignment>(valign));

  xlsx->mergeCells(r,f);
  return "";
}

// ---------------------------------------------------------------------
std::string xlnew()
{
  if (arg.count() != 1) {
    error("expecting filename argument to xl new");
    return "";
  }
  filename=arg[0];
  delete xlsx;
  xlsx = new QXlsx::Document(filename);
  sheetnames.clear();
  setcommon();
  return "";
}

// ---------------------------------------------------------------------
// returns sheetnames delimited by zero
std::string xlopenfile()
{
  if (arg.count() != 1) {
    error("expecting filename argument to xl open");
    return "";
  }
  filename=arg[0];
  delete xlsx;
  xlsx = new QXlsx::Document(filename);

  if (!xlsx->load()) {
    qDebug() << "could not load " + filename;
    error("could not load " + q2s(filename));
    filename = "";
    return "";
  }

  setcommon();

  sheetnames = xlsx->sheetNames();
  if (sheetnames.count() == 0) {
    error("file has no worksheets");
    return "";
  }
  sheetname = sheetnames[0];
  return qsl2s(sheetnames);
}

// ---------------------------------------------------------------------
// returns topleft, rws, cls used
std::string xlopensheet()
{
  if (1 != arg.count()) {
    error("expecting sheet name argument");
    return "";
  }
  QString name = arg[0];
  qDebug() << sheetnames << name;
  if (sheetnames.contains(name)) {
    sheetname = name;
    xlsx->selectSheet(name);
    CellRange d = xlsx->dimension();
    int row = d.firstRow() - 1;
    int col = d.firstColumn() - 1;
    int rws = d.lastRow() - row;
    int cls = d.lastColumn() - col;
    xyrs = {row,col,rws,cls};
    qDebug() << "xyrs" << intlist2qs(xyrs);
    return q2s(intlist2qs(xyrs));
  } else {
    error("sheet name not found: " + q2s(name));
    sheetname = "";
    return "";
  }
}

// ---------------------------------------------------------------------
// arg is row, col [,rws,cls]
// returns del-delimited list:
// shape;typ;data...
// check decimal percentage currency scientific date&time
std::string xlread()
{
  if (!defblock(arg)) return "";

// !!! must check these work
  int r,c;
  char del = '\0';
  QString str;
  std::string shp, typ, res;
  shp += i2s(rws) + ' ' + i2s(cls) + del;
  for (int i=0; i<blen; i++) {
    r = 1 + row + i / cls;
    c = 1 + col + i % cls;
    auto cell = xlsx->cellAt(r,c);
    if (!cell) {
      typ.push_back('0');
      res.push_back(del);
      continue;
    }

    auto val = cell->value();
    //https://doc.qt.io/qt-6/qmetatype.html
    int tid = val.typeId();
    qDebug() << "tid" << tid << "val" << val;
    if (tid == 1) val = (val == true) ? 1 : 0;
    auto nam = val.typeName();
    qDebug() << "auto val" << val << nam << tid;
    typ.push_back(numTypes.contains(tid) ? '1' : '0');
    str = val.toString();
    res.append(q2s(str) + del);
  }

  return shp + typ + del  + res;
}

// ---------------------------------------------------------------------
std::string xlsave()
{
  //xlsx->saveAs("/home/chris/temp/test.xlsx");
  xlsx->saveAs(filename);
  return "";
}

// ---------------------------------------------------------------------
std::string xlsheet()
{
  if (1 != arg.count()) {
    error("expecting sheet name argument");
    return "";
  }
  QString name = arg[0];
  if (sheetnames.contains(name))
    xlsx->selectSheet(name);
  else {
    xlsx->addSheet(name);
    sheetnames.append(name);
  }

  return "";
}

// ---------------------------------------------------------------------
// 0 = normal
// 1 = bold
// 2 = italic
// 3 = bold + italic
std::string xlstyle()
{
  int n = arg.count();
  if (!(n == 1 || n == cls || n == blen))
    return errorlen("style",n);
  int s;

  for (int i=0; i<blen; i++) {
    s = arg[i % n].toInt();
    formats[i].setFontBold((s==1) || s == 3);
    formats[i].setFontItalic((s==2) || s == 3);
  }
  return "";
}
