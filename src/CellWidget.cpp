// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  <QLine>
#include  <QRect>
#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"
#include  "Box.h"


namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  CellWidget::CellWidget ( QWidget* parent )
    : QWidget   (parent)
    , cell_     (NULL)
    , viewport_ (Box(0,0,500,500))
  {
    setAttribute    ( Qt::WA_OpaquePaintEvent );
    setAttribute    ( Qt::WA_NoSystemBackground );
    setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
    setFocusPolicy  ( Qt::StrongFocus );
    setMouseTracking( true );
  }


  CellWidget::~CellWidget (){
    delete cell_;
  }


  void  CellWidget::setCell ( Cell* cell ){
    cell_ = cell;
    repaint();
  }


  QSize  CellWidget::minimumSizeHint () const{
    return QSize(500,500); 
  }


  void  CellWidget::resizeEvent ( QResizeEvent* event ){
    const QSize & size = event -> size ();
    // Assume the resize is always done by drawing the bottom right corner .
    viewport_.setX2( viewport_.getX1 () + size.width  () );
    viewport_.setY1( viewport_.getY2 () - size.height () );
    cerr << " CellWidget :: resizeEvent () ␣ viewport_ : " << viewport_ << endl ;
 }

 void CellWidget :: keyPressEvent ( QKeyEvent * event ) {
    event->ignore();
    if ( event->modifiers() & ( Qt::ControlModifier | Qt::ShiftModifier )) return ;
    switch ( event->key()) {
      case Qt::Key_Up     : goUp    (); break ;
      case Qt::Key_Down   : goDown  (); break ;
      case Qt::Key_Left   : goLeft  (); break ;
      case Qt::Key_Right  : goRight (); break ;
      default : return ;
    }
    event->accept();
  }

  void CellWidget::goRight() {
    viewport_.translate( Point (-20, 0) );
    repaint();
  }
  void CellWidget::goUp() {
    viewport_.translate( Point (0, -20) );
    repaint ();
  }
  void CellWidget::goDown() {
    viewport_.translate( Point (0, 20) );
    repaint ();
  }
  void CellWidget::goLeft() {
    viewport_.translate( Point (20, 0) );
    repaint ();
  }

  void  CellWidget::paintEvent ( QPaintEvent* event )
  {
    QFont  bigFont = QFont( "URW Bookman L", 36 );

    QString cellName = "NULL";
    if (cell_) cellName = cell_->getName().c_str();

    QPainter painter(this);
    painter.setFont      ( bigFont );
    painter.setBackground( QBrush( Qt::black ) );
    painter.eraseRect    ( QRect( QPoint(0,0), size() ) );
    painter.setPen  (QPen(Qt::gray));

    int frameWidth  = 460;
    int frameHeight = 100;

    QRect nameRect ( (size().width ()-frameWidth )/2
                   , 0 //pour le mettre en haut //(size().height()-frameHeight)/2
                   , frameWidth
                   , frameHeight
                   );

    painter.drawRect( nameRect );
    painter.drawText( nameRect, Qt::AlignCenter, cellName );

    painter.setPen(QPen(Qt::darkGreen,3));
    query(InstanceShapes,painter);

    painter.setPen(QPen(Qt::cyan,1));
    query(Wires,painter);

    painter.setPen(QPen(Qt::red));
    query(InternalTerms,painter);

    painter.setPen(QPen(Qt::red));
    query(ExternalTerms,painter);
  }

  void CellWidget::query ( unsigned int flags , QPainter& painter ) {
    if (( not cell_ ) or ( not flags )) return ;
    const vector<Instance*>& instances = cell_->getInstances();

    if( instances.empty() ){ // Si pas d'instances dans la cellule
      if ( flags & InstanceShapes ) {
        const vector<Shape*>& shapes = cell_->getSymbol()->getShapes();
        for ( size_t j = 0; j<shapes.size() ; ++j ) {
          
          BoxShape*     boxShape      = dynamic_cast<BoxShape*>     ( shapes[j]);
          LineShape*    lineShape     = dynamic_cast<LineShape*>    ( shapes[j]);
          TermShape*    termShape     = dynamic_cast<TermShape*>    ( shapes[j]);
          EllipseShape* ellipseShape  = dynamic_cast<EllipseShape*> ( shapes[j]);
          ArcShape*     arcShape      = dynamic_cast<ArcShape*>     ( shapes[j]);

          if(boxShape){
            QRect rect = boxToScreenRect(boxShape->getBoundingBox());
            painter.setPen(QPen(Qt::gray,3));
            painter.drawRect(rect);
            continue;
          }
          if(lineShape){
            QPoint p1 = pointToScreenPoint(Point(lineShape->getBoundingBox().getX1(),shapes[j]->getBoundingBox().getY1()));
            QPoint p2 = pointToScreenPoint(Point(lineShape->getBoundingBox().getX2(),shapes[j]->getBoundingBox().getY2()));
            if(lineShape->getSens()){
              p1 = pointToScreenPoint(Point(lineShape->getBoundingBox().getX1(),shapes[j]->getBoundingBox().getY2()));
              p2 = pointToScreenPoint(Point(lineShape->getBoundingBox().getX2(),shapes[j]->getBoundingBox().getY1()));
            }
            painter.setPen(QPen(Qt::gray,3));
            painter.drawLine(p1,p2);
            continue;
          }
          if(termShape){
            QRect term = boxToScreenRect(termShape->getBoundingBox());
            painter.setPen(QPen(Qt::gray,3));
            painter.drawRect(term);
            continue;
          }
          if(arcShape){
            QRect arc = boxToScreenRect(arcShape->getBoundingBox());
            painter.setPen(QPen(Qt::gray,3));
            painter.drawArc(arc,arcShape->getStart()*16,arcShape->getSpan()*16);
            continue;
          }
          if(ellipseShape){
            QRect ellipse = boxToScreenRect(ellipseShape->getBoundingBox());
            painter.setPen(QPen(Qt::gray,3));
            painter.drawEllipse(ellipse);
            continue;
          }
        }
      }
    }
    else{ //Si instances
      for ( size_t i = 0; i < instances.size() ; ++i ) {
        Point instPos = instances[i]->getPosition();
        const Symbol * symbol = instances[i]->getMasterCell()->getSymbol();
        if ( not symbol ) continue ;

        if ( flags & InstanceShapes ) {

          const vector<Shape*>& shapes = symbol->getShapes();
          for ( size_t j = 0; j<shapes.size() ; ++j ) {
            
            
            BoxShape*     boxShape      = dynamic_cast<BoxShape*>     ( shapes[j]);
            LineShape*    lineShape     = dynamic_cast<LineShape*>    ( shapes[j]);
            TermShape*    termShape     = dynamic_cast<TermShape*>    ( shapes[j]);
            EllipseShape* ellipseShape  = dynamic_cast<EllipseShape*> ( shapes[j]);
            ArcShape*     arcShape      = dynamic_cast<ArcShape*>     ( shapes[j]);

            if(boxShape){
              QRect rect = boxToScreenRect(boxShape->getBoundingBox().translate(instPos));
              painter.setPen(QPen(Qt::darkGreen,3));
              painter.drawRect(rect);
              continue;
            }
            if(lineShape){
              QPoint p1 = pointToScreenPoint(Point(lineShape->getBoundingBox().getX1(),shapes[j]->getBoundingBox().getY1()).translate(instPos));
              QPoint p2 = pointToScreenPoint(Point(lineShape->getBoundingBox().getX2(),shapes[j]->getBoundingBox().getY2()).translate(instPos));
              if(lineShape->getSens()){
                p1 = pointToScreenPoint(Point(lineShape->getBoundingBox().getX1(),shapes[j]->getBoundingBox().getY2()).translate(instPos));
                p2 = pointToScreenPoint(Point(lineShape->getBoundingBox().getX2(),shapes[j]->getBoundingBox().getY1()).translate(instPos));
              }
              painter.setPen(QPen(Qt::darkGreen,3));
              painter.drawLine(p1,p2);
              continue;
            }
            if(termShape){
              QRect term = boxToScreenRect(termShape->getBoundingBox().translate(instPos));
              painter.setPen(QPen(Qt::darkGreen,3));
              painter.drawRect(term);
              continue;
            }
            if(arcShape){
              QRect arc = boxToScreenRect(arcShape->getBoundingBox().translate(instPos));
              painter.setPen(QPen(Qt::darkGreen,3));
              painter.drawArc(arc,arcShape->getStart()*16,arcShape->getSpan()*16);
              continue;
            }
            if(ellipseShape){
              QRect ellipse = boxToScreenRect(ellipseShape->getBoundingBox().translate(instPos));
              painter.setPen(QPen(Qt::darkGreen,3));
              painter.drawEllipse(ellipse);
              continue;
            }
          }

          painter.setPen ( QPen(Qt::green,0)); //NOM INSTANCES
          painter.setFont( QFont( "URW Bookman L", 10 ) );

          // QPoint center = pointToScreenPoint(Point((symbol->getBoundingBox().getX1()+symbol->getBoundingBox().getX2())/2
          // , (symbol->getBoundingBox().getY1()+ symbol->getBoundingBox().getY2())/2).translate(instPos));
          // painter.drawText( center, QString::fromStdString(instances[i]->getName()));
          QPoint p1 = pointToScreenPoint(Point(symbol->getBoundingBox().getX1(),symbol->getBoundingBox().getY1()).translate(instPos));
          QPoint p2 = pointToScreenPoint(Point(symbol->getBoundingBox().getX2(),symbol->getBoundingBox().getY2()).translate(instPos));
          painter.drawText( QRect(p1,p2), Qt::AlignCenter, QString::fromStdString(instances[i]->getName()));  
  
        }
        if(flags & InternalTerms){         
          const vector<Shape*>& shapes = instances[i]->getMasterCell()->getSymbol()->getShapes();        
          for (size_t j= 0; j< shapes.size();++j){
            TermShape* termShape = dynamic_cast<TermShape*>(shapes[j]);
            if(termShape){
              
              QPoint term = pointToScreenPoint(Point(
              termShape->getBoundingBox().translate(instPos).getX1()
              ,termShape->getBoundingBox().translate(instPos).getY1()));
              
              painter.setPen(QPen(Qt::red,10));
              painter.drawPoint(term);
              
              painter.setPen(QPen(Qt::red,0));
              painter.setFont      ( QFont( "URW Bookman L", 10 ) );
              if(instances[i]){
                if(termShape->getNameAlign() == 1){ //TopLeft
                  QRect textRect(term.x() - 15, term.y() - 25, 100, 20);
                  painter.drawText( textRect, QString::fromStdString(termShape->getTerm()->getName()) );
                }
                if(termShape->getNameAlign() == 2){ //TopRight
                  QRect textRect(term.x() +5 , term.y() -25, 100, 20);
                  painter.drawText( textRect, QString::fromStdString(termShape->getTerm()->getName()) );
                }
                if(termShape->getNameAlign() == 3){ //BottomLeft
                  QRect textRect(term.x() - 15, term.y() + 5, 100, 20);
                  painter.drawText( textRect, QString::fromStdString(termShape->getTerm()->getName()) );
                }
                if(termShape->getNameAlign() == 4){ //BottomRight
                  QRect textRect(term.x() + 5, term.y() + 5, 100, 20);
                  painter.drawText( textRect, QString::fromStdString(termShape->getTerm()->getName()) );
                }
              }
              continue;
            }
          }
        }
        if ( flags & Wires ) { 
          const vector<Net*>& nets = cell_->getNets();
          for ( size_t j = 0; j<nets.size() ; ++j ) {

            const vector<Line*>& lines = nets[j]->getLines();

            for (size_t j= 0; j< lines.size();++j){ //Partie faite par Emile Pirard qui permet d'aller chercher correctement les positions target/sources
            
              painter.setPen ( QPen ( Qt :: cyan , 1 ) );
              
              QPoint target = pointToScreenPoint(lines[j]->getTargetPosition());
              NodeTerm* nt = (dynamic_cast<NodeTerm*>(lines[j]->getTarget())); //Node donne des points qui ne sont pas bons / NodeTerm donne des points spécifiques
              if (nt!=NULL){  
                if (Instance* inst = (nt->getTerm()->getInstance())){

                  Point instPos = inst -> getPosition ();
                  const Symbol * symbol = inst -> getMasterCell() -> getSymbol();
                  const vector<Shape *>& shapes = symbol -> getShapes ();
                  for ( size_t k =0 ; k < shapes.size() ; ++ k ) {
                    TermShape* term = dynamic_cast<TermShape*>( shapes [ k ]);
                    if(term!=NULL){
                  
                      if(((NodeTerm*)(lines[j]->getTarget()))->getTerm()->getName()==term->getTerm()->getName()){
                        target = pointToScreenPoint( Point(
                        term->getBoundingBox().translate( instPos ).getX1()
                        ,term->getBoundingBox().translate( instPos ).getY1()));
                      }
                    }
                  }
                }
              }

              TermShape * term2;
              QPoint source = pointToScreenPoint(lines[j]->getSourcePosition());
              
              NodeTerm * nt2 = (dynamic_cast<NodeTerm*>(lines[j]->getSource()));
              if (nt2!=NULL){  
              
                if (Instance* inst2 = (nt2->getTerm()->getInstance())){
              
                  Point instPos2 = inst2 -> getPosition ();
              
                  const Symbol * symbol2 = inst2 -> getMasterCell() -> getSymbol();
                  const vector<Shape *>& shapes2 = symbol2 -> getShapes ();
              
                  for ( size_t k =0 ; k < shapes2.size() ; ++ k ) {
                    term2 = dynamic_cast<TermShape*>( shapes2 [ k ]);
                    
                    if(term2!=NULL){
                      if(((NodeTerm*)(lines[j]->getSource()))->getTerm()->getName()==term2->getTerm()->getName()){
                        source = pointToScreenPoint( Point(
                        term2->getBoundingBox().translate( instPos2 ).getX1()
                        ,term2->getBoundingBox().translate( instPos2 ).getY1()));
                      }
                    }
                  }
                }
              }

              painter.drawLine(source,target);
              painter.setPen ( QPen ( Qt::cyan , 0 ) );
              painter.setBrush(QBrush(Qt::cyan));
              if(lines[j]->getSource()->getDegree()>2){
                int x = source.x();
                int y = source.y();
                QRect grospoint = QRect(QPoint(x-4,y-4),QPoint(x+4,y+4));
                painter.drawEllipse(grospoint);
              }
              if(lines[j]->getTarget()->getDegree()>2){
                int x = target.x();
                int y = target.y();
                QRect grospoint = QRect(QPoint(x-4,y-4),QPoint(x+4,y+4));
                painter.drawEllipse(grospoint);
              }
              painter.setBrush(QBrush());
            }
          }
        }

        if(flags & ExternalTerms){
          const vector<Term*>& terms = cell_->getTerms();
          for (size_t j= 0; j< terms.size();++j){
            //Noms des terms
          painter.setFont( QFont( "URW Bookman L", 10 ) );

            if((terms[j]->isExternal()) && (terms[j]->getDirection() == Term::toDirection("Out"))){
              QRect rect = boxToScreenRect(Box(
                terms[j]->getPosition().getX()
                ,terms[j]->getPosition().getY()
                ,terms[j]->getPosition().getX()
                ,terms[j]->getPosition().getY()).inflate(4)
              );
              painter.setPen(QPen(Qt::red,3));
              painter.setBrush(QBrush(Qt::red));
              painter.drawRect(rect);
              //NOMS TERMS
              int textX = rect.x() + 20;
              int textY = rect.y();
              painter.setPen ( QPen(Qt::red,1));
              painter.drawText( QRect(textX,textY,100,20) ,QString::fromStdString(terms[j]->getName()) );
              continue;
            }
            if((terms[j]->isExternal()) && (terms[j]->getDirection() == Term::toDirection("In"))){ //Pour les dessins.
              QRect rect = boxToScreenRect(Box(
                terms[j]->getPosition().getX()
                ,terms[j]->getPosition().getY()
                ,terms[j]->getPosition().getX()
                ,terms[j]->getPosition().getY()).inflate(4)
              );
              painter.setPen(QPen(Qt::red,3));
              painter.setBrush(QBrush(Qt::red));
              painter.drawRect(rect);
              //NOMS TERMS
              int textX = rect.x() - 20;
              int textY = rect.y();
              painter.setPen ( QPen(Qt::red,1));
              painter.drawText( QRect(textX,textY,100,20), QString::fromStdString(terms[j]->getName()) );
              continue;
            }
          }
        }
      }
    }
  }
}

 // Netlist namespace.
