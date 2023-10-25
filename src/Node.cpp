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
    stream << indent << "<node term=\"" << term_->getName()  << "\" " << " id=\"" << getId() << "\"" << " x=\"" << position_.getX() << "\""<< " y=\"" << position_.getY() << "\">\n";
    stream << indent << "<node term=\"" << term_->getName()  << "\" " << " instance=\"" << getId() << "\"" << " id=\"" << getId() << "\"" << " x=\"" << position_.getX() << "\""<< " y=\"" << position_.getY() << "\">\n";
    
  }

}  // Netlist namespace.
