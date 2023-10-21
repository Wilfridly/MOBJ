#include <iostream>
#include <string>

#include "Instance.h"
#include "Cell.h"
#include "Term.h"
#include "Point.h"
#include "Net.h"

namespace Netlist{
    Instance::Instance      ( Cell* owner, Cell* model, const std::string& ){}
    Instance::~Instance      (){}

    //Accessseurs
    const std::string&        Instance::getName       () const{return NULL;}
    Cell*                     Instance::getMasterCell () const{return NULL;}
    Cell*                     Instance::getCell       () const{return NULL;}
    const std::vector<Term*>& Instance::getTerms      () const{return std::vector<Term*>();}
    Term*                     Instance::getTerm       ( const std::string& ) const{return NULL;}
    Point                     Instance::getPosition   () const{return Point(0,0);}

    //Modificateurs
    bool  Instance::connect       ( const std::string& name, Net* ){return true;}
    void  Instance::add           ( Term* ){}
    void  Instance::remove        ( Term* ){}
    void  Instance::setPosition   ( const Point& ){}
    void  Instance::setPosition   ( int x, int y ){}
}