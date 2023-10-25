#include <string>
#include <vector>
#include <cstdlib>
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
        owner_->add(this);
        // std::cout << "debug::Net CTOR" << std::endl;
    }
    
    Net::~Net(){
        owner_->remove(this);
        // std::cout << "debug::Net::dtor NET" << std::endl;
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
            if(nodes_[i]== NULL) {
                // std::cout<< "debug::Net::getFreeNodeId ()"<< std::endl;
                return i;
            }
        }
        // std::cout<< "debug::Net::getFreeNodeId ()"<< std::endl;
        return nodes_.size();
    }

    //Modificateurs 
    void  Net::add( Node* node){
        // std::cout<<"node added with sucess"<<std::endl;
        // if(node->getId() != Node::noid) 
        //     nodes_[node->getId()];
        // else{
        //     size_t indice = getFreeNodeId();
        //     if (nodes_.size() != indice){ 
        //         nodes_[indice]= node;
        //         node->setId(indice);
        //     }
        //     else{
        //         nodes_.push_back(node);
        //         node->setId(indice);   
        //     }           
        // }

        size_t indice = node->getId();
        if(indice == Node::noid){
            indice = getFreeNodeId();
            nodes_.insert(nodes_.begin()+indice,node);
            node->setId(indice);
        }
        else{
            nodes_.insert(nodes_.begin()+indice,node);
        }
        std::cout << "debug::Net::add(Node* node)" << std::endl;
    }
    
    bool  Net::remove( Node* node){
        for(size_t i = 0; i < nodes_.size();i++){
            if(nodes_[i] == node){
                node->setId(Node::noid);
                node->getTerm()->setNet(NULL);
                nodes_[i] = NULL;
                // std::cout << "debug::Net::remove()true" << std::endl;
                return true;
            }
        }
        // std::cout << "debug::Net::remove()false" << std::endl;
        return false;
    }

    void  Net::toXml ( std::ostream& stream ){
        stream << --indent << "<net name=\"" << name_ << "\" type=\"" << Term::toString(type_) << "\"/>\n";
        indent++;
        
        for(std::vector<Node*>::iterator it = nodes_.begin() ; it != nodes_.end() ; ++it ){
            (*it)->toXml(std::cout);
        }    
    }
}