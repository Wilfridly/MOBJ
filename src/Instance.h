#ifndef NETLIST_INSTANCE_H
#define NETLIST_INSTANCE_H

#include <string>
#include <vector>

#include "Cell.h"
#include "Term.h"
#include "Point.h"
#include "Net.h"

namespace Netlist{
    class Point;

    class Instance{
        private :
            Cell*               owner_;
            Cell*               masterCell_;
            std::string         name_;
            std::vector<Term*>  terms_;
            Point               position_;
        public :
            //Constructeurs & Destructeur
            Instance      ( Cell* owner, Cell* model, const std::string& );
            ~Instance      ();

            //Accessseurs
            const std::string&        getName       () const;
            Cell*                     getMasterCell () const;
            Cell*                     getCell       () const;
            const std::vector<Term*>& getTerms      () const;
            Term*                     getTerm       ( const std::string& ) const;
            Point                     getPosition   () const;

            //Modificateurs
            bool  connect       ( const std::string& name, Net* );
            void  add           ( Term* );
            void  remove        ( Term* );
            void  setPosition   ( const Point& );
            void  setPosition   ( int x, int y );
    };
}
#endif