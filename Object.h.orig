#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object //Class that all objects derives from with variables and functions that all of them should have
{
    public:
    Object();
    Color color;

    //method functions
    virtual Color getColor()
    {
        return color;
    }

    virtual double findIntersection(Ray ray)
    {
        return 0;
    }

    virtual Vect getNormalAt(Vect point)
    {
        return Vect (0, 0, 0);
    }

};

Object::Object()
{
    color = Color (0.0,0.0,0.0,0.0);
}

#endif
