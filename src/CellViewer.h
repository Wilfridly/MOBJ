#ifndef NETLIST_CELL_VIEWER_H
#define NETLIST_CELL_VIEWER_H

#include <QMainWindow>
#include "Cell.h"
#include "CellWidget.h"
#include "CellsLib.h"
#include "InstancesModel.h"
#include "InstancesWidget.h"
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"


namespace Netlist{

    class CellViewer : public QMainWindow{
            Q_OBJECT;
        public:
                                CellViewer      ( QWidget* parent = NULL);
            virtual             ~CellViewer     ();
            Cell*               getCell         ()const;
            inline CellsLib*    getCellsLib     (){return cellsLib_;}
            inline CellWidget*  getCellWidget   (){return cellWidget_;}
        public slots:
            void setCell            (Cell*);
            void saveCell           ();
            void openCell           ();
            void showCellsLib       ();
            void showInstancesWidget();
        signals:
            void cellLoaded(); //généré par moc : emetteur
        private:
            CellWidget*         cellWidget_;
            CellsLib*           cellsLib_;
            InstancesWidget*    instancesWidget_;
            SaveCellDialog*     saveCellDialog_;
            OpenCellDialog*     openCellDialog_;
    };
}

#endif
