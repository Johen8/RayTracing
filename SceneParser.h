#ifndef _SCENEPARSER_H_
#define _SCENEPARSER_H_

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>

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

//this function is needed here because a cube is made of triangle components
void makeCube (Vect corner1, Vect corner2, Color color, vector<Object*>& scene_objects);

using namespace std;
//test if given path is valid by creating a dummy file
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
    if(scenefile.is_open()) //if cannot open scenefile, throw proper error
    {
        getline(scenefile,input); //1
        getline(scenefile,input); //2
        getline(scenefile,input); //3
        getline(scenefile,input); //4
        getline(scenefile,input); //5
        path.assign(input);

        testPath(path);

        cout << endl << "Chosen path:" << path << endl;

        getline(scenefile,input); //6
        scenefile >> width;       //7
        getline(scenefile,input); //7
        getline(scenefile,input); //8
        scenefile >> height;		 //9
        getline(scenefile,input); //9
        getline(scenefile,input); //10
        scenefile >> aadepth;     //11
        getline(scenefile,input); //11
        cout<<endl<<"Image with resolution "<<width<<"x"<<height<<" and AA depth of "<<aadepth<<endl;

        getline(scenefile,input); //12
        scenefile >> ambientlight; //13
        getline(scenefile,input); //13

        getline(scenefile,input); //14
        scenefile >> sources;     //15
        getline(scenefile,input); //15

        getline(scenefile,input); //16
        getline(scenefile,input); //17
        cout<<endl<<"Ambient Ligth: "<<ambientlight<<", N Sources: "<< sources <<endl;
        for (int i = 0; i < sources; i++)
        {
					 // READ FORMAT 18
            scenefile >> xi >> yi >> zi;
            cout <<"X: "<< xi<< " Y: "<< yi << " Z: "<< zi <<endl;
            scenefile.ignore();
            scenefile >> cri >> cgi >> cbi;
        		getline(scenefile,input);     // FORMAT 18
            cout <<"R: "<< cri<< " G: "<< cgi << " B: "<< cbi <<endl;
            Vect location (xi, yi, zi);
            //cout << location.getVectX() << endl;
            Color scolor (cri, cgi, cbi, 0);
            //cout << scolor.getColorBlue() << endl;
            Light* light_input = new Light(location, scolor);
            light_sources.push_back(dynamic_cast<Source*>(light_input));
        }

        getline(scenefile,input); // CAMERA POSITION
        scenefile >> xi >> yi >> zi;  // get
        campos.setVect(xi, yi, zi);
        cout << endl << "CAM POS  "<<"X: "<< xi<< " Y: "<< yi << " Z: "<< zi <<endl;
        getline(scenefile,input);

        getline(scenefile,input);   // Where to look
        scenefile >> xi >> yi >> zi; //get
        cout << "Look At  "<<"X: "<< xi<< " Y: "<< yi << " Z: "<< zi <<endl;
        look_at.setVect(xi, yi, zi);
        getline(scenefile,input);

        getline(scenefile,input); // N PLANES
        scenefile >> planes;      // get
        cout<<endl<<"N of Planes: "<<planes<<endl;
        getline(scenefile,input);

        getline(scenefile,input); //planes
        getline(scenefile,input); // Format planes
        for (int i = 0; i < planes; i++) //with the number of planes, adds them to the Object vector
        {
            scenefile >> xi >> yi >> zi >> di;
            cout <<"X: "<< xi<< " Y: "<< yi << " Z: "<< zi << " d: " << di <<endl;
            scenefile.ignore();
            scenefile >> cri >> cgi >> cbi >> csi;
            cout <<"R: "<< cri<< " G: "<< cgi << " B: "<< cbi << " S: " << csi <<endl;
            Vect location (xi, yi, zi);
            Color scolor (cri, cgi, cbi, csi);
            Plane* plane_input = new Plane(location, di, scolor);
            scene_objects.push_back(dynamic_cast<Object*>(plane_input)); //dynamic cast used to add a Plane object to a Object vector
        		getline(scenefile,input);
        }

        getline(scenefile,input); // N Spheres
        scenefile >> spheres;  // get
        cout<<endl<<"N of Spheres: "<<spheres<<endl;
        getline(scenefile,input);

        getline(scenefile,input); // Spheres
        getline(scenefile,input); // Format Spheres
        for (int i = 0; i < spheres; i++)
        {
            scenefile >> xi >> yi >> zi >> ri;
            cout <<"X: "<< xi<< " Y: "<< yi << " Z: "<< zi << " r: " << ri <<endl;
            scenefile.ignore();
            scenefile >> cri >> cgi >> cbi >> csi;
            cout <<"R: "<< cri<< " G: "<< cgi << " B: "<< cbi << " S: " << csi <<endl;
            Vect location (xi, yi, zi);
            //cout << location.getVectX() << endl;
            Color scolor (cri, cgi, cbi, csi);
            //cout << scolor.getColorBlue() << endl;
            Sphere* sphere_input = new Sphere(location, ri, scolor); //had to do this because
                                            //objects are destructed every iteration, and the vector
                                            //doesn't contain them anymore, this way it's just the
                                            //pointer that is destructed, but the memory allocated
                                            //by 'new' is still there
            scene_objects.push_back(dynamic_cast<Object*>(sphere_input));
        		getline(scenefile,input);
        }

        getline(scenefile,input); // N Cubes
        scenefile >> cubes;		// get
        cout<<endl<<"N of Cubes: "<< cubes <<endl;
        getline(scenefile,input); // Remove line

        getline(scenefile,input); // Cubes
        getline(scenefile,input); // Format Cubes
        for (int i = 0; i < cubes; i++)
        {
            scenefile >> xi >> yi >> zi;
            cout <<"X: "<< xi<< " Y: "<< yi << " Z: "<< zi <<endl;
            scenefile.ignore();
            Vect location (xi, yi, zi);
            scenefile >> xi >> yi >> zi;
            cout <<"X: "<< xi<< " Y: "<< yi << " Z: "<< zi <<endl;
            scenefile.ignore();
            Vect location2 (xi, yi, zi);
            scenefile >> cri >> cgi >> cbi >> csi;
            cout <<"R: "<< cri<< " G: "<< cgi << " B: "<< cbi << " S: " << csi <<endl;
            Color scolor (cri, cgi, cbi, csi);
            makeCube(location, location2, scolor, scene_objects);
        		getline(scenefile,input);
        }
    }
    else
    {
        throw -1;
    }
}

#endif // _SCENEPARSER_H_
