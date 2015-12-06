#ifndef _ESSENTIALS_H
#define _ESSENTIALS_H

#include<vector>
#include<fstream>
#include "Object.h"
#include "Source.h"
#include "Vect.h"
#include "Color.h"

using namespace std;

struct RGBType
{
    float r;
    float g;
    float b;
};

int winningObjectIndex(vector<double> object_intersections);

Color getColorAt(Vect intersection_position,
                 Vect intersecting_ray_direction,
                 vector<Object*> scene_objects,
                 int index_of_winning_object,
                 vector<Source*> light_sources,
                 double accuracy,
                 double ambientlight);

void savebmp (const char *filename, int w, int h, int dpi, RGBType *data);

#endif
