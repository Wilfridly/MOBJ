#include <string>
#include <vector>
#include <cstdlib>
#include  <libxml/xmlreader.h>

#include "Term.h"
#include "Cell.h"
#include "Net.h"
#include "Node.h"
#include  "XmlUtil.h"



namespace Netlist{
    Net::Net( Cell* owner, const std::string& name, Term::Type t ){ //ctor
        owner_ = owner;
        name_ = name;
        id_ = Node::noid;
        type_ = t;
        nodes_ = std::vector<Node*>();
        owner_->add(this);
        // std::cout << "debug::Net CTOR" << std::endl;
    }
    
    Net::~Net(){ //dtor
        owner_->remove(this);
        // std::cout << "debug::Net::dtor NET" << std::endl;
    }  
    
    //Accesseurs
    Cell* Net::getCell()                        const{return owner_;}
    const std::string& Net::getName()           const{return name_;}
    unsigned int Net::getId()                   const{return id_;}
    Term::Type Net::getType()                   const{return type_;}
    const std::vector<Node*>& Net::getNodes()   const{return nodes_;}
    
    size_t Net::getFreeNodeId () const{ //recherche d'une place libre
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

        //Methode avec pushback
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

        // Methode avec insert
        size_t indice = node->getId();
        if(indice == Node::noid){ //si Id = 0, ajouter sur le noeud
            indice = getFreeNodeId();
            nodes_.insert(nodes_.begin()+indice,node);
            node->setId(indice);
        }
        else{
            nodes_.insert(nodes_.begin()+indice,node);
        }
    }
    
    bool  Net::remove( Node* node){ //enlever un noeud
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
        stream << indent << "<net name=\"" << name_ << "\" type=\"" << Term::toString(type_) << "\"/>\n";
        ++indent;
        for(std::vector<Node*>::iterator it = nodes_.begin() ; it != nodes_.end() ; ++it ){
            (*it)->toXml(std::cout);
        }    
        --indent;
        stream << indent << "</net>\n";

    }

    Net* Net::fromXml( Cell* cell, xmlTextReaderPtr reader){ //NON FINI
        
        const std::string name = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"name"));
        const std::string type = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"type"));
        const xmlChar* nodeTag = xmlTextReaderConstString(reader,(const xmlChar*)"node");

        Term::Type t;

        (type == "Internal" )? t = Term::Internal : t = Term::External;

        Net* net = new Net(cell,name,t);
        
        if(name.empty()||type.empty()) return NULL;
        
        while(true){
            int status = xmlTextReaderRead(reader);
            if (status != 1) {
                if (status != 0) {
                    std::cout << "[ERROR] Cell::fromXml(): Unexpected termination of the XML parser." << std::endl;
                }
                break;
            }
            switch ( xmlTextReaderNodeType(reader) ) {
                case XML_READER_TYPE_COMMENT:
                case XML_READER_TYPE_WHITESPACE:
                case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }

            const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
            if(nodeName == nodeTag){
                // if(Node::fromXml(net,reader)) 
                    continue;
            }
            else
                break;
        }
        
        return net;
    }
}