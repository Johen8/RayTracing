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
#include "Scene.h"
#include "Essentials.h"



using namespace std;

class SceneCreator {

public:
    SceneCreator();
    /* Function that takes two corners of a cube and creates adequately
    * placed triangles to form the cube. Also adds them to the scene_objects vector
    */

private:
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

//test if given path is valid by creating a dummy file
private:
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

public :
    Scene* SceneParser( int &width, int &height, string &path, string &scene_file )
    {
        vector<Object*> scene_objects; //vector that contains objects, notice that it is a vector of Object
        vector<Source*> light_sources;
        int aadepth;
        double ambientlight;
        Vect campos, look_at; //Vectors indicating camera position and where it should look at

        ifstream scenefile;
        int spheres, planes, sources, cubes;
        double xi, yi, zi, ri, cri, cgi, cbi, csi, di;
        string input;
        scenefile.open(scene_file.c_str());
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
            if (sources>0)
            {
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
            }
            else
            {
                getline(scenefile,input);
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
            if(planes > 0)
            {
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
            }
            else
            {
                getline(scenefile,input);
            }

            getline(scenefile,input); // N Spheres
            scenefile >> spheres;  // get
            cout<<endl<<"N of Spheres: "<<spheres<<endl;
            getline(scenefile,input);

            getline(scenefile,input); // Spheres
            getline(scenefile,input); // Format Spheres
            if (spheres>0)
            {
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
            }
            else
            {
                getline(scenefile,input);
            }


            getline(scenefile,input); // N Cubes
            scenefile >> cubes;		// get
            cout<<endl<<"N of Cubes: "<< cubes <<endl;
            getline(scenefile,input); // Remove line

            getline(scenefile,input); // Cubes
            getline(scenefile,input); // Format Cubes
            if(cubes > 0)
            {
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
                getline(scenefile,input);
            }

        }
        else
        {
            throw -1;
        }
        Scene* sne = new Scene( height, width, aadepth, ambientlight, campos, look_at, scene_objects, light_sources);
        return sne;
    }

};
SceneCreator::SceneCreator() {
}


#endif // _SCENEPARSER_H_
