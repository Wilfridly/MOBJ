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
    }

    Term::Term ( Instance* instance, const Term* modelTerm ):node_(this,Node::noid){
        owner_ = static_cast<void*>(instance);
        name_ = modelTerm->name_;
        direction_ = modelTerm->direction_;
        type_ = Internal;
        net_ = modelTerm->net_;
    }

    Term::~Term (){}
                
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
            net_ = net;
            net_->add(&node_);
        }

        void  Term::setNet( const std::string& name){
            name_ = name;
            //net_ = 

        }

        void  Term::setPosition( const Point& p ){
            p.getX();
            p.getY();
        }
        void  Term::setPosition( int x, int y ){
            
        }
}