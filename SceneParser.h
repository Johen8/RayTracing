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

void makeCube (Vect corner1, Vect corner2, Color color, vector<Object*>& scene_objects);

using namespace std;

void testPath(string path)
{
    string dummy = "dummy.txt";
    FILE *f1=fopen((path+dummy).c_str(),"wb");
    if (f1!=NULL)
    {
        fclose(f1);
        remove((path+dummy).c_str());
    }
    else
    {
        throw -2;
    }
}

void SceneParser(int &height, int &width, int &aadepth, double &ambientlight,
                 Vect &campos, Vect &look_at, vector<Object*> &scene_objects,
                 vector<Source*> &light_sources, string &path)
{
    ifstream scenefile;
    int spheres, planes, sources, cubes;
    double xi, yi, zi, ri, cri, cgi, cbi, csi, di;
    string input;
    scenefile.open("scenefile.txt");
    if(scenefile.is_open())
    {
        getline(scenefile,input);
        //cout << input << endl;
        getline(scenefile,input);
        getline(scenefile,input);

        getline(scenefile,input);
        getline(scenefile,input);
        path.assign(input);

        testPath(path);

        cout << endl << "Chosen path:" << path << endl;
        scenefile.ignore();

        getline(scenefile,input);
        //cout << input << endl;
        scenefile >> width;
        //cout << width << endl;
        scenefile.ignore();

        getline(scenefile,input);
        //cout << input << endl;
        scenefile >> height;
        //cout << height << endl;
        scenefile.ignore();

        getline(scenefile,input);
        scenefile >> aadepth;
        scenefile.ignore();

        cout<<endl<<"Image with resolution "<<width<<"x"<<height<<" and AA depth of "<<aadepth<<endl;

        getline(scenefile,input);
        scenefile >> ambientlight;
        scenefile.ignore();

        getline(scenefile,input);
        scenefile >> sources;
        scenefile.ignore();

        getline(scenefile,input);
        getline(scenefile,input);
        for (int i = 0; i < sources; i++)
        {
            scenefile >> xi >> yi >> zi;
            scenefile.ignore();
            scenefile >> cri >> cgi >> cbi;
            //cout << xi << zi << cri <<endl;
            Vect location (xi, yi, zi);
            //cout << location.getVectX() << endl;
            Color scolor (cri, cgi, cbi, 0);
            //cout << scolor.getColorBlue() << endl;
            Light* light_input = new Light(location, scolor);
            light_sources.push_back(dynamic_cast<Source*>(light_input));
        }
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
        scenefile >> planes;
        scenefile.ignore();

        getline(scenefile,input);
        getline(scenefile,input);
        for (int i = 0; i < planes; i++)
        {
            scenefile >> xi >> yi >> zi >> di;
            scenefile.ignore();
            scenefile >> cri >> cgi >> cbi >> csi;
            //cout << xi << di << csi <<endl;
            Vect location (xi, yi, zi);
            //cout << location.getVectX() << endl;
            Color scolor (cri, cgi, cbi, csi);
            //cout << scolor.getColorBlue() << endl;
            Plane* plane_input = new Plane(location, di, scolor);
            scene_objects.push_back(dynamic_cast<Object*>(plane_input));
        }
        scenefile.ignore();

        getline(scenefile,input);
        scenefile >> spheres;
        //cout << spheres << endl;
        scenefile.ignore();

        getline(scenefile,input);
        getline(scenefile,input);
        for (int i = 0; i < spheres; i++)
        {
            scenefile >> xi >> yi >> zi >> ri;
            scenefile.ignore();
            scenefile >> cri >> cgi >> cbi >> csi;
            //cout << zi << ri << endl;
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

        getline(scenefile,input);
        scenefile >> cubes;
        scenefile.ignore();

        getline(scenefile,input);
        getline(scenefile,input);
        for (int i = 0; i < cubes; i++)
        {
            scenefile >> xi >> yi >> zi;
            scenefile.ignore();
            //cout << xi << ri << csi <<endl;
            Vect location (xi, yi, zi);
            scenefile >> xi >> yi >> zi;
            scenefile.ignore();
            Vect location2 (xi, yi, zi);
            scenefile >> cri >> cgi >> cbi >> csi;
            //cout << location.getVectX() << endl;
            Color scolor (cri, cgi, cbi, csi);
            //cout << scolor.getColorBlue() << endl;
            makeCube(location, location2, scolor, scene_objects);
        }
        scenefile.ignore();
    }
    else
    {
        throw -1;
    }
}

#endif // _SCENEPARSER_H_
