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

using namespace std;

void makeCube (Vect corner1, Vect corner2, Color color, vector<Object*>& scene_objects)
{
        //corner1
        double c1x = corner1.getVectX();
        double c1y = corner1.getVectY();
        double c1z = corner1.getVectZ();

        //corner2
        double c2x = corner2.getVectX();
        double c2y = corner2.getVectY();
        double c2z = corner2.getVectZ();

        Vect A(c2x, c1y, c1z);
        Vect B(c2x, c1y, c2z);
        Vect C(c1x, c1y, c2z);

        Vect D(c2x, c2y, c1z);
        Vect E(c1x, c2y, c1z);
        Vect F(c1x, c2y, c2z);

        //left side
        scene_objects.push_back(new Triangle (D, A, corner1, color));
        scene_objects.push_back(new Triangle (corner1, E, D, color));
        //far side
        scene_objects.push_back(new Triangle (corner2, B, A, color));
        scene_objects.push_back(new Triangle (A, D, corner2, color));
        //right side
        scene_objects.push_back(new Triangle (F, C, B, color));
        scene_objects.push_back(new Triangle (B, corner2, F, color));
        //front side
        scene_objects.push_back(new Triangle (E, corner1, C, color));
        scene_objects.push_back(new Triangle (C, F, E, color));
        //top
        scene_objects.push_back(new Triangle (D, E, F, color));
        scene_objects.push_back(new Triangle (F, corner2, D, color));
        //bottom
        scene_objects.push_back(new Triangle (corner1, A, B, color));
        scene_objects.push_back(new Triangle (B, C, corner1, color));
}


int main()
{
    cout << "rendering..." << endl;

    clock_t t1, t2;
    time_t now;
    t1 = clock();

    vector<Object*> scene_objects;

    char arquivo[25];
    string path = "T:\\GitHub\\RayTracing\\Resultados\\";
    string filename2;

    int dpi=72;
    int width, height, aadepth;
    double ambientlight;
    Vect campos, look_at;// (3, 1.5, -4);
    //Vect look_at (0,0,0);
    SceneParser(height, width, aadepth, ambientlight, campos, look_at, scene_objects);
    int n = width*height;
    int thisone;
    RGBType *pixels = new RGBType[n];

   // double aathreshold = 0.1;
    double aspectratio = (double)width/(double)height;
    double accuracy = 0.000001;

    Vect O (0,0,0);
    Vect X (1,0,0);
    Vect Y (0,1,0);
    Vect Z (0,0,1);

    Vect location2 (1.75, 0, 0);


    Vect diff_btw (campos.getVectX() - look_at.getVectX(),
                   campos.getVectY() - look_at.getVectY(),
                   campos.getVectZ() - look_at.getVectZ());

    Vect camdir = diff_btw.negative().normalize();
    Vect camright = Y.crossProduct(camdir).normalize();
    Vect camdown = camright.crossProduct(camdir);

    Camera scene_cam (campos, camdir, camright, camdown);

    Color white_light (1.0, 1.0, 1.0, 0);
    Color pretty_green(0.5, 1.0, 0.5, 0.3);
    Color maroon(0.5, 0.25, 0.25, 0);
    Color checkered(0.5, 0.25, 0.25, 2);
    Color gray (0.5, 0.5, 0.5, 0);
    Color black (0.0, 0.0, 0.0, 0.0);
    Color orange (0.94, 0.75, 0.31, 0.1);

    Vect light_position (-7, 10, -10);
    Light scene_light (light_position, white_light);
    vector<Source*> light_sources;
    light_sources.push_back(dynamic_cast<Source*>(&scene_light));

    //scene objects
    //Sphere scene_sphere (O, 1, pretty_green);
    Plane scene_plane(Y, -1, checkered); //Y É A NORMAL????
    //Sphere scene_sphere2 (location2, 0.5, maroon);
    //Triangle scene_triangle(Vect (3, 0, 0), Vect (0, 3, 0), Vect (0, 0, 3), orange);


    //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
    //scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
    //scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle));

    //makeCube(Vect (1,1,1), Vect(-1,-1,-1), orange, scene_objects);

    double xamnt, yamnt;
    //double tempRed, tempGreen, tempBlue;

    for (int x = 0; x < width; x++) //percorre os pixels um a um
    {
        for(int y = 0; y < height; y++)
        {
            thisone = y*width + x;

            //start with a blank pixel
            double tempRed[aadepth*aadepth];
            double tempGreen[aadepth*aadepth];
            double tempBlue[aadepth*aadepth];

            for(int aax = 0; aax < aadepth; aax++)
            {
                for(int aay = 0; aay < aadepth; aay++)
                {
                    int aa_index = aay*aadepth + aax;

                    srand(time(0));

                    // create the ray from the camera to this pixel
                    if(aadepth == 1)
                    { //no AA
                        //start with no anti-aliasing
                        if(width>height) //serve para os raios que saem da camera cobrirem o plano da imagem
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
                        // anti-aliasing
                        if(width>height) //serve para os raios que saem da camera cobrirem o plano da imagem
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

                    Vect cam_ray_origin = scene_cam.getCameraPosition();
                    Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(
                                                            camdown.vectMult(yamnt - 0.5))).normalize();

                    Ray cam_ray(cam_ray_origin,cam_ray_direction);

                    vector<double> intersections;

                    for (unsigned index = 0; index < scene_objects.size(); index++)
                    {
                        intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
                    }

                    int index_of_winning_object = winningObjectIndex(intersections);

                    if(index_of_winning_object == -1)
                    {
                        //set the background black
                        tempRed[aa_index]=0./255; //vetor pixels é a matrix feita numa linha (daí que vem o indice thisone)
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

                            //nao basta a cor do objeto, precisamos da cor na coordenada certa!! por causa
                            //de sombras e tal,
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

            pixels[thisone].r=avgRed;
            pixels[thisone].g=avgGreen;
            pixels[thisone].b=avgBlue;
        }
    }

    now = time(NULL);
    strftime(arquivo,25,"%Y-%m-%d %H.%M.%S.bmp",localtime(&now));

    filename2 = arquivo;
    filename2 = path+filename2;
    //cout << filename2 << endl;

    savebmp(filename2.c_str(), width, height, dpi, pixels);

    delete pixels;//, tempRed, tempGreen, tempBlue;

    t2 = clock();
    float diff = ((float)t2 - (float)t1)/1000;

    cout << diff << " seconds" << endl;
    cout << "Output: " << filename2.c_str() << endl;
    return 0;
}
