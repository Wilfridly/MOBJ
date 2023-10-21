#include <string>
#include <vector>

#include "Term.h"
#include "Cell.h"
#include "Net.h"
#include "Node.h"


namespace Netlist{
    Net::Net     ( Cell*, const std::string&, Term::Type ){} 
    Net::~Net     (){}  
    
    //Accesseurs
    Cell*               Net::getCell       () const{return NULL;}
    const std::string&        Net::getName       () const{return "Hello";}
    unsigned int              Net::getId         () const{return 3;}
    Term::Type                     Net::getType       () const{return Term::Internal;}
    const std::vector<Node*>& Net::getNodes      () const{return std::vector<Node*>();}
    size_t                    Net::getFreeNodeId () const{return NULL;}

    //Modificateurs 
    void  Net::add    ( Node* ){}
    bool  Net::remove ( Node* ){return true;}
}