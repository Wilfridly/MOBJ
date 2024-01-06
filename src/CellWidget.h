
#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"


namespace Netlist {

  class Cell;
  class NodeTerm;


class CellWidget : public QWidget {
    Q_OBJECT;
  public:
                    CellWidget         ( QWidget* parent=NULL );//done
    virtual        ~CellWidget         ();                      //done
            void    setCell            ( Cell* );               //done
    inline  Cell*   getCell            () const{ return cell_;}  //done

    inline  int     xToScreenX         ( int x ) const{ return (x - viewport_.getX1());} //done
    inline  int     yToScreenY         ( int y ) const{ return (viewport_.getY2() - y);} //done

    inline  int     screenXToX         ( int x ) const{ return (x + viewport_.getX1());} //done
    inline  int     screenYToY         ( int y ) const{ return (viewport_.getY2() - y);} //done

    inline  QRect   boxToScreenRect    ( const Box& box ) const{ 
      return QRect(
        xToScreenX(box.getX1())
        ,yToScreenY(box.getY1())
        ,xToScreenX(box.getX2()) - xToScreenX(box.getX1())
        ,yToScreenY(box.getY2()) - yToScreenY(box.getY1())
      );
    }    
    inline  QPoint  pointToScreenPoint ( const Point& point ) const{ return QPoint(xToScreenX(point.getX()), yToScreenY(point.getY()));}

    inline  Box     screenRectToBox    ( const QRect& qrect) const{
      return Box(
        screenXToX(qrect.x())
        ,screenYToY(qrect.y())
        ,screenXToX(qrect.x() + qrect.width())
        ,screenYToY(qrect.y() + qrect.height())
      );
    }
    

    inline  Point   screenPointToPoint ( const QPoint& qpoint ) const{ return Point(screenXToX(qpoint.x()), screenYToY(qpoint.y())); }

    virtual QSize   minimumSizeHint    () const;                //done
    virtual void    resizeEvent        ( QResizeEvent* );       //done
  protected:
    virtual void    paintEvent         ( QPaintEvent* );        //done
    virtual void    keyPressEvent      ( QKeyEvent* );          //done
  public slots:
            void    goLeft             ();                      //done
            void    goRight            ();                      //done
            void    goUp               ();                      //done
            void    goDown             ();                      //done  
  protected:
    void query(unsigned int, QPainter& ); 
  private:
    Cell* cell_;
    Box   viewport_;
    enum Layer{
      InstanceShapes=1,
      Wires         ,
      InternalTerms ,
      ExternalTerms 
    };
  };

}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H
