#ifndef SVGVIEW2_H
#define SVGVIEW2_H

#include <string>
#include <QWidget>
#include <QString>
#include <QSvgRenderer>
#include <QSize>
#include <QMouseEvent>
#include <QSlider>
#include <QPushButton>
#include <QPainter>
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#include <QtPrintSupport/QPrintPreviewDialog>
#else
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintPreviewDialog>
#endif
#endif

#include "child.h"

class QPrinter;
class QPainter;


class SvgView2 : public QWidget
{
  Q_OBJECT
public:
  SvgView2(Child *c, QWidget *parent = 0);
  virtual ~SvgView2();
  void paintEvent(QPaintEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void resizeEvent(QResizeEvent *event);
  void focusInEvent(QFocusEvent *event);
  void focusOutEvent(QFocusEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void setFile(const QString& filePath);
  void setXml(const std::string & contents);
  void showZoom(bool v);
  void printSVG();
  void saveasSVG(const QString& filePath);
  int getZoom();
  QPoint getOrigin();
  void setOrigin(int x,int y);

  std::string type;
  bool m_showzoom;

public slots:
  void setZoom(int); // 100 <= newZoom < 0

private:
  QSvgRenderer* m_renderer;
  QSlider* m_zoomSlider;
  QSize m_imageSize;
  qreal m_zoomLevel;
  qreal m_imageScale; // How many Image coords 1 widget pixel is worth

  QRectF m_viewBox;
  QRectF m_viewBoxBounds;
  QSizeF m_viewBoxSize;
  QPointF m_viewBoxCenter;
  QPointF m_viewBoxCenterOnMousePress;
  QPoint m_mousePress;

  QPoint m_origin;
  QPoint m_lastOrigin;
  QPoint m_mouseLastPos;
  bool m_mouseDown;

  void updateImageScale();
  QRectF getViewBox(QPointF viewBoxCenter);

  void buttonEvent(QEvent::Type type, QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  Child *pchild;
  void setFini();

};



#endif
