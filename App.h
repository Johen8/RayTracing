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
#include "Scene.h"

#include "omp.h"

using namespace std;

class App
{
private:
    App();

    static App* instance;

    clock_t t1, t2; //clock variables used to measure running time and
    time_t now;     //for image filename purposes

    char file_name[25]; //variables for collecting the path and for creating the filename
    string path;
    int width, height;

    string filename2;
    int dpi;
    SceneCreator* sCreator;
    Scene* scene;

    RGBType *pixels;

public:
    static App* getInstance() {
        if( NULL == instance ) {
            instance = new App();
        }
        return instance;
    }
public:
    void run()
    {
        t1 = clock();


        sCreator = new SceneCreator( );

        //Vect look_at (0,0,0);
        try //try to parse the Scene file. Two types of common errors are catched
        {
            scene = sCreator->SceneParser(height, width, path);
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

        pixels = scene->getPixels();

        dpi=72;
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
App *App::instance = NULL;

#endif // _APP_H_
