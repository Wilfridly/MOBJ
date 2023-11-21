// -*- explicit-buffer-name: "Node.cpp<M1-MOBJ/4-5>" -*-

#include  <limits>
#include  "Node.h"
#include  "Term.h"
#include  "Net.h"
#include  "Instance.h"
#include  "Cell.h"

namespace Netlist {

  using namespace std;


  const size_t  Node::noid = numeric_limits<size_t>::max();


  Node::Node ( Term* term, size_t id  )
    : id_      (id)
    , term_    (term)
    , position_()
  { }


  Node::~Node ()
  {
    if (getNet()) getNet()->remove( this );
  }


  inline Net* Node::getNet () const { return term_->getNet(); }


// Node::toXml() à écrire ici.
  void  Node::toXml ( std::ostream& stream ){
    if(term_->isInternal()==true){
      stream << indent <<"<node term=\"" << term_->getName()  << "\" " << " instance=\"" << term_->getInstance()->getName();
      stream << "\"" << " id=\"" << getId() << "\"/>\n";
    }
    else{
      stream << indent << "<node term=\"" << term_->getName()  << "\" " << " id=\"" << getId();
      stream << "\"/>\n";
    }
  }

  bool Node::fromXml(Net* net, xmlTextReaderPtr reader){ //Non fini
    const std::string termName = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"term"));
    const std::string instName = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"instance"));
    const std::string id       = xmlCharToString(xmlTextReaderGetAttribute(reader,(const xmlChar*)"id"));
        
    
    if(instName.empty()){
      std::cout << "non" << std::endl;
      net->getCell()->getTerm(termName)->getNode()->setId(atoi(id.c_str()));
      std::cout << "non" << std::endl;
      
      if(net->getCell()->connect(termName,net) != true) return false;
      std::cout << "non" << std::endl;

    }
    else{
      std::cout << "oui" << std::endl;
      net->getCell()->getInstance(instName)->getTerm(termName)->getNode()->setId(atoi(id.c_str()));
      if(net->getCell()->getInstance(instName)->connect(termName,net) != true) return false;
    }
    std::cout << "oui2" << std::endl;

    return true;
  }
}  // Netlist namespace.
