#include <QtWidgets>
#include <QtTest>
#include "../../lib/wd/table.h"
#include "../../lib/wd/form.h"
#include "../../lib/base/state.h"

// Exercises the real Table implementation and its live Qt widget tree. The
// closed form prevents dispatch into an absent J engine, not Table callbacks.
class TableSortTest : public QObject
{
  Q_OBJECT
  Form *form;
  Table *table;
  QTableWidget *view;

  void sort(Qt::SortOrder order) { table->sortrows(0,order); }
  void clickHeader(int column) {
    QHeaderView *h=view->horizontalHeader();
    QTest::mouseClick(h->viewport(),Qt::LeftButton,Qt::NoModifier,
                     QPoint(h->sectionViewportPosition(column)+20,h->height()/2));
    QCoreApplication::processEvents();
  }
  void populate() {
    table->set("hdr","Key ID Value");
    table->set("lab","rowB rowA rowC");
    table->set("data","B idB valB A idA valA C idC valC");
    view->resize(600,220);
    view->show();
    QCoreApplication::processEvents();
  }
private slots:
  void init() {
    NoEvents=true;
    ShowIde=true;
    form=new Form("test","","base");
    form->closed=true;
    table=new Table("t","3 3",form,form->pane);
    view=static_cast<QTableWidget*>(table->widget);
    populate();
  }
  void cleanup() {
    NoEvents=true;
    delete table;
    delete form;
  }
  void initialCoordinatesAndUnpopulatedTable() {
    delete table;
    table=new Table("empty","",form,form->pane);
    view=static_cast<QTableWidget*>(table->widget);
    QCOMPARE(table->row,-1); QCOMPARE(table->col,-1);
    QCOMPARE(table->lastrow,-1); QCOMPARE(table->lastcol,-1);
    sort(Qt::AscendingOrder);
    table->set("shape","3 3");
    sort(Qt::DescendingOrder);
    QCOMPARE(table->row,-1); QCOMPARE(table->col,-1);
    QCOMPARE(table->lastrow,-1); QCOMPARE(table->lastcol,-1);
    QCOMPARE(view->currentRow(),-1);
  }
  void signalSuppressionAndRestoration() {
    auto *header=view->verticalHeader();
    view->setCurrentCell(1,1);
    view->setRowHeight(0,51); view->setRowHeight(1,37);
    view->setRowHidden(2,true);
    QSignalSpy changed(view,&QTableWidget::cellChanged);
    QSignalSpy current(view,&QTableWidget::currentCellChanged);
    QSignalSpy resized(header,&QHeaderView::sectionResized);
    QSignalSpy layouts(view->model(),&QAbstractItemModel::layoutChanged);
    QVERIFY(changed.isValid() && current.isValid() && resized.isValid() && layouts.isValid());
    int modelCallbacks=0;
    connect(view->model(),&QAbstractItemModel::layoutAboutToBeChanged,this,[&]() {
      ++modelCallbacks;
      QVERIFY(view->signalsBlocked());
      QVERIFY(header->signalsBlocked());
      QVERIFY(!view->model()->signalsBlocked());
      QVERIFY(!view->selectionModel()->signalsBlocked());
    });
    NoEvents=false;
    table->event="before";
    for (int flags=0; flags<4; ++flags) {
      view->blockSignals(flags & 1);
      header->blockSignals(flags & 2);
      sort(flags%2 ? Qt::DescendingOrder : Qt::AscendingOrder);
      QCOMPARE(view->signalsBlocked(),bool(flags & 1));
      QCOMPARE(header->signalsBlocked(),bool(flags & 2));
      QCOMPARE(table->event,std::string("before"));
      QCOMPARE(view->item(view->currentRow(),1)->text(),QString("idA"));
    }
    QCOMPARE(modelCallbacks,4); QCOMPARE(layouts.count(),4);
    QCOMPARE(changed.count(),0); QCOMPARE(current.count(),0); QCOMPARE(resized.count(),0);
    view->blockSignals(false); header->blockSignals(false);
    view->setCurrentCell(1,1);
    view->item(1,1)->setText("changed after sort");
    header->resizeSection(1,59);
    QCOMPARE(current.count(),1); QCOMPARE(changed.count(),1); QCOMPARE(resized.count(),1);
  }
  void invalidSortColumnsAreNoOps() {
    auto *label=view->verticalHeaderItem(0);
    QSignalSpy layouts(view->model(),&QAbstractItemModel::layoutChanged);
    table->hoverrow=0; table->hovercol=1;
    table->sortrows(-1,Qt::AscendingOrder);
    table->sortrows(view->columnCount(),Qt::DescendingOrder);
    QCOMPARE(layouts.count(),0);
    QCOMPARE(view->item(0,1)->text(),QString("idB"));
    QCOMPARE(view->verticalHeaderItem(0),label);
    QCOMPARE(table->hoverrow,0); QCOMPARE(table->hovercol,1);
    QVERIFY(!view->signalsBlocked()); QVERIFY(!view->verticalHeader()->signalsBlocked());
  }
  void nonzeroSortColumnAndPermutation() {
    table->set("shape","7 4");
    table->set("lab","r0 r1 r2 r3 r4 r5 r6");
    for (int r=0; r<7; ++r) {
      table->set("cell",std::to_string(r)+" 0 "+std::to_string(r));
      table->set("cell",std::to_string(r)+" 2 "+std::to_string((r*3)%7));
      table->set("block",std::to_string(r)+" 3");
      table->set("align",std::to_string(r%3));
      table->set("protect",std::to_string(r%2));
    }
    table->set("block","");
    const QVector<int> align=table->cellalign, protect=table->cellprotect;
    for (int i=0; i<6; ++i) {
      table->sortrows(2,i%2 ? Qt::DescendingOrder : Qt::AscendingOrder);
      QSet<int> seen;
      for (int r=0; r<7; ++r) {
        const int old=view->item(r,0)->text().toInt();
        QVERIFY(old>=0 && old<7); QVERIFY(!seen.contains(old)); seen.insert(old);
        QCOMPARE(view->verticalHeaderItem(r)->text(),QString("r%1").arg(old));
        QCOMPARE(view->item(r,2)->text().toInt(),i%2 ? 6-r : r);
        for (int c=0; c<4; ++c) {
          QCOMPARE(table->cellalign[r*4+c],align[old*4+c]);
          QCOMPARE(table->defcellalign[r*4+c],align[old*4+c]);
          QCOMPARE(table->cellprotect[r*4+c],protect[old*4+c]);
          QCOMPARE(table->defcellprotect[r*4+c],protect[old*4+c]);
        }
      }
      QCOMPARE(seen.size(),7);
    }
  }
  void missingKeysAndPartialLabels() {
    table->set("shape","0 3"); table->set("shape","5 3");
    for (int r=0; r<5; ++r)
      view->setItem(r,1,new QTableWidgetItem(QString::number(r)));
    for (int i=0; i<2; ++i) {
      sort(i ? Qt::DescendingOrder : Qt::AscendingOrder);
      for (int r=0; r<5; ++r) {
        QCOMPARE(view->item(r,1)->text(),QString::number(r));
        QVERIFY(!view->verticalHeaderItem(r));
      }
    }
    auto *label=new QTableWidgetItem("record 0");
    view->setVerticalHeaderItem(0,label);
    view->setItem(0,0,new QTableWidgetItem("B"));
    view->setItem(1,0,new QTableWidgetItem("A"));
    sort(Qt::AscendingOrder);
    QCOMPARE(view->verticalHeaderItem(1),label);
    QVERIFY(!view->verticalHeaderItem(0));
    for (int r=2; r<5; ++r) QVERIFY(!view->verticalHeaderItem(r));
  }
  void reshapeThenRepeatedSorts() {
    table->set("shape","2 2");
    sort(Qt::AscendingOrder);
    table->set("shape","4 4");
    table->set("lab","r0 r1 r2 r3");
    table->set("data","D idD x y B idB x y A idA x y C idC x y");
    table->set("block","1 3"); table->set("type","100"); table->set("cell","1 3 1");
    table->set("block","");
    QWidget *check=table->cellwidget[7]; QVERIFY(check);
    for (int i=0; i<4; ++i) {
      sort(i%2 ? Qt::DescendingOrder : Qt::AscendingOrder);
      const int br=i%2 ? 2 : 1;
      QCOMPARE(view->item(br,1)->text(),QString("idB"));
      QCOMPARE(view->verticalHeaderItem(br)->text(),QString("r1"));
      QCOMPARE(table->cellwidget[br*4+3],check);
      QCOMPARE(table->celltype[br*4+3],100); QCOMPARE(table->defcelltype[br*4+3],100);
      QCOMPARE(check->objectName(),QString::number(br*4+3));
      QCOMPARE(table->cellwidget.size(),16);
    }
  }
  void labelsPropertiesAndRepeatedSorts() {
    QTableWidgetItem *label=view->verticalHeaderItem(0);
    label->setToolTip("header B"); label->setBackground(Qt::red);
    view->item(0,1)->setToolTip("cell B");
    view->item(0,1)->setForeground(Qt::blue);
    QFont f=view->font(); f.setBold(true); view->item(0,1)->setFont(f);
    view->setRowHeight(0,51);
    view->setRowHeight(2,63); view->setRowHidden(2,true);
    for (int i=0;i<6;++i) {
      sort(i%2 ? Qt::DescendingOrder : Qt::AscendingOrder);
      QCOMPARE(view->verticalHeaderItem(1),label);
      QCOMPARE(label->toolTip(),QString("header B"));
      QCOMPARE(label->background().color(),QColor(Qt::red));
      QCOMPARE(view->item(1,1)->text(),QString("idB"));
      QCOMPARE(view->item(1,1)->toolTip(),QString("cell B"));
      QCOMPARE(view->item(1,1)->foreground().color(),QColor(Qt::blue));
      QVERIFY(view->item(1,1)->font().bold());
      QCOMPARE(view->rowHeight(1),51);
      int cr=i%2 ? 0 : 2;
      QVERIFY(view->isRowHidden(cr));
      view->setRowHidden(cr,false); QCOMPARE(view->rowHeight(cr),63);
      view->setRowHidden(cr,true);
      QCOMPARE(view->verticalHeaderItem(0)->text(),i%2 ? QString("rowC") : QString("rowA"));
      if (i==0 && !qgetenv("JQT_SORT_SCREENSHOT").isEmpty())
        QVERIFY(view->grab().save(QString::fromLocal8Bit(qgetenv("JQT_SORT_SCREENSHOT"))));
    }
  }
  void metadataAndSubsequentCreation() {
    table->set("block","0 2"); table->set("align","2"); table->set("protect","1");
    table->set("block","");
    sort(Qt::AscendingOrder);
    QCOMPARE(table->cellalign[5],2); QCOMPARE(table->defcellalign[5],2);
    QCOMPARE(table->cellprotect[5],1); QCOMPARE(table->defcellprotect[5],1);
    delete view->takeItem(1,2);
    table->set("cell","1 2 recreated");
    QVERIFY(view->item(1,2)->textAlignment() & Qt::AlignRight);
    QVERIFY(!(view->item(1,2)->flags() & Qt::ItemIsEditable));
    table->set("shape","4 3");
    QCOMPARE(table->cellalign[5],2); QCOMPARE(table->defcellprotect[5],1);
  }
  void duplicateAndMissingKeys() {
    table->set("shape","5 3");
    table->set("lab","r0 r1 r2 r3 r4");
    for (int r=0;r<5;++r) view->setItem(r,1,new QTableWidgetItem(QString::number(r)));
    view->item(0,0)->setText("X"); view->item(1,0)->setText("X");
    view->item(2,0)->setText(""); // An empty string differs from a missing item.
    for (int i=0;i<4;++i) {
      sort(i%2 ? Qt::DescendingOrder : Qt::AscendingOrder);
      for (int r=0;r<5;++r)
        QCOMPARE(view->verticalHeaderItem(r)->text(),"r"+view->item(r,1)->text());
      QCOMPARE(view->item(3,1)->text(),QString("3"));
      QCOMPARE(view->item(4,1)->text(),QString("4"));
      int first=i%2 ? 0 : 1;
      QCOMPARE(view->item(first,1)->text(),QString("0"));
      QCOMPARE(view->item(first+1,1)->text(),QString("1"));
    }
  }
  void selectionAndCachedCoordinates() {
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    view->setCurrentCell(1,1); // A moves between rows 0 and 2 on every toggle.
    view->clearSelection(); view->item(0,1)->setSelected(true); view->item(2,2)->setSelected(true);
    table->row=0; table->col=1; table->lastrow=2; table->markrow=0;
    table->hoverrow=0; table->hovercol=1;
    table->set("block","0 1");
    for (int i=0;i<4;++i) {
      sort(i%2 ? Qt::DescendingOrder : Qt::AscendingOrder);
      QCOMPARE(view->currentRow(),i%2 ? 2 : 0); QCOMPARE(view->currentColumn(),1);
      QVERIFY(view->item(1,1)->isSelected());
      QVERIFY(view->item(i%2 ? 0 : 2,2)->isSelected());
      QCOMPARE(view->selectedItems().size(),2);
      QCOMPARE(table->row,1); QCOMPARE(table->markrow,1);
      QCOMPARE(table->lastrow,i%2 ? 0 : 2);
      QCOMPARE(table->hoverrow,-1); QCOMPARE(table->hovercol,-1);
      QCOMPARE(table->row1,0); QCOMPARE(table->col1,1);
    }
  }
  void mixedWidgetsAndCallbacks() {
    table->set("block","0 2"); table->set("type","100"); table->set("cell","0 2 1");
    table->set("block","1 2"); table->set("type","200"); table->set("cell","1 2 \"0 red green\"");
    table->set("block","2 2"); table->set("type","400"); table->set("cell","2 2 Push");
    table->set("block","");
    QWidget *checkContainer=view->cellWidget(0,2);
    auto *check=qobject_cast<QCheckBox*>(table->cellwidget[2]);
    auto *combo=qobject_cast<QComboBox*>(table->cellwidget[5]);
    auto *button=qobject_cast<QPushButton*>(table->cellwidget[8]);
    QVERIFY(check && combo && button);
    check->setToolTip("widget B");
    for (int i=0;i<4;++i) {
      NoEvents=true;
      sort(i%2 ? Qt::DescendingOrder : Qt::AscendingOrder);
      int ar=i%2 ? 2 : 0, cr=i%2 ? 0 : 2;
      QCOMPARE(view->cellWidget(1,2),checkContainer);
      QCOMPARE(table->cellwidget[5],check);
      QCOMPARE(table->celltype[5],100); QCOMPARE(table->defcelltype[5],100);
      QCOMPARE(table->celltype[ar*3+2],200); QCOMPARE(table->celltype[cr*3+2],400);
      QCOMPARE(check->objectName(),QString("5"));
      QCOMPARE(check->toolTip(),QString("widget B"));
      QCOMPARE(table->get("cell","1 2"),check->isChecked() ? std::string("1") : std::string("0"));
      NoEvents=false;
      QTest::mouseClick(check,Qt::LeftButton,Qt::NoModifier,QPoint(5,check->height()/2));
      QCOMPARE(table->row,1); QCOMPARE(table->col,2); QCOMPARE(table->event,std::string("change"));
      combo->setCurrentIndex(1-combo->currentIndex());
      QCOMPARE(table->row,ar); QCOMPARE(table->col,2);
      QCOMPARE(table->readcell(ar,2),std::to_string(combo->currentIndex()));
      QTest::mouseClick(button,Qt::LeftButton);
      QCOMPARE(table->row,cr); QCOMPARE(table->col,2); QCOMPARE(table->event,std::string("clicked"));
      NoEvents=true;
      table->set("cell",std::to_string(ar)+" 2 0");
      QCOMPARE(combo->currentIndex(),0);
    }
  }
  void textEditorAndEditableCombo() {
    table->set("block","0 2"); table->set("type","10"); table->set("cell","0 2 notes");
    table->set("block","1 2"); table->set("type","300"); table->set("cell","1 2 \"0 red green\"");
    QWidget *editor=table->cellwidget[2]; QWidget *combo=table->cellwidget[5];
    sort(Qt::AscendingOrder);
    QCOMPARE(table->cellwidget[5],editor); QCOMPARE(table->cellwidget[2],combo);
    QCOMPARE(table->get("cell","1 2"),std::string("notes"));
    QCOMPARE(table->get("cell","0 2"),std::string("red"));
    table->set("cell","1 2 changed");
    QCOMPARE(table->get("cell","1 2"),std::string("changed"));
    QCOMPARE(editor->objectName(),QString("5"));
  }
  void nativeHeaderAndEventMode() {
    table->set("headerclick","event");
    NoEvents=false;
    clickHeader(0);
    QCOMPARE(view->item(0,0)->text(),QString("B"));
    QCOMPARE(view->verticalHeaderItem(0)->text(),QString("rowB"));
    QCOMPARE(table->event,std::string("hdrclick"));
    table->set("sortindicator","none"); table->set("headerclick","native");
    clickHeader(0);
    QCOMPARE(view->item(0,0)->text(),QString("A"));
    QCOMPARE(view->verticalHeaderItem(0)->text(),QString("rowA"));
    QCOMPARE(table->event,std::string("hdrclick"));
    QCOMPARE(table->row,-1);
    clickHeader(0);
    QCOMPARE(view->item(0,0)->text(),QString("C"));
    QCOMPARE(view->verticalHeaderItem(0)->text(),QString("rowC"));
  }
  void stockAndLegacyRoutesAndEmptyTables() {
    table->event="before";
    NoEvents=false;
    table->set("sort","0 descending");
    QCOMPARE(table->event,std::string("before"));
    QCOMPARE(view->verticalHeaderItem(0)->text(),QString("rowC"));
    table->on_headerClicked(0);
    QCOMPARE(table->event,std::string("before"));
    QCOMPARE(view->verticalHeaderItem(0)->text(),QString("rowA"));
    NoEvents=true;
    table->set("shape","0 3"); sort(Qt::AscendingOrder);
    table->set("shape","3 0"); sort(Qt::DescendingOrder);
    table->set("shape","1 1");
    table->set("lab","single"); sort(Qt::AscendingOrder);
    QCOMPARE(view->verticalHeaderItem(0)->text(),QString("single"));
    sort(Qt::DescendingOrder);
    QCOMPARE(view->verticalHeaderItem(0)->text(),QString("single"));
  }
};

QTEST_MAIN(TableSortTest)
#include "table_sort_test.moc"
