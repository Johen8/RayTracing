#ifndef _SCENEPARSER_H_
#define _SCENEPARSER_H_

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

using namespace std;

void SceneParser(int &height, int &width, int &aadepth, double &ambientlight,
                 Vect &campos, Vect &look_at, vector<Object*>& scene_objects)
{
    ifstream scenefile;
    int spheres;
    double xi, yi, zi, ri, cri, cgi, cbi, csi;
    string input;
    scenefile.open("scenefile.txt");
    if(scenefile.is_open())
    {
        while (getline(scenefile,input))
        {
            //cout << input << endl;
            getline(scenefile,input);
            //cout << input << endl;
            scenefile >> width;
            cout << width << endl;
            scenefile.ignore();
            getline(scenefile,input);
            //cout << input << endl;
            scenefile >> height;
            cout << height << endl;
            scenefile.ignore();
            getline(scenefile,input);
            scenefile >> aadepth;
            scenefile.ignore();
            getline(scenefile,input);
            scenefile >> ambientlight;
            scenefile.ignore();
            getline(scenefile,input);
            scenefile >> xi >> yi >> zi;
            campos.setVect(xi, yi, zi);
            scenefile.ignore();
            getline(scenefile,input);
            scenefile >> xi >> yi >> zi;
            look_at.setVect(xi, yi, zi);
            scenefile.ignore();
            getline(scenefile,input);
            scenefile >> spheres;
            //cout << spheres << endl;
            scenefile.ignore();
            getline(scenefile,input);
            for (int i = 0; i < spheres; i++)
            {
                scenefile >> xi >> yi >> zi >> ri >> cri >> cgi >> cbi >> csi;
                //cout << xi << ri << csi <<endl;
                Vect location (xi, yi, zi);
                //cout << location.getVectX() << endl;
                Color scolor (cri, cgi, cbi, csi);
                //cout << scolor.getColorBlue() << endl;
                Sphere* sphere_input = new Sphere(location, ri, scolor); //had to do this because
                                                //objects are destructed every iteration, and the vector
                                                //doesn't contain them anymore, this way it's just the
                                                //pointer that is destructed, but the memory allocated
                                                //by 'new' is still there (or something like that)
                scene_objects.push_back(dynamic_cast<Object*>(sphere_input));
            }
            scenefile.ignore();

        }
    }
    else
    {
        cout << "não foi possivel abrir o arquivo"<< endl;
    }
}

#endif // _SCENEPARSER_H_
