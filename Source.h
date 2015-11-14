#ifndef _SOURCE_H
#define _SOURCE_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Source
{
    public:
    Source();

    virtual Vect getLightPosition()
    {
        return Vect(0,0,0);
    }
    //method functions
    virtual Color getColor()
    {
        return Color (1.0,1.0,1.0,0.0);
    }

};

Source::Source()
{
}

#endif
