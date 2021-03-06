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
#include "App.h"

#include "omp.h"

using namespace std;


int main()
{

    App *app;
    app = App::getInstance();
    app -> run();
    return 0;
}
