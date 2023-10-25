#include <iostream>
#include <string>

#include "Cell.h"
#include "Term.h"
#include "Point.h"
#include "Instance.h"
#include "Net.h"


namespace Netlist{
    Term::Term (Cell* cell,const std::string& name, Direction d):node_(this,Node::noid){
        owner_ = static_cast<void*>(cell); //static cast
        name_ = name;
        direction_ = d;
        type_ = External;
        net_ = NULL;
        cell->add(this);
        // std::cout << "Debug::Term CTOR : " << name_ << std::endl;
    }

    Term::Term ( Instance* instance, const Term* modelTerm ):node_(this,Node::noid){
        owner_ = static_cast<void*>(instance);
        name_ = modelTerm->name_;
        direction_ = modelTerm->direction_;
        type_ = Internal;
        net_ = modelTerm->net_;
        instance->add(this);
        // std::cout << "Debug::Term2 CTOR" << std::endl;
    }

    Term::~Term (){
        if(type_ == External){
            static_cast<Cell*>(owner_)->remove(this);
        }
        else static_cast<Instance*>(owner_)->remove(this);
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

    Cell* Term::getOwnerCell () const{
        if(type_ == External) return static_cast<Cell*>(owner_);
        if(type_ == Internal) return static_cast<Instance*>(owner_)->getCell();
        else return NULL;
    }

    //Modificateurs
    void Term::setNet( Net* net){
        if(net == NULL){
            net_ = NULL;
            if(node_.getId() != Node::noid)
                net_->remove(&node_); 
        }
        else{
            net_ = net;
            net_->add(&node_);
        }
    }

    void  Term::setNet( const std::string& name){
        Net* net = getOwnerCell()->getNet(name);
        if(net == NULL){
            std::cout << "[ERROR]" << name << " non trouvé " << std::endl;
            exit( 1 );
        }
        net_ = net;
        net_->add(&node_);
    }

    void  Term::setPosition( const Point& p ){
        node_.setPosition(p);
    }

    void  Term::setPosition( int x, int y ){
        node_.setPosition(x,y);
    }
    
    void  Term::toXml ( std::ostream& stream ){
        stream << indent << "<term name=\"" << name_ << "\" direction=\"" << toString(direction_) << "\"/>\n";    
    }

}