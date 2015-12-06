#ifndef _WINNINGOBJECTINDEX_H
#define _WINNINGOBJECTINDEX_H

#include<vector>

using namespace std;

int winningObjectIndex(vector<double> object_intersections)
{
    // return the index of the winning intersecition
    int index_of_minimum_value;

    //prevent unnessary calculations
    if (object_intersections.size() == 0)
    {
        //if there are no intersections
        return -1;
    }
    else if(object_intersections.size() == 1)
    {
        //
        if(object_intersections.at(0) > 0)
        {
            //if that intersection is greater than zero then its our
            //index of minimum value
            return 0;
        }
        else
        {
            //otherwise the only intersection value is negative
            return -1;
        }
    }
    else
    {
        //otherwise there is more than one intersection
        //first find the maximum value
        double max = 0;
        for (int i = 0; i < object_intersections.size(); i++)
        {
            if (max < object_intersections.at(i))
            {
                max = object_intersections.at(i);
            }
        }
        // the starting from the maxmimum value to find the minimum positive value
        if (max > 0)
        {
            //we only want positive intersections
            for (int index = 0; index < object_intersections.size(); index++)
            {
                if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max)
                {
                    max = object_intersections.at(index);
                    index_of_minimum_value = index;
                }
            }
            return index_of_minimum_value;
        }
        else
        {
            //all the intersections were negative
            return -1;
        }
    }

}



#endif // _WINNINGOBJECTINDEX_H
