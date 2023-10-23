#include <iostream>
#include <string>

#include "Instance.h"
#include "Cell.h"
#include "Term.h"
#include "Point.h"
#include "Net.h"

namespace Netlist{
    Instance::Instance( Cell* owner, Cell* model, const std::string& name){
        owner_ = owner;
        masterCell_ = model;
        name_ = name;
        terms_ = std::vector<Term*>();
        position_ = Point(0,0);
    }
    
    Instance::~Instance(){}

    //Accessseurs
    const std::string& Instance::getName() const{
        return name_;
    }

    Cell* Instance::getMasterCell () const{
        return masterCell_;
    }

    Cell* Instance::getCell() const{
        return owner_;
    }

    const std::vector<Term*>& Instance::getTerms() const{
        return terms_;
    }

    Term* Instance::getTerm( const std::string& s) const{
        return NULL;
    }

    Point Instance::getPosition() const{
        return Point(0,0);
    }

    //Modificateurs
    bool  Instance::connect       ( const std::string& name, Net* ){return true;}
    void  Instance::add           ( Term* ){}
    void  Instance::remove        ( Term* ){}
    void  Instance::setPosition   ( const Point& ){}
    void  Instance::setPosition   ( int x, int y ){}
}