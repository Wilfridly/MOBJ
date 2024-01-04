#ifndef NETLIST_INSTANCES_WIDGET_H
#define NETLIST_INSTANCES_WIDGET_H

#include <QTableView>
#include <QPushButton>
#include "InstancesWidget.h"
#include "InstancesModel.h"
#include <iostream>

namespace Netlist{
  
  class Cell;
  class CellViewer;

  class InstancesWidget : public QWidget {
      Q_OBJECT;
    public:
                    InstancesWidget ( QWidget* parent=NULL );
              void  setCellViewer   ( CellViewer* );
              int   getSelectedRow  () const;
      inline  void  setCell         ( Cell* cell){ return baseModel_->setCell(cell); }
    public slots:
              void  load            ();
    private:
              CellViewer*     cellViewer_;
              InstancesModel* baseModel_;
              QTableView*     view_;
              QPushButton*    load_;
  };

}
#endif