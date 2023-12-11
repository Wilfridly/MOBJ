#ifndef NETLIST_NET_H
#define NETLIST_NET_H

#include <vector>
#include <string>
#include "Line.h"
#include "Cell.h"
#include "Term.h"
#include "Node.h"

namespace Netlist{
    class Net{
        private :
            Cell*               owner_;
            std::string         name_;
            unsigned int        id_;
            Term::Type          type_;
            std::vector<Node*>  nodes_;
            std::vector<Line*>  lines_;
        public : 
            //constructeur & destructeur
            Net     ( Cell*, const std::string&, Term::Type ); 
            ~Net    ();  
            
            //Accesseurs
            Cell*                     getCell       () const;
            const std::string&        getName       () const;
            unsigned int              getId         () const;
            Term::Type                getType       () const;
            const Node*               getNode       (int&) const;
            const std::vector<Node*>& getNodes      () const;
            size_t                    getFreeNodeId () const;
            
            //Modificateurs 
            void  add    ( Node* );
            bool  remove ( Node* );
            
            //Methodes TME7
            void  add    ( Line* );
            bool  remove ( Line* );
            inline const std::vector<Line*>& getLines () const { return lines_;} //TME7
            
            //gestionnaire des xmls
            static Net* fromXml( Cell*, xmlTextReaderPtr);
                   void toXml  ( std::ostream& stream   ); 
    };
}
#endif 