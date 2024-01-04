#ifndef NETLIST_OPEN_CELL_DIALOG_H
#define NETLIST_OPEN_CELL_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <string>

namespace Netlist{
    class Cell;

    class OpenCellDialog : public QDialog {
        Q_OBJECT;
    public:
                            OpenCellDialog( QWidget* parent=NULL );
                std::string run();
        const   QString     getCellName() const;
        void                setCellName ( const QString& );
    protected:
        QLineEdit* lineEdit_ ;
    };
}

#endif