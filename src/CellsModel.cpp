#include "Cell.h"
#include "CellsModel.h"

namespace Netlist{

    CellsModel::CellsModel( QObject * parent )
        : QAbstractTableModel ( parent )
        , cell_ ( NULL ){ 

    }

    CellsModel::~CellsModel(){
    }

    void CellsModel::setCell( Cell* cell ){
        emit layoutAboutToBeChanged();
        cell_ = cell;
        emit layoutChanged();
    }

    int CellsModel::rowCount( const QModelIndex& parent ) const{
        return ( cell_ ) ? Cell::getAllCells().size ():0; 
    
    }
    int CellsModel::columnCount( const QModelIndex& parent ) const{
        return 1; 
    }

    QVariant CellsModel::data( const QModelIndex& index, int role ) const{
        if ( not cell_ or not index.isValid ()) return QVariant();
        if ( role == Qt::DisplayRole ) {
            int row = index.row();
            switch (index.column ()){
                case 0: return Cell::getAllCells()[ row ]->getName().c_str();
            }
        }
        return QVariant ();
    }
    QVariant CellsModel::headerData ( int section
                                        , Qt::Orientation orientation
                                        , int role ) const{
        if ( orientation == Qt::Vertical ) return QVariant ();
        if ( role != Qt::DisplayRole ) return QVariant ();
        switch ( section ) {
            case 0: return "Cell" ;
        }
        return QVariant ();
    }

    Cell * CellsModel::getModel ( int row ){
        if ( not cell_ ) return NULL ;
        if ( row >= ( int ) Cell::getAllCells().size()) return NULL ;
        return Cell::getAllCells()[ row ];
    }
}