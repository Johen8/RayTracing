#ifndef _COLOR_H
#define _COLOR_H

class Color
{
    double red, green, blue, special; //special value can be used for many purposes
                                      //currently it can be used between 0 and 1 to make
                                      //a reflective surface and as 2 to make a checkered surface
                                      //other properties such as transparency can be added
                                      //through this variable and the proper addition of
                                      //commands in the getColorAt function

    public:
    Color();

    Color(double, double, double, double);

    //method functions
    double getColorRed()
    {
        return red;
    }

    double getColorGreen()
    {
        return green;
    }

    double getColorBlue()
    {
        return blue;
    }

    double getColorSpecial()
    {
        return special;
    }

    void setColorRed(double redValue) { red = redValue; }
    void setColorGreen(double greenValue) { green = greenValue; }
    void setColorBlue(double blueValue) { blue = blueValue; }
    void setColorSpecial(double specialValue) { special = specialValue; }

    double brightness()
    {
        return (red + green + blue)/3;
    }

    Color colorScalar(double scalar)
    {
        return Color (red*scalar, green*scalar, blue*scalar, special);
    }

    Color colorAdd(Color color)
    {
        return Color(red + color.getColorRed(),
                     green + color.getColorGreen(),
                     blue + color.getColorBlue(),
                     special);
    }

    Color colorMult(Color color)
    {
        return Color (red*color.getColorRed(),
                      green*color.getColorGreen(),
                      blue*color.getColorBlue(),
                      special);
    }

    Color colorAverage(Color color)
    {
        return Color ((red+color.getColorRed())/2,
                      (green+color.getColorGreen())/2,
                      (blue+color.getColorBlue())/2,
                      special);
    }

    Color clip()
    {
        double alllight = red + green + blue;
        double excesslight = alllight - 3;
        if (excesslight > 0)
        {
            red = red + excesslight*(red/alllight);
            green = green + excesslight*(green/alllight);
            blue = blue + excesslight*(blue/alllight);
        }
        if (red > 1)
        {
            red = 1;
        }
        if (blue > 1)
        {
            blue = 1;
        }
        if (green > 1)
        {
            green = 1;
        }
        if (red < 0)
        {
            red = 0;
        }
        if (blue < 0)
        {
            blue = 0;
        }
        if (green < 0)
        {
            green = 0;
        }
        return Color(red, green, blue, special);
    }

};

Color::Color()
{
    red = 0.5;
    green = 0.5;
    blue = 0.5;
    special = 0;
}

Color::Color(double r, double g, double b, double s)
{
    red = r;
    green = g;
    blue = b;
    special = s;
}

#endif

