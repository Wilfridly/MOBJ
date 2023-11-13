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
        owner_->add(this);
        // std::cout << "Debug::Instance CTOR" << std::endl;

        std::vector<Term*> terms = model->getTerms();
        for ( std::vector<Term*>::const_iterator it = terms.begin() ; it != terms.end() ; ++it ) {
            new Term(this, *it);
        }
    }
    
    Instance::~Instance(){
        owner_->remove(this);
        while (not terms_.empty()) delete *terms_.begin();
    }

    //Accessseurs
    const std::string& Instance::getName()          const{return name_;}
    Cell* Instance::getMasterCell       ()          const{return masterCell_;}
    Cell* Instance::getCell             ()          const{return owner_;}
    const std::vector<Term*>& Instance::getTerms()  const{return terms_;}


    Term* Instance::getTerm( const std::string& name) const{
        for ( std::vector<Term*>::const_iterator it = terms_.begin() ; it != terms_.end() ; ++it ) {
            if ((*it)->getName() == name)  return *it;
        }
        return NULL;
    }

    Point Instance::getPosition() const{return position_;}

    //Modificateurs
    bool  Instance::connect       ( const std::string& name, Net* net){
        Term* term = getTerm(name);
        if (term == NULL) return false;
        term->setNet(net);
        return true;
    }

    void  Instance::add           ( Term* term){
        if(getTerm(term->getName())!= NULL) return;
        terms_.push_back(term);
    }

    void  Instance::remove        ( Term* term){
        for ( std::vector<Term*>::iterator it = terms_.begin() ; it != terms_.end() ; ++it ) {
            if (*it == term) terms_.erase( it );
        }
    }

    void  Instance::setPosition   ( const Point& pos){
        position_ = pos;
    }

    void  Instance::setPosition   ( int x, int y ){
        position_ = Point(x,y);
    }

    void  Instance::toXml ( std::ostream& stream ){
        stream << indent << "<instance name=\"" << name_ << "\" mastercell=\"" << masterCell_->getName() << "\"" <<" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
    }
}