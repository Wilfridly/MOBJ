#include <iostream>
#include <string>
#include <stdlib.h>


#include "Cell.h"
#include "Term.h"
#include "Point.h"
#include "Instance.h"
#include "Net.h"
#include "XmlUtil.h"

namespace Netlist{

    Symbol::Symbol ( Cell* owner){
        owner_ = owner;
    }

    Symbol::~Symbol(){
    }
    
    Cell* Symbol::getCell () const{
        return owner_;
    }
    
    Box Symbol::getBoundingBox () const{
        Box bb;
        for(size__t i = 0; i < shapes_.size();i++){
            bb.merge(shapes_[i]->getBoundingBox())
        }
        return bb;
    }
    
    Point Symbol::getTermPosition ( Term* term ) const{
        return term->getPosition();
    }
    
    TermShape* Symbol::getTermShape    ( Term* ) const{
        return term->getTermShape();
    }
    void Symbol::add ( Shape* ){
               
    }
    
    void Symbol::remove ( Shape* );
    
    void Symbol::toXml ( std::ostream& ) const;
    
    static Symbol* Symbol::fromXml         ( Cell*, xmlTextReaderPtr );

}