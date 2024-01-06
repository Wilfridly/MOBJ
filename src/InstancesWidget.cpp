#include <QHeaderView>
#include <QLayout>
#include "InstancesWidget.h"
#include "InstancesModel.h"
#include "CellViewer.h"
#include "Cell.h"

namespace Netlist{
    InstancesWidget :: InstancesWidget ( QWidget * parent )
        : QWidget ( parent )
        , cellViewer_ ( NULL )
        , baseModel_ ( new InstancesModel ( this ))
        , view_ ( new QTableView ( this ))
        , load_ ( new QPushButton ( this ))
    {
        setAttribute ( Qt::WA_QuitOnClose , false );
        setAttribute ( Qt::WA_DeleteOnClose , false );
        setContextMenuPolicy ( Qt::ActionsContextMenu );
        QVBoxLayout* layout = new QVBoxLayout(this); //gère l'affichage

        view_ -> setShowGrid ( false );
        view_ -> setAlternatingRowColors ( true );
        view_ -> setSelectionBehavior ( QAbstractItemView::SelectRows );
        view_ -> setSelectionMode ( QAbstractItemView::SingleSelection );
        view_ -> setSortingEnabled ( true );
        view_ -> setModel ( baseModel_ ); // On associe le modele .

        QHeaderView * horizontalHeader = view_->horizontalHeader();
        horizontalHeader ->setDefaultAlignment ( Qt::AlignHCenter );
        horizontalHeader ->setMinimumSectionSize ( 300 );
        horizontalHeader ->setStretchLastSection ( true );
        QHeaderView * verticalHeader = view_ ->verticalHeader();
        verticalHeader -> setVisible ( false );
        load_ -> setText ( "Load" );

        layout->addWidget(view_); //Ajout des widgets
        layout->addWidget(load_);
        connect(load_, SIGNAL( clicked () ), this, SLOT( load () ));
    }

    void InstancesWidget::setCellViewer(CellViewer* cellviewer){
        cellViewer_ = cellviewer;
    }

    int InstancesWidget::getSelectedRow () const{
        QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
        if ( selecteds.empty ()) 
            return -1;
        return selecteds.first().row();
    }

    void InstancesWidget::load(){
        int selectedRow = getSelectedRow ();
        if ( selectedRow < 0) return ;
        cellViewer_ -> setCell( baseModel_->getModel( selectedRow ) );
    }
}
