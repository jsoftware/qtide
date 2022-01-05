#ifndef WGRID_H
#define WGRID_H

#include <QFont>
#include <QPainter>
#include <QWidget>

#include "../base/base.h"
#include "qgrid.h"
#include "qutil.h"
#include "../wd/child.h"

// using namespace std;

class QGrid;

// ---------------------------------------------------------------------
class WGrid : public QWidget
{
  Q_OBJECT

public:
  WGrid(Child *,QGrid*,QWidget *,Wasa *);

  int axislength(QList<int> a);
  QVector<int> axismerge(QList<int> a);
  QStringList axisrep(QStringList a,int p);
  QVector<int> axisreps(QList<int> a);

  void defs();
  void draw();
  bool getgrid();
  void maxscrollc(int);
  void maxscrollr(int);
  void set(std::string,std::string);
  std::string state(std::string);

  bool WGridInit;
  Child *pchild;
  Form *pform;
  QGrid *p;
  int row,col;

protected:
  void mouseDoubleClickEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void paintEvent(QPaintEvent *event);

public slots:
  void barhchanged(int);
  void barvchanged(int);

private:
  bool drawinit();

  void drawcells();
  void drawcellback();
  void drawcelllines();
  void drawcelltext();
  void drawheaders();
  void drawheaderback();
  void drawheaderlines();
  void drawheadermerge();
  void drawheadertext();
  void drawlabels();
  void drawlabelback();
  void drawlabelmerge();
  void drawlabellines();
  void drawlabeltext();
  void drawmergesizes();
  void drawpixmaps();
  void drawsizes();
  void drawview();
  void drawsizes(int,int,int,int);
  void drawtop();

  void getcellpos(QPoint p);
  void hdrmergesizes();
  void labmergesizes();

  void setdataalign(QStringList s);
  void setheader(QStringList s);
  void setheaderalign(QStringList s);
  void setheadermerge(QStringList s);
  void setlabel(QStringList s);
  void setlabelalign(QStringList s);
  void setlabelmerge(QStringList s);
  void setshape(QStringList s);

  int minwidth, minheight, sbx;
  int Crws, Ccls, Hrws, Lcls;
  int Cw, Ch, Gw, Gh, Lw, Hh;
  int Mx, My, Mw, Mh;

  QStringList Data, Hdr, HdrX, Lab, LabX;
  QVector<int> DataAlign;
  QVector<int> DefAlign, DefMerge;
  QVector<int> HdrAlign, HdrAlignX, LabAlign, LabAlignX;
  QVector<int> HdrMerge, HdrMergeIndex, HdrMergeMask;
  QVector<int> LabMerge, LabMergeIndex, LabMergeMask;

  QVector<int> Cws, Chs, Cwx, Chx;
  QVector<int> Dws, Dhs, Hws, Hhs, Lws, Lhs;
  QVector<int> Dwx, Dhx, Hhx, Lwx, Twx, Thx;
  QVector<int> Cwp, Chp, Gws, Ghs, Gwp, Ghp;
  QVector<bool> Dfm, Hfm, Lfm;

  int Scls, Srws, Sclsmax, Srwsmax;
  int Vcls, Vrws, Vw, Vh;
  QVector<int> Vws, Vhs, Vwp, Vhp;

  QFont font;
  Wasa *sa;

  QColor CellColorAlt;
  QColor CellBack;
  QColor CellBackAlt;
  QColor CellText;
  QColor GridBorder;
  QColor GridLight;
  QColor GridLines;
  QColor GridMark;
  QColor GridShadow;
  QColor HdrBack;

  QPainter *painter;
  QPixmap *pmall;
  QPixmap *pmtop;
  QPixmap *pmhdr;
  QPixmap *pmlab;
  QPixmap *pmcell;
};

#endif
