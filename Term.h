#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H

#include <string>
#include "Point.h"
#include "Node.h"


namespace Netlist {
    class Cell;
    class Net;
    class Instance;
    class Term{
        public :
            enum Type      { Internal=1, External=2 };
            enum Direction { In=1, Out=2, Inout=3, Tristate=4, Transcv=5, Unknown=6 };

            Term ( Cell*    , const std::string& name, Direction ); //constructeur
            Term ( Instance*, const Term* modelTerm );
            ~Term (); // destructeur
                       
            static std::string  toString    ( Type );
            static std::string  toString    ( Direction );
            static Direction    toDirection ( std::string );

            //Prédicats et accesseurs
            inline bool               isInternal   () const{return true;}
            inline bool               isExternal   () const{return true;}
            inline const std::string& getName      () const{return name_;}
            inline Node*              getNode      () {return NULL;}
            inline Net*               getNet       () const {return NULL;}
            inline Cell*              getCell      () const {return NULL;}
                   Cell*              getOwnerCell () const;
            inline Instance*          getInstance  () const {return NULL;}
            inline Direction          getDirection () const {return In;}
            inline Point              getPosition  () const {return Point(0,0);}
            inline Type               getType      () const {return type_;}

            //Modificateurs
                void  setNet       ( Net* );
                void  setNet       ( const std::string& );
            inline void  setDirection ( Direction d);
                void  setPosition  ( const Point& );
                void  setPosition  ( int x_, int y_);
        private :
            void*         owner_;
            std::string   name_;
            Direction     direction_;
            Type          type_;
            Net*          net_;
            Node          node_;
    };
}
#endif 