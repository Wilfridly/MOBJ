// -*- explicit-buffer-name: "Main.cpp<M1-MOBJ/4-5>" -*-

#include <string>
#include <sstream>
#include <iostream>
#include <memory>
using namespace std;

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
using namespace Netlist;


int main ( int argc, char* argv[] )
{
  {
    cout << "\nConstruction du modele <and2>." << endl;
    Cell* and2 = new Cell ( "and2" );
    new Term( and2, "i0", Term::In  );
    new Term( and2, "i1", Term::In  );
    new Term( and2,  "q", Term::Out );
    and2->toXml( cout );

    cout << "\nConstruction du modele <or2>." << endl;
    Cell* or2 = new Cell ( "or2" );
    new Term( or2, "i0", Term::In  );
    new Term( or2, "i1", Term::In  );
    new Term( or2,  "q", Term::Out );
    or2->toXml( cout );

    cout << "\nConstruction du modele <xor2>." << endl;
    Cell* xor2 = new Cell ( "xor2" );
    new Term( xor2, "i0", Term::In  );
    new Term( xor2, "i1", Term::In  );
    new Term( xor2,  "q", Term::Out );
    xor2->toXml( cout );

    cout << "\nConstruction du modele <halfadder>." << endl;
    Cell* halfadder = new Cell ( "halfadder" );
    new Term( halfadder, "a"   , Term::In  );
    new Term( halfadder, "b"   , Term::In  );
    new Term( halfadder, "sout", Term::Out );
    new Term( halfadder, "cout", Term::Out );
    Net*      ha_a    = new Net      ( halfadder, "a"   , Term::External );
    Net*      ha_b    = new Net      ( halfadder, "b"   , Term::External );
    Net*      ha_sout = new Net      ( halfadder, "sout", Term::External );
    Net*      ha_cout = new Net      ( halfadder, "cout", Term::External );
    Instance* ha_xor2 = new Instance ( halfadder, Cell::find("xor2"), "xor2_1" );
    Instance* ha_and2 = new Instance ( halfadder, Cell::find("and2"), "and2_1" );
    halfadder->connect( "a"   , ha_a    );
    halfadder->connect( "b"   , ha_b    );
    halfadder->connect( "sout", ha_sout );
    halfadder->connect( "cout", ha_cout );
    ha_xor2->connect( "i0", ha_a    );
    ha_xor2->connect( "i1", ha_b    );
    ha_xor2->connect(  "q", ha_sout );
    ha_and2->connect( "i0", ha_a    );
    ha_and2->connect( "i1", ha_b    );
    ha_and2->connect(  "q", ha_cout );
    halfadder->toXml( cout );

  }

  return 0;
}

