#include <string>
#include <vector>

#include "Term.h"
#include "Cell.h"
#include "Net.h"
#include "Node.h"


namespace Netlist{
    Net::Net( Cell* owner, const std::string& name, Term::Type t ){
        owner_ = owner;
        name_ = name;
        id_ = Node::noid;
        type_ = t;
        nodes_ = std::vector<Node*>();
        std::cout << "debug::Net::ctor" << std::endl;
    }
    
    Net::~Net(){
        std::cout << "debug::Net::dtor" << std::endl;
    }  
    
    //Accesseurs
    Cell* Net::getCell() const{
        return owner_;
    }

    const std::string& Net::getName() const{
        return name_;
    }

    unsigned int Net::getId() const{
        return id_;
    }

    Term::Type Net::getType() const{
        return type_;
    }

    const std::vector<Node*>& Net::getNodes() const{
        return nodes_;
    }
    
    size_t Net::getFreeNodeId () const{
        for(size_t i = 0; i < nodes_.size();i++){
            if(nodes_[i]== NULL)
                return i;
        }
        return nodes_.size();
    }

    //Modificateurs 
    void  Net::add( Node* node){
        if(node->getId() != Node::noid)
        {
            nodes_[node->getId()];
        }
        else{
            size_t indice = getFreeNodeId();
            if (nodes_.size() != indice)
                nodes_[indice]= node;
            else
                nodes_.push_back(node);
        }
        std::cout << "debug::Net::add(Node* node)" << std::endl;
    }
    
    bool  Net::remove( Node* node){
        return true;
    }
}