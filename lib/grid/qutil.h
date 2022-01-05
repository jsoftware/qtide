#ifndef QUTIL_H
#define QUTIL_H

#include <QStringList>

// using namespace std;

int getcellcount(int,int,QVector<int>);
int getindexof(int w, QVector<int> p);
bool griderror(QString s);

QStringList expandstringlist(QStringList,QVector<int>);
QVector<int> expandvectorint(QVector<int>,QVector<int>);

QVector<int> hdrmergeindex(QVector<int>);
QVector<int> hdrmergemask(QVector<int>);

QVector<int> qlist2ints(QStringList);
bool validmerge(int,QVector<int>);
int viewlength(QVector<int>d, int s, int w);

int maxscroll(int,QVector<int>);
int roundint(double d);
void timex(QString id);

QVector<int> mcolmax(int,QVector<int>);
QVector<int> mrowmax(int,QVector<int>);
QVector<int> mcolsum(int,QVector<int>);
QVector<int> mrowsum(int,QVector<int>);

int vindex(int,QVector<int>);
int vsum(int, QVector<int>);
std::string v2s(QVector<int>);

QVector<int> vadd(int, QVector<int>);
QVector<int> vmax(int, QVector<int>);
QVector<int> vmax(QVector<int>,QVector<int>);
QVector<int> vmult(QVector<int>,QVector<int>);
QVector<int> vmult(int m, QVector<int> v);
QVector<int> vshape(int, QVector<int>);
QVector<int> vsumscan(int, QVector<int>);
QVector<int> vsumscanp(int, QVector<int>);

#endif
