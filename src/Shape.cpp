#include "Box.h"
#include "Symbol.h"
#include "Shape.h"
#include "Term.h"
#include "Cell.h"
#include "XmlUtil.h"

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
            shape = TermShape::fromXml( owner, reader ); //owner->getSymbol() si c'est une Cell mais ça duplique. 
        if (shape == NULL)
            std::cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
            << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
        return shape;
    }
    






    //Classe fille Boxshape
    BoxShape::BoxShape( Symbol* owner, const Box& box ):Shape(owner),box_( box ){}
    
    BoxShape::BoxShape( Symbol* owner, int x1 , int y1 , int x2 , int y2  ):Shape(owner),box_(Box(x1,y1,x2,y2)) {}
    BoxShape::~BoxShape (){}
        //Accesseurs
    Box BoxShape::getBoundingBox () const{
        return box_ ; 
    }
    void  BoxShape::toXml ( std::ostream& stream )const{
        indent++;
        stream << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>" << std::endl;
        indent--;
    }

    Shape* BoxShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){
        int x1 = 0;
        int x2 = 0;
        int y1 = 0;
        int y2 = 0;  

        const xmlChar* BoxTag   = xmlTextReaderConstString           ( reader, (const xmlChar*)"box" );
        const xmlChar* BoxName = xmlTextReaderConstLocalName         ( reader );
        
        if(BoxTag == BoxName){
            if( xmlGetIntAttribute(reader, "x1", x1) &&
                xmlGetIntAttribute(reader, "x2", x2) &&
                xmlGetIntAttribute(reader, "y1", y1) &&
                xmlGetIntAttribute(reader, "y2", y2)) {
                
                Box box = Box(x1,y1,x2,y2);    
                Shape* shape = new BoxShape(owner,box);
                return shape;
            }
        }
        else{
            std::cerr << "Problème avec le fromXml de BoxShape" << std::endl;
        }
        return NULL;
    }







    //Classe fille TermShape
    TermShape::TermShape( Symbol* owner, Term* term , int x , int y, TermShape::NameAlign align ) : Shape(owner),term_(term),x_(x) ,y_(y), align_(align){}
    TermShape::~TermShape (){}
        //Accesseurs
    Box TermShape::getBoundingBox () const{
        return Box(x_,y_,x_,y_); 
    }
    
    std::string TermShape::toString (TermShape::NameAlign NameA){
        switch (NameA){
            case 1:
                return(std::string("top_left"));
            case 2:
                return(std::string("top_right"));
            case 3:
                return(std::string("bottom_left"));
            case 4:
                return(std::string("bottom_right"));
            default:
                return(std::string("Unknown"));     
        }
    }
    TermShape::NameAlign TermShape::toNameAlign(std::string str){
        if(str == "top_left") return (TopLeft);
        if(str == "top_right") return (TopRight);
        if(str == "bottom_left") return (BottomLeft);
        if(str == "bottom_right") return (BottomRight);
        else return(Unknown);
    }

    void  TermShape::toXml ( std::ostream& stream )const{
        indent++;
        stream << indent << "<term name=" << term_->getName() << "\" x1=\"" << x_ << "\" y1=\"" << y_ << "\" align=\"" << TermShape::toString(align_) << "\"/>" << std::endl;
        indent--;
    }

    Shape* TermShape::fromXml ( Symbol* owner, xmlTextReaderPtr reader){
        int x1 = 0;
        int y1 = 0;

        const xmlChar* BoxTag   = xmlTextReaderConstString           ( reader, (const xmlChar*)"term" );
        const xmlChar* BoxName = xmlTextReaderConstLocalName         ( reader );
        
        if(BoxTag == BoxName){
            
            const std::string TermName = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"name"));
            const std::string align = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"align"));

            if(xmlGetIntAttribute(reader, "x1", x1) && xmlGetIntAttribute(reader, "y1", y1) ){    
                TermShape* termshape = new TermShape(owner,owner->getCell()->getTerm(TermName),x1,y1,toNameAlign(align));
                return termshape;
                // return new TermShape(owner->getSymbol(), owner->getTerm(TermName) ,x1, y1, toNameAlign(align));
            }
        }
        else{
            std::cerr << "Problème avec le fromXml de TermShape" << std::endl;
        }
        return NULL;
    }





    //Classe fille LineShape
    LineShape::LineShape( Symbol* owner, int x1, int y1, int x2, int y2):Shape(owner),x1_(x1),y1_(y1),x2_(x2),y2_(y2){}
    LineShape::~LineShape(){}
    
    Box LineShape::getBoundingBox () const{
        return Box(x1_,y1_,x2_,y2_); 
    }
    void  LineShape::toXml ( std::ostream& stream ) const{
        indent++;
        stream << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_ << "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>" << std::endl;
        indent--;
    }
    
    Shape* LineShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){
        int x1 = 0;
        int x2 = 0;
        int y1 = 0;
        int y2 = 0;  

        const xmlChar* BoxTag   = xmlTextReaderConstString           ( reader, (const xmlChar*)"line" );
        const xmlChar* BoxName = xmlTextReaderConstLocalName         ( reader );
        
        if(BoxTag == BoxName){
            if(xmlGetIntAttribute(reader, "x1", x1) && xmlGetIntAttribute(reader, "x2", x2) && xmlGetIntAttribute(reader, "y1", y1) && xmlGetIntAttribute(reader, "y2", y2)){    
                Shape* shape = new LineShape(owner, x1, y1, x2, y2);
                return shape;
            }
        }
        else{
            std::cerr << "Problème avec le fromXml de LineShape" << std::endl;
        }
        return NULL;
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
    void  EllipseShape::toXml ( std::ostream& stream ) const{
        indent++;
        stream << indent <<"<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>" << std::endl;
        indent--;
    }
    
    Shape* EllipseShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){
        int x1 = 0;
        int x2 = 0;
        int y1 = 0;
        int y2 = 0;  

        const xmlChar* BoxTag   = xmlTextReaderConstString           ( reader, (const xmlChar*)"ellipse" );
        const xmlChar* BoxName = xmlTextReaderConstLocalName         ( reader );

        if(BoxTag == BoxName){
            if(xmlGetIntAttribute(reader, "x1", x1) && xmlGetIntAttribute(reader, "x2", x2) && xmlGetIntAttribute(reader, "y1", y1) && xmlGetIntAttribute(reader, "y2", y2)){    
                Shape* shape = new EllipseShape(owner, Box(x1, y1, x2, y2 ));
                return shape;
            }
        }
        else{
            std::cerr << "Problème avec le fromXml de EllipseShape" << std::endl;
        }
        return NULL;
    }
    







    //Classe fille ArcShape
    ArcShape::ArcShape(Symbol* owner, const Box& box, int start, int span):Shape(owner),box_(box), start_(start), span_(span){}
    // ArcShape::ArcShape(Symbol* owner, int x1 , int y1 , int x2 , int y2  ):Shape(owner){
    //     box_ = new Box(x1,y1,x2,y2);
    // }
    ArcShape::~ArcShape(){}

    Box ArcShape::getBoundingBox () const{
        return box_; 
    }

    void  ArcShape::toXml ( std::ostream& stream )const{
        indent++;
        stream << indent <<"<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\" start=\"" << start_ << "\" span=\"" << span_ << "\"/>" << std::endl;
        indent--;    
    }

    Shape* ArcShape::fromXml(Symbol* owner, xmlTextReaderPtr reader){
        int x1 = 0;
        int x2 = 0;
        int y1 = 0;
        int y2 = 0;  
        int start = 0;
        int span = 0 ;
        const xmlChar* BoxTag   = xmlTextReaderConstString           ( reader, (const xmlChar*)"arc" );
        const xmlChar* BoxName = xmlTextReaderConstLocalName         ( reader );

        if(BoxTag == BoxName){
            if(xmlGetIntAttribute(reader, "x1", x1) && xmlGetIntAttribute(reader, "x2", x2) && xmlGetIntAttribute(reader, "y1", y1) && xmlGetIntAttribute(reader, "y2", y2) && xmlGetIntAttribute(reader,"start", start) && xmlGetIntAttribute(reader,"span", span)){    
                Shape* shape = new ArcShape(owner, Box(x1, y1, x2, y2 ), start, span);
                return shape;
            }
        }
        else{
            std::cerr << "Problème avec le fromXml de ArcShape" << std::endl;
        }
        return NULL;
    }
}
