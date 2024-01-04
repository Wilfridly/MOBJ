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
    Term::Term (Cell* cell,const std::string& name, Direction d):node_(this,Node::noid){ //ctor
        owner_ = static_cast<void*>(cell); //static cast
        name_ = name;
        direction_ = d;
        type_ = External;
        net_ = NULL;
        cell->add(this);
        // std::cout << "Debug::Term CTOR : " << name_ << std::endl;
    }

    Term::Term ( Instance* instance, const Term* modelTerm ):node_(this,Node::noid){ //ctor2
        owner_ = static_cast<void*>(instance);
        name_ = modelTerm->name_;
        direction_ = modelTerm->direction_;
        type_ = Internal;
        net_ = modelTerm->net_;
        instance->add(this);
        // std::cout << "Debug::Term2 CTOR" << std::endl;
    }

    Term::~Term (){ //dtor
        if(type_ == External){
            static_cast<Cell*>(owner_)->remove(this);
        }
        else static_cast<Instance*>(owner_)->remove(this);
        // std::vector<Net*> netss = this->getNets();
        // while ( not netss_.empty() ) delete *netss_.begin();

    }
                
    std::string Term::toString (Type t){
        return((t == 1) ? std::string("Internal") : std::string("External"));
    }

    std::string Term::toString (Term::Direction d){
        switch (d){
            case 1:
                return(std::string("In"));
            case 2:
                return(std::string("Out"));
            case 3:
                return(std::string("Inout"));
            case 4:
                return(std::string("Tristate"));
            case 5:
                return(std::string("Transcv"));
            default:
                return(std::string("Unknown"));     
        }
    }
    Term::Direction Term::toDirection(std::string str){
        if(str == "In") return (In);
        if(str == "Out") return (Out);
        if(str == "Inout") return (Inout);
        if(str == "Tristate") return (Tristate);
        if(str == "Transcv") return (Transcv);
        else return(Unknown);
    }

    //Prédicats et accesseurs

    Cell* Term::getOwnerCell () const{ //si cell, renvoie l'owner, sinon on cherche l'owner par instance
        if(type_ == External) return static_cast<Cell*>(owner_);
        if(type_ == Internal) return static_cast<Instance*>(owner_)->getCell();
        else return NULL;
    }

    // Modificateurs
    void Term::setNet( Net* net){
        if(net == NULL){ //test si le net existe
            net_ = NULL;
            if(node_.getId() != Node::noid)
                net_->remove(&node_);
            if(type_ == External) 
                static_cast<Cell*>(owner_)->remove(net); 
        }
        else{ //set le net
            net_ = net;
            // std::cout << "ça marche le set net ?" << std::endl;
            net_->add(&node_);
        }
    }

    void  Term::setNet( const std::string& name){
        Net* net = getOwnerCell()->getNet(name);
        if(net == NULL){ //test si le net existe
            // std::cout << "[ERROR]" << name << " non trouvé " << std::endl;
            exit(1);
        }
        //set le net
        net_->add(&node_);
        net_ = net;
    }


    void  Term::setPosition( const Point& p ){
        node_.setPosition(p);
    }

    void  Term::setPosition( int x, int y ){
        node_.setPosition(x,y);
    }
    
    void  Term::toXml ( std::ostream& stream ){
        stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_) << "\" x=\"" << getPosition().getX() << "\" y=\""<< getPosition().getY() << "\"/> \n" ;    
    }

    Term* Term::fromXml(Cell* cell, xmlTextReaderPtr reader)
    {
        const std::string termName =        xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"name"));
        const std::string termdirection =   xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"direction"));
        const std::string termX =           xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"x"));
        const std::string termY =           xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"y"));

        Term::Direction d = toDirection(termdirection);
        Term* term = new Term(cell, termName, d);

        term->setPosition(atoi(termX.c_str()),atoi(termY.c_str()));
        
        if(termName.empty())
            return NULL;
        return term;
    }
}