#pragma once
#include "Box.h"
#include "Symbol.h"
#include "Shape.h"
#include "Term.h"
#include "Cell.h"

namespace Netlist{
    //Classe mère Shape
    Shape::Shape(Symbol* owner) : owner_(owner){
        owner->add(this);
    }
    Shape::~Shape(){
        owner_->remove(this);
    }
    Symbol* Shape ::getSymbol() const{ return owner_ ; }

    //Classe fille Boxshape
    BoxShape::BoxShape( Symbol* owner, const Box& box ):Shape(owner),box_( box ){}
    BoxShape::BoxShape( Symbol* owner, int x1 , int y1 , int x2 , int y2  ):Shape(owner) {
        owner_ = owner;
        box_ = new Box(x1,y1,x2,y2);
    }
    BoxShape::~BoxShape (){}
        //Accesseurs
    Box BoxShape::getBoundingBox () const{
        return box_ ; 
    }

    //Classe fille TermShape
    TermShape::TermShape( Symbol* owner, Term* term , int x , int y ):owner_(owner),term_(term),x_(x) ,y_(y){}
    TermShape::~TermShape (){}
        //Accesseurs
    Box TermShape::getBoundingBox () const{
        Box box = new Box(x,y,x,y);
        return box; 
    }
    
    //Classe fille LineShape
    LineShape::LineShape( Symbol* owner, int x1, int y1, int x2, int y2):owner_(owner),x1_(x1),y1_(y1),x2_(x2),y2_(y2){}
    LineShape::~LineShape(){}
    
    Box LineShape::getBoundingBox () const{
        Box box = new Box(x1,y1,x2,y2);
        return box; 
    }

    //Classe fille EllipseShape
    EllipseShape::EllipseShape( Symbol* owner, const Box& box ):Shape(owner),box_( box ){}
    EllipseShape::EllipseShape( Symbol* owner, int x1 , int y1 , int x2 , int y2  ):Shape(owner){
        box_ = new Box(x1,y1,x2,y2);
    }
    EllipseShape::~EllipseShape(){}

    Box EllipseShape::getBoundingBox () const{
        return box_; 
    }

    //Classe fille ArcShape
    ArcShape::ArcShape(Symbol* owner, Box& box):Shape(owner),box_(box){}
    ArcShape::ArcShape(Symbol* owner, int x1 , int y1 , int x2 , int y2  ):Shape(owner){
        box_ = new Box(x1,y1,x2,y2);
    }
    ArcShape::~ArcShape(){}
}