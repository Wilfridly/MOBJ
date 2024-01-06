#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include "Box.h"
#include "Symbol.h"

namespace Netlist {
    
    class Shape{
        public:
                            Shape         ( Symbol*);
            virtual         ~Shape        ();
            virtual Box     getBoundingBox()const=0;
                    Symbol* getSymbol     ()const;
            static  Shape*  fromXml       ( Symbol*, xmlTextReaderPtr);
            virtual void    toXml         ( std::ostream& stream )const = 0;
        private:
            Symbol* owner_;
    };

//BOXSHAPE
    class BoxShape : public Shape { 
        public :
                                        BoxShape            ( Symbol*, const Box& );
                                        BoxShape            ( Symbol*, int x1 , int y1 , int x2 , int y2 );
                                        ~BoxShape           ();
                    Box                 getBoundingBox      () const ;
            void                        toXml               ( std::ostream& stream )const;
            static  Shape*              fromXml             ( Symbol*, xmlTextReaderPtr);

        private :
            Box box_;
    };






//TERMSHAPE
    class TermShape : public Shape {
        public :
            enum NameAlign { TopLeft=1, TopRight, BottomLeft, BottomRight, Unknown};
                                TermShape       ( Symbol *, Term*, int, int, NameAlign);
                                ~TermShape      ();
                   Box          getBoundingBox  () const ;
            inline Term*        getTerm         () const{ return term_;};
            inline int          getX            () const{ return x_;};
            inline int          getY            () const{ return y_;};
            inline NameAlign    getNameAlign    () const{ return align_;};
            static std::string  toString        ( NameAlign );
            static NameAlign    toNameAlign     ( std::string );
            void                toXml           ( std::ostream& stream )const;
            static  Shape*      fromXml         ( Symbol*, xmlTextReaderPtr);

        private :
            Term* term_ ;
            int x_ , y_ ;
            NameAlign align_;
    };




    
//LINESHAPE
    class LineShape : public Shape {
        public :
                                    LineShape       ( Symbol *, int x1 , int y1 , int x2 , int y2 );
                                    ~LineShape      ();
                Box                 getBoundingBox  () const ;
        void                        toXml           ( std::ostream& stream )const;
        static  Shape*              fromXml         ( Symbol*, xmlTextReaderPtr);
        inline  bool                getSens         () const{ return sens_;}
        private :
            int x1_ , y1_ , x2_ , y2_;
            bool sens_;
    };




    
//ELLIPSESHAPE
    class EllipseShape : public Shape{
        public : 
                                        EllipseShape        ( Symbol*, const Box& );
                                        // EllipseShape        ( Symbol*, int x1 , int y1 , int x2 , int y2 );
                                        ~EllipseShape       ();
                    Box                 getBoundingBox      () const ;
            void                        toXml               ( std::ostream& stream )const;
            static  Shape*              fromXml             ( Symbol*, xmlTextReaderPtr);
        private :
            Box box_;
    };




    
//ARCSHAPE
    class ArcShape : public Shape{
        public : 
                                        ArcShape        ( Symbol*, const Box&, int start, int span);
                                        // ArcShape        ( Symbol*, int x1 , int y1 , int x2 , int y2 );
                                        ~ArcShape       ();
                    Box                 getBoundingBox  () const ;
            void                        toXml           ( std::ostream& stream )const;
            static  Shape*              fromXml         ( Symbol*, xmlTextReaderPtr);
            int                         getStart        () const;
            int                         getSpan         () const;
        private :
            Box box_;
            int start_;
            int span_;
    };
}

#endif