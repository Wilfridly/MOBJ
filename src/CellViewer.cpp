#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QWidget>
#include "CellViewer.h"
#include "CellsLib.h"
#include "CellsModel.h"

namespace Netlist{

    CellViewer::CellViewer ( QWidget* parent )
        : QMainWindow       ( parent )
        , cellWidget_       ( NULL )
        , saveCellDialog_   ( NULL )
        , openCellDialog_   ( NULL )
        , cellsLib_         ( NULL )
        , instancesWidget_  ( NULL )
    {
        cellWidget_     =   new CellWidget      ();
        cellsLib_       =   new CellsLib        ( parent );
        instancesWidget_=   new InstancesWidget ( parent );
        saveCellDialog_ =   new SaveCellDialog ( this );
        openCellDialog_ =   new OpenCellDialog ( this );
        setCentralWidget ( cellWidget_ );

        instancesWidget_->setCellViewer(this);
        cellsLib_->setCellViewer(this);


        QMenu* fileMenu = menuBar()->addMenu ( "&File" );
        QAction* action =   new QAction ( "&Save As" , this );
        action->setStatusTip( "Save to disk (rename) the Cell" );
        action->setShortcut ( QKeySequence ("CTRL+S") );
        action->setVisible  ( true );
        fileMenu -> addAction ( action );
        connect( action, SIGNAL( triggered() ), this, SLOT( saveCell() ) );

        action = new QAction ( "&Quit", this );
        action->setStatusTip( "Exit the Netlist Viewer" );
        action->setShortcut ( QKeySequence ("CTRL+Q") );
        action->setVisible  ( true );
        fileMenu->addAction( action );
        connect( action , SIGNAL( triggered() ), this, SLOT( close () ) );

        action = new QAction ( "&Open file" , this );
        action->setStatusTip( "Open from disk (rename) the Cell" );
        action->setShortcut ( QKeySequence ("CTRL+O") );
        action->setVisible  ( true );
        fileMenu -> addAction ( action );
        connect( action, SIGNAL( triggered() ), this, SLOT( openCell() ) );

        fileMenu = menuBar()->addMenu ( "&Chargement" );
        action = new QAction ( "&Cells", this );
        action->setStatusTip( "Open the cell" );
        action->setShortcut ( QKeySequence ("CTRL+C") );
        action->setVisible  ( true );
        fileMenu -> addAction ( action );
        connect( action, SIGNAL( triggered() ), this, SLOT( showCellsLib() ) );

        action = new QAction ( "&Instances", this );
        action->setStatusTip( "Open the instance" );
        action->setShortcut ( QKeySequence ("CTRL+V") );
        action->setVisible  ( true );
        fileMenu -> addAction ( action );
        connect( action, SIGNAL( triggered() ), this, SLOT( showInstancesWidget() ) );

    }

    CellViewer::~CellViewer(){
        delete cellWidget_;
        delete cellsLib_;
        delete instancesWidget_;
        delete saveCellDialog_;
        delete openCellDialog_;
    }

    Cell* CellViewer::getCell () const{
        return cellWidget_->getCell();
    }

    void CellViewer::setCell(Cell* cell){
        cellWidget_->setCell(cell);
        instancesWidget_->setCell(cell);
        cellsLib_->getBaseModel()->setCell(cell);
    }

    void CellViewer::openCell(){ //TME9/10 Else à faire
        std::string name = openCellDialog_->run();

        if(name.empty()) return;
        Cell* cell = Cell::find(name);

        if(cell == NULL){
            cell = Cell::load(name);
            if(cell == NULL){
                std::cerr << "Cell pas trouvé" << std::endl;
            }
        }
        else{
            setCell(cell);
            return;
        }
        return;
    }

    void CellViewer::saveCell (){
        Cell* cell = getCell();
        if (cell == NULL) 
            return ;
        QString cellName = cell->getName().c_str();

        if(saveCellDialog_->run(cellName)) {
            cell->setName(cellName.toStdString());
            cell->save   (cellName.toStdString());
        }
    }

    void CellViewer::showCellsLib(){
        return cellsLib_->show();
    }

    void CellViewer::showInstancesWidget(){
        return instancesWidget_->show();
    }
}