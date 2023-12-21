#include <iostream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <vector>

#include "Cell.h"
#include "Term.h"
#include "Point.h"
#include "Instance.h"
#include "Net.h"
#include "XmlUtil.h"
#include "Shape.h"

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
        Box box;
        for(size_t i = 0; i < shapes_.size();i++){
            box.merge(shapes_[i]->getBoundingBox());
        }
        return box;
    }
    
    Point Symbol::getTermPosition ( Term* term ) const{
        return term->getPosition();
    }
    
    TermShape* Symbol::getTermShape(Term* term) const {
        for (std::vector<Shape*>::const_iterator it = shapes_.begin(); it != shapes_.end() ; ++it) {

            TermShape* termShape = dynamic_cast<TermShape*>(*it);
            if (termShape != NULL && termShape->getTerm() == term) {
                return termShape;
            }
        }
        return NULL;
    }

    void Symbol::add ( Shape* shape){
        shapes_.push_back(shape);
    }
    
    void Symbol::remove ( Shape* shape){
        for(std::vector<Shape*>::iterator it = shapes_.begin(); it != shapes_.end() ; ++it){
            if(*it == shape){
                shapes_.erase(it);
            }
        }
    }
    
    void Symbol::toXml ( std::ostream& stream ) const {
        stream << indent <<"<symbol>" << std::endl;
         for(std::vector<Shape*>::const_iterator it = shapes_.begin(); it != shapes_.end() ; ++it){
            (*it)->toXml(stream);
        }
        stream <<indent <<"</symbol>" << std::endl;
    }
    
    Symbol* Symbol::fromXml ( Cell* owner, xmlTextReaderPtr reader){
        
        Symbol* symbol = new Symbol(owner);

        while(true){
            int status = xmlTextReaderRead(reader);

            if (status == 0){
                break; //Fin du XML
            }
            else if (status !=1){
                std::cout << "[ERROR] Symbol::fromXml(): Unexpected termination of the XML parser." << std::endl;
                delete symbol;
                return NULL;
            }

            switch ( xmlTextReaderNodeType(reader) ) {
                case XML_READER_TYPE_COMMENT:
                case XML_READER_TYPE_WHITESPACE:
                case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }

            if(xmlTextReaderNodeType(reader)== XML_READER_TYPE_END_ELEMENT){
                return symbol;
            }
            // const xmlChar* Name = xmlTextReaderConstLocalName( reader );
            Shape* shapeXml = Shape::fromXml(symbol,reader);
            if(shapeXml != NULL){
                owner->getSymbol()->add(shapeXml);
            }
            else{
                std::cout << "[ERROR] Symbol::fromXml(): Unknown" << std::endl;
                delete symbol;
                return NULL;
            }
        }
        return symbol;
    }
}