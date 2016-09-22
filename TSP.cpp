#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <dos.h>
#include <unistd.h>
#include "koolplot.h"
using namespace std;

struct node
{
    int xCoordinate;
    int yCoordinate;
};

void readFile ();//func to read file
node *createNode (int x, int y);//func to create nodes in the vector
void swapPoints();//func to swap points
void swapBack();//func to swap points back to orignal

void readPoints();//function to read points in vec
double totalDistSq();//func to calc total dis squared
double distSqBtwPoints (int p);//func to calc distance btwn points
void anneal();//function to anneal the points, to minimize distance
void koolPlot();//func to display graph of annealed points
int randomSwap1, randomSwap2;//global variables of the vector points that are swapped

vector <node> points;//vector of nodes to store all points
int numPoints;//number of points in the vector

int main()
{
    srand (time(NULL));//set the randomization
    readFile();//read the file containing points
    anneal();//anneal the points to minimize distance
    koolPlot();//output the points on a graph
    return 0;
}

void koolPlot()//plot the
{
    //initialize arrays
    double xVal[numPoints], yVal[numPoints];
    //double largestx=points[0].xCoordinate, smallestx=points[0].xCoordinate, largesty=points[0].yCoordinate, smallesty=points[0].yCoordinate;
    for (int i=0; i<numPoints; i++)
    {
        xVal[i] = points[i].xCoordinate;
        yVal[i] = points[i].yCoordinate;
    }
    plotdata x(xVal, numPoints);
    plotdata y(yVal, numPoints);
    for (int i=0; i<numPoints; i++)
    {
        addMark(x, y, points[i].xCoordinate, points[i].yCoordinate);
    }
    plot(x, y, LIGHTGRAY, "Graphic Representation");

}

void anneal ()//anneal func to put vector of points in order to make distance the smallest
{
    double temperature = 10000.0;//set the temperature of the anneal
    double deltaDistance = 0;//the change in distance btwn the new arrangement of points in vector and the oold arrangement
    double coolingRate = 0.99999;//set the cooling rate - so it accepts values that incr the distance of the
    double stopTemp = 0.000000000000000000001;//set the stop temperature - it determines the number of iterations the program goes through
    double distancePrev = totalDistSq();//set distance prev as the totalDistSq
    cout << endl;
    while (temperature > stopTemp)//
    {
        swapPoints();
        deltaDistance = totalDistSq() - distancePrev;
        //if the new order has a smaller distance
        //or if the new order has a larger distance but satisfies Boltzman condition then accept the arrangement
        double randNum = rand () % (1);//create a random number
        if ((deltaDistance <= 0) || (exp(-deltaDistance / temperature) > randNum))//determine whether to keep the swapped vector or not
        {
            distancePrev = distancePrev + deltaDistance;
        }
        else//swap the vector back
        {
            swapBack();
        }
        //cool down the temperature
        temperature *= coolingRate;
    }
}

node *createNode (int x, int y)//function to create a new node and fill it with given data
{
    node *newNode;//declare a new node
    newNode = new node;;//create a new node
    newNode -> xCoordinate = x;//place data in the new node
    newNode -> yCoordinate = y;//place data in the new node
    return newNode;
}

void readFile ()//give instructions and read the file inputted by the user
{
    //give instructions
    cout << "INSTRUCTIONS" << endl;
    cout<<endl;
    cout << "Create a text file, with the first line" << endl;
    cout << "contain the number of points that need to be travelled"<< endl;
    cout << "then, on the next line, put the x (first) and" << endl;
    cout << "y (second) coordinates of the first point" << endl;
    cout << "separated by a space, do that for every line such" << endl;
    cout << "that each line contains one points." <<endl;
    cout << endl;
    cout << endl;
    ifstream infile;
    string infilename;//variable to get filename
    cout << "Enter the input file name" << endl;
    getline (cin,infilename);//get filename
    cout << endl;
    do//open the file
    {
        infile.open(infilename.c_str(), ifstream::in);
        if (!infile.is_open())
        {
            cout << "coud not open " << infilename << endl;
            cout << "Enter another filename" << endl;
        }
        else
            break;
        getline (cin, infilename);
    }
    while (true);
    int x=0, y=0;//create the variables to store the values of the coordinates
    while (infile.good())
    {
        infile >> numPoints;//determine the number of points in the problem
        if (numPoints <= 0)
        {
            cout << "Invalid number of coordinates value (n) enter value greater than 0" << endl;
            return;
        }
        // when n is given the loop will run to collect that number of coordinates
        for(int i = 0; i < numPoints; i++)
        {
            //counter++;
            infile >> x >> y;
            points.push_back(*createNode(x,y));// counters to check if all coodinates are greater than 0 and less than 21

        }
    }
    numPoints++;//add the first point again so that a loop can be created
    points.push_back(*createNode(points [0].xCoordinate, points [0].yCoordinate));//push back the first point as the last point
}

void readPoints ()//read points in the vector
{
    cout << "The coordinates inputted are:" << endl;
    for (int i = 0; i < numPoints; i++)
    {
        cout << "(" << points[i].xCoordinate << ", " << points[i].yCoordinate << ")" << endl;
    }
}

void swapPoints()//func to swap points
{
    while (true)
    {
        randomSwap1 = rand () % (numPoints - 2) + 1;//come up with a rand integer to swap the vector
        randomSwap2 = rand () % (numPoints - 2) + 1;//come up with a rand integer to swap the vector
        if (randomSwap1 != randomSwap2)//ensure you are not swapping the same point
            break;
    }
    node temp = points[randomSwap1];//temp node to store the value to swap
    //swap values
    points [randomSwap1] = points [randomSwap2];
    points [randomSwap2] = temp;
}

void swapBack()//func to swap points back to the way they were
{
    node temp = points[randomSwap1];//temp node to store the value to swap
    //swap values
    points [randomSwap1] = points [randomSwap2];
    points [randomSwap2] = temp;
}

double totalDistSq()//func to determine the totalDisSq
{
    double distance;
    double total=0;
    for (int p=0; p<numPoints-1; p++)
    {
        distance = distSqBtwPoints(p);
        total = total+distance;
    }
    return total;//ret totalDistSq
}


double distSqBtwPoints (int p)//func todetermine distance brwn two consecutive points
{
    double distx = ((points[p+1].xCoordinate)-(points[p].xCoordinate))*((points[p+1].xCoordinate)-(points[p].xCoordinate));
    //cout << distx << endl;
    double disty = ((points[p+1].yCoordinate)-(points[p].yCoordinate))*((points[p+1].yCoordinate)-(points[p].yCoordinate));
    //cout << disty << endl;
    return (distx + disty);
}
