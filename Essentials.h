#ifndef _ESSENTIALS_H_
#define _ESSENTIALS_H_

#include<vector>

using namespace std;


struct RGBType
{
    float r;
    float g;
    float b;
};

//Function to output the data to a .bmp file
void savebmp (const char *filename, int w, int h, int dpi, RGBType *data)
{
    FILE *f;
    int k = w*h;
    int s = 3*k;
    int filesize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;

    unsigned char bmpfileheader[14] = {'B', 'M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

    bmpfileheader[ 2] = (unsigned char )(filesize);
    bmpfileheader[ 3] = (unsigned char )(filesize>>8);
    bmpfileheader[ 4] = (unsigned char )(filesize>>16);
    bmpfileheader[ 5] = (unsigned char )(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(w);
    bmpinfoheader[ 5] = (unsigned char)(w>>8);
    bmpinfoheader[ 6] = (unsigned char)(w>>16);
    bmpinfoheader[ 7] = (unsigned char)(w>>24);

    bmpinfoheader[ 8] = (unsigned char)(h);
    bmpinfoheader[ 9] = (unsigned char)(h>>8);
    bmpinfoheader[10] = (unsigned char)(h>>16);
    bmpinfoheader[11] = (unsigned char)(h>>24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);

    f=fopen(filename,"wb");
    if (f!=NULL)
    {
        fwrite(bmpfileheader,1,14,f);
        fwrite(bmpinfoheader,1,40,f);

        for(int i = 0; i < k; i++)
        {
            double red = (data[i].r)*255;
            double green = (data[i].g)*255;
            double blue = (data[i].b)*255;

            unsigned char color[3]={(unsigned char)floor(blue),(unsigned char)floor(green),(unsigned char)floor(red)};

            fwrite(color,1,3,f);
        }
    }
    else
    {
        cout << "It is not possible to open the output file!" <<endl;
    }


    fclose(f);
}

// return the index of the winning intersecition, that is, the closest intersection to the camera
int winningObjectIndex(vector<double> object_intersections)
{
        int index_of_minimum_value;

    //prevent unnessary calculations
    if (object_intersections.size() == 0)
    {
        //if there are no intersections
        return -1;
    }
    else if(object_intersections.size() == 1)
    {
        //
        if(object_intersections.at(0) > 0)
        {
            //if that intersection is greater than zero then its our
            //index of minimum value
            return 0;
        }
        else
        {
            //otherwise the only intersection value is negative
            return -1;
        }
    }
    else
    {
        //otherwise there is more than one intersection
        //first find the maximum value
        double maxim = 0;
        for (unsigned i = 0; i < object_intersections.size(); i++)
        {
            if (maxim < object_intersections.at(i))
            {
                maxim = object_intersections.at(i);
            }
        }
        // the starting from the maxmimum value to find the minimum positive value
        if (maxim > 0)
        {
            //we only want positive intersections
            for (unsigned index = 0; index < object_intersections.size(); index++)
            {
                if (object_intersections.at(index) > 0 && object_intersections.at(index) <= maxim)
                {
                    maxim = object_intersections.at(index);
                    index_of_minimum_value = index;
                }
            }
            return index_of_minimum_value;
        }
        else
        {
            //all the intersections were negative
            return -1;
        }
    }

}

//returns the color at a coordinate based on the intersection position of a ray
Color getColorAt(Vect intersection_position,
                 Vect intersecting_ray_direction,
                 vector<Object*> scene_objects,
                 int index_of_winning_object,
                 vector<Source*> light_sources,
                 double accuracy,
                 double ambientlight)
{
    Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
    Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

    if(winning_object_color.getColorSpecial() == 2)
    { // checkered/tile floor pattern
        int square = (int)floor((intersection_position).getVectX())+
                     (int)floor((intersection_position).getVectZ());
        if((square%2) != 0)
        {
            //white tile
            winning_object_color.setColorBlue(1);
            winning_object_color.setColorGreen(1);
            winning_object_color.setColorRed(1);
        }
    }

    Color final_color = winning_object_color.colorScalar(ambientlight);

    if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1)
    { //reflection from objects with specular intensity
        double dot1 = winning_object_normal.dotProduct(
                        intersecting_ray_direction.negative());
        Vect scalar1 = winning_object_normal.vectMult(dot1);
        Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
        Vect scalar2 = add1.vectMult(2);
        Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
        Vect reflection_direction = add2.normalize();

        Ray reflection_ray (intersection_position,reflection_direction);

        //determine what the ray intersects with first
        vector<double> reflection_intersections;

        for (unsigned reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++)
        {
            reflection_intersections.push_back(scene_objects.at(reflection_index)->
                                               findIntersection(reflection_ray));
        }

        int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

        if (index_of_winning_object_with_reflection != -1)
        {   //reflection ray missed everything else
            if (reflection_intersections.at(index_of_winning_object_with_reflection)>accuracy)
            { // determine the position and direction at the point of intersection with
                //the reflection ray. the ray only affects the color if it reflected off something
                Vect reflection_intersection_position = intersection_position.vectAdd(
                                                        reflection_direction.vectMult(
                                                        reflection_intersections.at(
                                                        index_of_winning_object_with_reflection)));
                Vect reflection_intersection_ray_direction = reflection_direction;

                Color reflection_intersection_color = getColorAt(reflection_intersection_position,
                                                                 reflection_intersection_ray_direction,
                                                                 scene_objects,
                                                                 index_of_winning_object_with_reflection,
                                                                 light_sources,
                                                                 accuracy, ambientlight);
                final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(
                                                    winning_object_color.getColorSpecial()));


            }
        }
    }

    //produces shadows
    for (unsigned light_index = 0; light_index < light_sources.size(); light_index++)
    {
        Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(
                                                     intersection_position.negative()).normalize();

        float cosine_angle = winning_object_normal.dotProduct(light_direction);

        if(cosine_angle > 0)
        {
            //test for shadows
            bool shadowed = false;

            Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(
                                                    intersection_position.negative());
            float distance_to_light_magnitude = distance_to_light.magnitude();

            Ray shadow_ray(intersection_position,
                           light_sources.at(light_index)->getLightPosition().vectAdd(
                                                  intersection_position.negative()).normalize());
            vector<double> secondary_intersections;

            for (unsigned object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++)
            {
                secondary_intersections.push_back(scene_objects.at(object_index)->
                                                  findIntersection(shadow_ray));
            }

            for(unsigned c = 0; c < secondary_intersections.size(); c++)
            {
                if (secondary_intersections.at(c) > accuracy)
                {
                    if (secondary_intersections.at(c) <= distance_to_light_magnitude)
                    {
                        shadowed = true;
                    }
                    break;
                }
            }

            if (shadowed == false)
            {
                final_color = final_color.colorAdd(winning_object_color.colorMult(
                                                   light_sources.at(light_index)->getColor()).
                                                   colorScalar(cosine_angle));
                if(winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1)
                {
                    double dot1 = winning_object_normal.dotProduct(
                                  intersecting_ray_direction.negative());
                    Vect scalar1 = winning_object_normal.vectMult(dot1);
                    Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
                    Vect scalar2 = add1.vectMult(2);
                    Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
                    Vect reflection_direction = add2.normalize();

                    double specular = reflection_direction.dotProduct(light_direction);
                    if (specular > 0)
                    {
                        specular = pow(specular, 10);
                        final_color = final_color.colorAdd(
                                      light_sources.at(light_index)->
                                      getColor().colorScalar(
                                      specular*winning_object_color.getColorSpecial()));
                    }
                }
            }
        }
    }

    return final_color.clip(); //used to make the color values not to pass the maximum possible value
}



#endif // _ESSENTIALS_H_
