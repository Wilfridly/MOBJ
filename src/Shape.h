#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include "Box.h"
#include "Symbol.h"

namespace Netlist {
    class shape{
        public:
            Shape(Symbol*);
            ~Shape();
            virtual Box getBoundingBox()const=0;
            Symbol* getSymbol()const;
        private:
            Symbol* owner_;
    };

    class LineShape{
        private:
        int x1_;
        int y1_;
        int x2_;
        int y2_;
    }

    class BoxShape{
        private :
        Box box_;
    }

    class EllipseShape{
        private :
        Box::Box box_;
    }

    class ArcShape{
        private :
        Box::Box box_;
        int start_;
        int span_;
    }

    class TermShape{
        private :
        int x1_;
        int y1_;
        Term::Term term_;
    }

}

#endif