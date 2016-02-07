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
    string scene_file = "scenefile.txt";
    string path;
    int width, height;

    string filename2;
    int dpi;
    SceneCreator* sCreator;
    Scene* scene;

    RGBType *pixels;
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

            unsigned char color[3]= {(unsigned char)(blue),(unsigned char)(green),(unsigned char)(red)};

            fwrite(color,1,3,f);
        }
    }
    else
    {
        cout << "It is not possible to open the output file!" <<endl;
    }


    fclose(f);
}

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
        sCreator = new SceneCreator( );
        bool leave_loop = false;

        //Vect look_at (0,0,0);
        while(!leave_loop)
        {
            try //try to parse the Scene file. Two types of common errors are catched
            {
                scene = sCreator->SceneParser(width, height, path, scene_file);
                leave_loop = true;
            }
            catch (int err)
            {
                if (err == -1)
                {
                    cout << "It was not possible to open the \"" << scene_file << "\" file, check if it is in the folder."<< endl << endl;
                    cout << "If you want to use another scene file that is in the Ray Tracer folder, type its name below, or 'exit' to abort:" <<endl;
                    cin >> scene_file;
                    getchar();
                    if (scene_file == "exit")
                        exit(-1);
                }
                else if (err == -2)
                {
                    cout << "Invalid output path chosen! Please verify Scene File!" <<endl;
                    getchar();
                    exit( -2 );
                }
            }
        }
        t1 = clock();

        cout <<endl<< "rendering..." << endl;

        pixels = scene->getPixels();

        dpi=72;
        now = time(NULL);
        strftime(file_name,25,"%Y-%m-%d %H.%M.%S.bmp",localtime(&now));

        filename2 = file_name;
        filename2 = path+filename2;
        //cout << filename2 << endl;

		  App::getInstance()->savebmp(filename2.c_str(), width, height, dpi, pixels);


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
