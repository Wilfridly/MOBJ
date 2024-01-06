#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H

#include <string>
#include  <libxml/xmlreader.h>

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
            inline bool               isInternal   () const{return (type_ == Internal) ? true : false;}
            inline bool               isExternal   () const{return (type_ == External) ? true : false;}
            inline const std::string& getName      () const{return name_;}
            inline NodeTerm*          getNode      () {return &node_;}
            inline Net*               getNet       () const {return net_;}
            inline Cell*              getCell      () const {return (type_ == External) ? static_cast<Cell*>(owner_) : NULL;}
                   Cell*              getOwnerCell () const;
            inline Instance*          getInstance  () const {return (type_ == Internal) ? static_cast<Instance*>(owner_) : NULL;}
            inline Direction          getDirection () const {return direction_;}
            inline Point              getPosition  () const {return node_.getPosition();}
            inline Type               getType      () const {return type_;}
                
            void                      toXml        ( std::ostream& stream );
            static Term*              fromXml      ( Cell*, xmlTextReaderPtr);
            //Modificateurs
                void        setNet       ( Net* );
                void        setNet       ( const std::string& );
            inline void     setDirection ( Direction d){direction_= d;}
                void        setPosition  ( const Point& );
                void        setPosition  ( int x_, int y_);
        private :
            void*         owner_;
            std::string   name_;
            Direction     direction_;
            Type          type_;
            Net*          net_;
            NodeTerm      node_;
    };
}
#endif 