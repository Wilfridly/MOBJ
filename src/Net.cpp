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
        lines_ = std::vector<Line*>();
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

        // Methode avec insert
        size_t indice = node->getId();
        if(indice == Node::noid){ //si Id = 0, ajouter sur le noeud
            indice = getFreeNodeId();
            nodes_.insert(nodes_.begin()+indice,node);
            node->setId(indice);
        }
        else{
            nodes_.resize(indice);
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

    void Net::add ( Line* line ){
        if (line) lines_.push_back( line ); 
    }

    bool Net::remove ( Line* line ){
        if (line) {
            for ( vector<Line*>::iterator il = lines_.begin(); il != lines_.end() ; ++il ) {
                if (*il == line) {
                    lines_.erase( il );
                    return true;
                }
            }
        }
        return false;
    }

    void  Net::toXml ( std::ostream& stream ){ // A MODIFIER
        stream << indent << "<net name=\"" << name_ << "\" type=\"" << Term::toString(type_) << "\">\n";
        ++indent;
        for(std::vector<Node*>::iterator it = nodes_.begin() ; it != nodes_.end() ; ++it ){
            if((*it) != NULL)
                (*it)->toXml(stream);
        }    
        --indent;
        stream << indent << "</net>\n";

    }

    Net* Net::fromXml( Cell* cell, xmlTextReaderPtr reader){ //A MODIFIER
        Net* net = NULL;
       
        const xmlChar* nodeTag = xmlTextReaderConstString(reader,(const xmlChar*)"node");
        std::string name = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"name"));
        std::string type = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"type"));

        Term::Type t;

        (type == "Internal" ) ? t = Term::Internal : t = Term::External;

        net = new Net(cell,name,t);
        
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
                if(Node::fromXml(net,reader)) {
                    std::cout << "AYAYA" << std::endl;
                    continue;
                }
            }
            break;
        }
        return net;
    }
}