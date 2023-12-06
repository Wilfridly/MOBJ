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

    Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader ){
        // Factory-like method.
        const xmlChar* boxTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
        const xmlChar* ellipseTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
        const xmlChar* arcTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
        const xmlChar* lineTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
        const xmlChar* termTag
        = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
        const xmlChar* nodeName
        = xmlTextReaderConstLocalName( reader );
        Shape* shape = NULL;
        if (boxTag == nodeName)
            shape = BoxShape::fromXml( owner, reader );
        if (ellipseTag == nodeName)
            shape = EllipseShape::fromXml( owner, reader );
        if (arcTag == nodeName)
            shape = ArcShape::fromXml( owner, reader );
        if (lineTag == nodeName)
            shape = LineShape::fromXml( owner, reader );
        if (termTag == nodeName)
            shape = TermShape::fromXml( owner, reader );
        if (shape == NULL)
            cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
            << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
        return shape;
    }
    






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
    void  BoxShape::toXml ( std::ostream& stream ){
        stream << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>";
    }

    bool* BoxShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){

        const xmlChar* BoxTag   = xmlTextReaderConstString           ( reader, (const xmlChar*)"box" );
        const xmlChar* BoxName = xmlTextReaderConstLocalName         ( reader );
        
        if(BoxTag == BoxName){
            const std::string symbx1 = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"x1"));
            const std::string symby1 = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"y1"));
            const std::string symbx2 = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"x2"));
            const std::string symby2 = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"y2"));
        }
        if (symbx1.isEmpty()! && symby1.isEmpty()! && symbx2.isEmpty()! && symby2.isEmpty()!){
            BoxShape* boxS = new BoxShape(owner, atoi(symbx1.c_str()), atoi(symby1.c_str()), atoi(symbx2.c_str()), atoi(symbx2.c_str()));
            return true;
        }
        else
            return false;

    }







    //Classe fille TermShape
    TermShape::TermShape( Symbol* owner, Term* term , int x , int y ):owner_(owner),term_(term),x_(x) ,y_(y){}
    TermShape::~TermShape (){}
        //Accesseurs
    Box TermShape::getBoundingBox () const{
        Box box = new Box(x,y,x,y);
        return box; 
    }
    void  TermShape::toXml ( std::ostream& stream ){
        stream << "<term name=" << term_.getName() << "\" x1=\"" << x_ << "\" y1=\"" << y_ << "\" align=\"" << align_ << "\"/>";
    }
    std::string TermShape::toString (TermShape::NameAlign NameA){
        switch (NameA){
            case 1:
                return(std::string("TopLeft"));
            case 2:
                return(std::string("TopRight"));
            case 3:
                return(std::string("BottomLeft"));
            case 4:
                return(std::string("BottomRight"));
            default:
                return(std::string("Unknown"));     
        }
    }
    TermShape::NameAlign TermShape::toNameAlign(std::string str){
        if(str == "TopLeft") return (TopLeft);
        if(str == "TopRight") return (TopRight);
        if(str == "BottomLeft") return (BottomLeft);
        if(str == "BottomRight") return (BottomRight);
        else return(Unknown);
    }






    //Classe fille LineShape
    LineShape::LineShape( Symbol* owner, int x1, int y1, int x2, int y2):owner_(owner),x1_(x1),y1_(y1),x2_(x2),y2_(y2){}
    LineShape::~LineShape(){}
    
    Box LineShape::getBoundingBox () const{
        Box box = new Box(x1,y1,x2,y2);
        return box; 
    }
    void  LineShape::toXml ( std::ostream& stream ){
        stream << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>";
    }







    //Classe fille EllipseShape
    EllipseShape::EllipseShape( Symbol* owner, const Box& box ):Shape(owner),box_( box ){}
    // EllipseShape::EllipseShape( Symbol* owner, int x1 , int y1 , int x2 , int y2  ):Shape(owner){
    //     box_ = new Box(x1,y1,x2,y2);
    // }
    EllipseShape::~EllipseShape(){}

    Box EllipseShape::getBoundingBox () const{
        return box_; 
    }
    void  EllipseShape::toXml ( std::ostream& stream ){
        stream << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box.getY2() << "\"/>";
    }







    //Classe fille ArcShape
    ArcShape::ArcShape(Symbol* owner, Box& box):Shape(owner),box_(box), start_(start), span_(span){}
    // ArcShape::ArcShape(Symbol* owner, int x1 , int y1 , int x2 , int y2  ):Shape(owner){
    //     box_ = new Box(x1,y1,x2,y2);
    // }
    ArcShape::~ArcShape(){}
    void  ArcShape::toXml ( std::ostream& stream ){
        stream << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" start=\"" << start_ << "\" span=\"" << span_ << "\"/>";
    }
}