#ifndef NETLIST_CELLS_LIB_H
#define NETLIST_CELLS_LIB_H

#include <QWidget>
#include <QPushButton>
#include <QTableView>

namespace Netlist{
    class Cell;
    class CellViewer;
    class CellsModel;

    class CellsLib : public QWidget {
        Q_OBJECT;
    public:
                            CellsLib       ( QWidget* parent=NULL );
            void            setCellViewer  ( CellViewer* );
            int             getSelectedRow () const;
        inline CellsModel*  getBaseModel   (){return baseModel_;}
    public slots:
            void        load           ();
    private:
        CellViewer*  cellViewer_;
        CellsModel*  baseModel_;
        QTableView*  view_;
        QPushButton* load_;
    };
}

#endif