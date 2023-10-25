#ifndef NETLIST_NET_H
#define NETLIST_NET_H

#include <vector>
#include <string>

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
        public : 
            //constructeur & destructeur
            Net     ( Cell*, const std::string&, Term::Type ); 
            ~Net    ();  
            
            //Accesseurs
            Cell*                     getCell       () const;
            const std::string&        getName       () const;
            unsigned int              getId         () const;
            Term::Type                getType       () const;
            const std::vector<Node*>& getNodes      () const;
            size_t                    getFreeNodeId () const;
            void                      toXml         ( std::ostream& stream );
            //Modificateurs 
            void  add    ( Node* );
            bool  remove ( Node* );
    };
}
#endif 