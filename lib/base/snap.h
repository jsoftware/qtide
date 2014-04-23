#ifndef SNAP_H
#define SNAP_H

void snap_init();
QString snapgetpath(QString);
QString snappath(QString);
bool snaprmdir(QString path);
void snapshot(bool force, QString path);
void snapshot_tree(QString folder);
QString ss_date();
void ss_info(QString s);
QStringList ss_list(QString s);
bool ss_mkdir(QString d);

#endif
