#ifndef _APP_H_
#define _APP_H_

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<limits>

#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Source.h"

#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"

#include "Essentials.h"
#include "SceneParser.h"

#include "omp.h"

using namespace std;

class App
{
    public:
    App();

void run()
{
    clock_t t1, t2; //clock variables used to measure running time and
    time_t now;     //for image filename purposes
    t1 = clock();

    vector<Object*> scene_objects; //vector that contains objects, notice that it is a vector of Object
    vector<Source*> light_sources;
	 SceneCreator* sCreator = new SceneCreator();

    char file_name[25]; //variables for collecting the path and for creating the filename
    string path;
    string filename2;

    int dpi=72;
    int width, height, aadepth;
    double ambientlight;
    Vect campos, look_at; //Vectors indicating camera position and where it should look at
    //Vect look_at (0,0,0);
    try //try to parse the Scene file. Two types of common errors are catched
    {
        sCreator->SceneParser(height, width, aadepth, ambientlight, campos, look_at, scene_objects, light_sources, path);
    }
    catch (int err)
    {
        if (err == -1)
        {
            cout << "It was not possible to open the \"scenefile.txt\" file, check if it is in the folder"<< endl;
    			getchar();
            exit( -1 );
        }
        else if (err == -2)
        {
            cout << "Invalid path chosen! Please verify Scene File!" <<endl;
    			getchar();
            exit( -2 );
        }
    }
    cout <<endl<< "rendering..." << endl;

    int n = width*height; //number of pixels in the image
    RGBType *pixels = new RGBType[n];

   // double aathreshold = 0.1;
    double aspectratio = (double)width/(double)height;
    double accuracy = 0.000001;

    Vect O (0,0,0);
    Vect X (1,0,0);
    Vect Y (0,1,0);
    Vect Z (0,0,1);

    //Vect location2 (1.75, 0, 0);


    Vect diff_btw (campos.getVectX() - look_at.getVectX(),
                   campos.getVectY() - look_at.getVectY(),
                   campos.getVectZ() - look_at.getVectZ()); //vector representing the difference
                                                            //between camera position and where it
                                                            //should look at

    Vect camdir = diff_btw.negative().normalize();
    Vect camright = Y.crossProduct(camdir).normalize();
    Vect camdown = camright.crossProduct(camdir);

    Camera scene_cam (campos, camdir, camright, camdown);
 /* OLD CODE -- NOT BEING USED -- KEEPING IT FOR THE SAKE OF NECESSITY --
    FROM BEFORE SCENE PARSER WAS CREATED

    Color white_light (1.0, 1.0, 1.0, 0);
    Color pretty_green(0.5, 1.0, 0.5, 0.3);
    Color maroon(0.5, 0.25, 0.25, 0);
    Color checkered(0.5, 0.25, 0.25, 2);
    Color gray (0.5, 0.5, 0.5, 0);
    Color black (0.0, 0.0, 0.0, 0.0);
    Color orange (0.94, 0.75, 0.31, 0.1);

    //Vect light_position (-7, 10, -10);
    //Light scene_light (light_position, white_light);
    //light_sources.push_back(dynamic_cast<Source*>(&scene_light));

    //scene objects
    //Sphere scene_sphere (O, 1, pretty_green);
    //Plane scene_plane(Y, -1, checkered); //Y É A NORMAL????
    //Sphere scene_sphere2 (location2, 0.5, maroon);
    //Triangle scene_triangle(Vect (3, 0, 0), Vect (0, 3, 0), Vect (0, 0, 3), orange);


    //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    //scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
    //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
    //scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle));

    //makeCube(Vect (1,1,1), Vect(-1,-1,-1), orange, scene_objects);
*/
    double xamnt, yamnt;
    //double tempRed, tempGreen, tempBlue;
    int x,y;

    #pragma omp parallel for private(y, xamnt, yamnt) //Parallelization utilizing OpenMP
                            //y, xamnt, yamnt are private variables, it means each thread
                            //has its own version of the variable
    for (x = 0; x < width; x++) //going through all the pixels one by one
    {
        for(y = 0; y < height; y++)
        {
            //cout<< y << endl;

            int thisone=0;
            #pragma omp critical //only one thread executes this command at a time
                                //there were some wrong pixels without it
            {
                thisone = (y*width + x);
            }
            //start with a blank pixel
            double tempRed[aadepth*aadepth];
            double tempGreen[aadepth*aadepth];
            double tempBlue[aadepth*aadepth];

            for(int aax = 0; aax < aadepth; aax++)
            {
                for(int aay = 0; aay < aadepth; aay++)
                {
                    int aa_index = aay*aadepth + aax;

                    if(aadepth == 1)
                    { //no AA
                        //start with no anti-aliasing
                        if(width>height) //this is used to make the rays coming from the camera to cover the plane of the image
                        {
                            //the image is wider than it is tall
                            xamnt = ((x+0.5)/width)*aspectratio -
                            (((width-height)/(double)height)/2);
                            yamnt = ((height - y) + 0.5)/height;
                        }
                        else if (height > width)
                        {
                            //the image is taller than it is wide
                            xamnt = (x + 0.5)/width;
                            yamnt = (((height - y) + 0.5)/height)/aspectratio -
                            (((height - width)/(double)width)/2);
                        }
                        else
                        {
                            // the image is square
                            xamnt = (x + 0.5)/width;
                            yamnt = ((height - y) + 0.5)/height;
                        }
                    }
                    else
                    {
                        // anti-aliasing by supersampling
                        if(width>height)
                        {
                            //the image is wider than it is tall
                            xamnt = ((x+(double)aax/((double)aadepth - 1))/width)*aspectratio -
                            (((width-height)/(double)height)/2);
                            yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
                        }
                        else if (height > width)
                        {
                            //the image is taller than it is wide
                            xamnt = (x + (double)aax/((double)aadepth - 1))/width;
                            yamnt = (((height - y) + (double)aax/((double)aadepth - 1))/height)/aspectratio -
                            (((height - width)/(double)width)/2);
                        }
                        else
                        {
                            // the image is square
                            xamnt = (x + (double)aax/((double)aadepth - 1))/width;
                            yamnt = ((height - y) + (double)aax/((double)aadepth - 1))/height;
                        }
                    }
                    // create the ray from the camera to this pixel

                    Vect cam_ray_origin = scene_cam.getCameraPosition();
                    Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(
                                                            camdown.vectMult(yamnt - 0.5))).normalize();

                    Ray cam_ray(cam_ray_origin,cam_ray_direction);

                    vector<double> intersections; //vector used to find all the intersections of each ray

                    for (unsigned index = 0; index < scene_objects.size(); index++)
                    {
                        intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
                    }

                    int index_of_winning_object = winningObjectIndex(intersections);

                    if(index_of_winning_object == -1) //no intersections found
                    {
                        //set the background black
                        tempRed[aa_index]=0./255;
                        tempGreen[aa_index]=0./255;
                        tempBlue[aa_index]=0./255;
                    }
                    else
                    {
                        //index corresponds to an object in our scene
                        if(intersections.at(index_of_winning_object) > accuracy)
                        {
                            //determine the position and direction vectors at the
                            //point of intersection
                            Vect intersection_position = cam_ray_origin.vectAdd(
                                                         cam_ray_direction.vectMult(
                                                         intersections.at(index_of_winning_object)));
                            Vect intersecting_ray_direction = cam_ray_direction;

                            Color intersection_color = getColorAt(intersection_position,
                                                                  intersecting_ray_direction,
                                                                  scene_objects,
                                                                  index_of_winning_object,
                                                                  light_sources,
                                                                  accuracy,
                                                                  ambientlight);

                            //not only the object color is important, the color on the
                            //specific coordinate is important because of shadows, for example
                            tempRed[aa_index]=intersection_color.getColorRed();
                            tempGreen[aa_index]=intersection_color.getColorGreen();
                            tempBlue[aa_index]=intersection_color.getColorBlue();
                        }
                    }
                }
            }
            //average the pixel color
            double totalRed = 0;
            double totalBlue = 0;
            double totalGreen = 0;

            for (int iRed = 0; iRed < aadepth*aadepth; iRed++)
            {
                totalRed = totalRed + tempRed[iRed];
            }
            for (int iGreen = 0; iGreen < aadepth*aadepth; iGreen++)
            {
                totalGreen = totalGreen + tempGreen[iGreen];
            }
            for (int iBlue = 0; iBlue < aadepth*aadepth; iBlue++)
            {
                totalBlue = totalBlue + tempBlue[iBlue];
            }

            double avgRed = totalRed/(aadepth*aadepth);
            double avgGreen = totalGreen/(aadepth*aadepth);
            double avgBlue = totalBlue/(aadepth*aadepth);

            #pragma omp critical
            {
                pixels[thisone].r=avgRed;
                pixels[thisone].g=avgGreen;
                pixels[thisone].b=avgBlue;
            }
        }
    } //END OF PARALLELIZED REGION

    now = time(NULL);
    strftime(file_name,25,"%Y-%m-%d %H.%M.%S.bmp",localtime(&now));

    filename2 = file_name;
    filename2 = path+filename2;
    //cout << filename2 << endl;

    savebmp(filename2.c_str(), width, height, dpi, pixels);

    delete pixels;//, tempRed, tempGreen, tempBlue;

    t2 = clock();
    float diff = ((float)t2 - (float)t1)/1000;

    cout <<endl<< "It took me "<<diff << " seconds to render!" << endl;
    cout << "Output: " << filename2.c_str() << endl;
    cout <<endl<<"Press any key to exit!" << endl;
    getchar();

}



};

App::App()
{

}


#endif // _APP_H_
