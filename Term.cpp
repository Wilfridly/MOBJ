#include <iostream>
#include <string>

#include "Cell.h"
#include "Term.h"
#include "Point.h"


namespace Netlist{
    Term::Term ( Cell* Cell   , const std::string& name, Direction d):node_(this,Node::noid){}
    Term::Term ( Instance* Instance, const Term* modelTerm ):node_(this,Node::noid){}
    Term::~Term (){}
                
    std::string Term::toString (Type){
        return "oui";
        //return((t == 1) ? std::string("Internal") : std::string("External"));
    }

    std::string Term::toString (Term::Direction d){
        return "oui";
        // switch (d){
        //     case 1:
        //         return(std::string("In"));
        //     case 2:
        //         return(std::string("Out"));
        //     case 3:
        //         return(std::string("Inout"));
        //     case 4:
        //         return(std::string("Tristate"));
        //     case 5:
        //         return(std::string("Transcv"));
        //     default:
        //         return(std::string("Unknown"));     
        // }
    }
    Term::Direction Term::toDirection(std::string str){
        // if(str == "In") return(1);
        // if(str == "Out") return (2);
        // if(str == "Inout") return (3);
        // if(str == "Tristate") return (4);
        // if(str == "Transcv") return (5);
        // else return(6);
        return In;
    }

    //Prédicats et accesseurs

    Cell*     Term::getOwnerCell () const{return NULL;}

    //Modificateurs
        void  Term::setNet       ( Net* ){}
        void  Term::setNet       ( const std::string& ){}
        void  Term::setPosition  ( const Point& ){}
        void  Term::setPosition  ( int x, int y ){}
}