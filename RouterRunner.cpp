// Author Sean Davis
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "RouterRunner.h"
#include "CPUTimer.h"
#include "router.h"

using namespace std;

void readFile(const char *filename, short ***map, short ***map2, int *width,
  Coordinates **cityPos, int *cityCount)
{
  ifstream inf(filename);
  inf >> *cityCount >> *width;
  *cityPos = new Coordinates[*cityCount];
  *map = new short* [*width];
  *map2 = new short* [*width];
  for(int i = 0; i < *width; i++)
  {
    (*map)[i] = new short[*width];
    (*map2)[i] = new short[*width];
  }

  for(int i = 0; i < *cityCount; i++)
    inf >> (*cityPos)[i].row >> (*cityPos)[i].col;

  for(int row = 0; row < *width; row++)
    for(int col = 0; col < *width; col++)
    {
      inf >> (*map)[row][col];
      (*map2)[row][col] = (*map)[row][col];
    }
}  // readFile()

void printCoordinates(Coordinates coordinates, Coordinates coordinates2)
{
  cout << "Row: " << coordinates.row << " Col: " << coordinates.col;
  cout << " to Row: " << coordinates2.row << " Col: " << coordinates2.col  << endl;
} // printCoordinates()


int checkPath(Coordinates path[], int pathCount, int pathNum, short **map2, 
  int width, int argc)
{
  int total = 0, i;

  for(i = 0; i < pathCount - 1; i++)
  {
    Coordinates coordinates = path[i];
    Coordinates coordinates2 = path[i + 1];
   
    if(argc > 2)
      cout << " (" << path[i].row << "," << path[i].col << ')';
    
    
    if(coordinates.row < 0 || coordinates.col < 0
      || coordinates.row >= width || coordinates.col >= width)
    {
      cout << "Path #" << pathNum <<  " Coordinates #" << i << " has coordinate(s) outside the acceptable range:\n";
      printCoordinates(coordinates, coordinates2);
      exit(1);
    }

    if(abs(coordinates.row - coordinates2.row) > 1
      || abs(coordinates.col - coordinates2.col) > 1)
    {
       cout << "Path #" << pathNum << " Coordinates " << i << " and " << i + 1 
         << " are not adjacent.\n";
      printCoordinates(coordinates, coordinates2);
      exit(1);
    }

    if (coordinates.row != coordinates2.row && coordinates.col != coordinates2.col)
    {  
      cout << "Path #" << pathNum << " Coordinates " << i << " and " << i + 1 
        << " travel diagonally.\n";
      printCoordinates(coordinates, coordinates2);
      exit(1);
    }
      
    if(coordinates.row == coordinates2.row && coordinates.col == coordinates2.col)
    {
      cout << "Path #" << pathNum << " Coordinates " << i << " and " << i + 1
        << " are the same!\n";
      printCoordinates(coordinates, coordinates2);
      exit(1);
    }
      
    int elevationChange = abs(map2[coordinates.row][coordinates.col] 
      - map2[coordinates2.row][coordinates2.col]);
    total += (elevationChange * elevationChange) + 10;
  } // for each coordinates

   if(argc > 2)
      cout << " (" << path[i].row << "," << path[i].col << ')';
  return total;
}  // createAdjMap()


int checkRoutes(short **map2, const Coordinates *cityPos, int cityCount, Coordinates **paths,
  int pathCounts[50], int width, int argc)
{
  int total = 0, pathTotal;
  bool visited[50];
  
  memset(visited, 0, 50 * sizeof(bool) );
  
  for(int i = 0;  i < 50 && pathCounts[i] > 0; i++)
  {
    if(argc > 2)
      cout << "Path #" << i << ":";  
    
    pathTotal = checkPath(paths[i], pathCounts[i], i, map2, width, argc);
    
    if(argc > 2)
      cout << " Total: " << pathTotal << endl;
    
    total += pathTotal;
    
    for(int j = 0; j < cityCount; j++)
      if((paths[i][0].row == cityPos[j].row 
        && paths[i][0].col == cityPos[j].col)
        || (paths[i][pathCounts[i] - 1].row == cityPos[j].row
        && paths[i][pathCounts[i] - 1].col == cityPos[j].col ))
        visited[j] = true;
  } // for each path 
  
  for(int i = 0; i < cityCount; i++)
    if(!visited[i])
      cout << "City #" << i << " was not connected.\n";
  
  return total;
}  // checkRoute()


int main(int argc, char* argv[])
{
  short **map, **map2;
  int width, cityCount, pathCounts[50];
  Coordinates *cityPos;
  Router *router;
  CPUTimer ct;
  readFile(argv[1], &map, &map2, &width, &cityPos, &cityCount);
  Coordinates **paths = new Coordinates*[cityCount];
  
  for(int i = 0; i < cityCount; i++)
  {
    paths[i] = new Coordinates[width * width];  // maximum number of edges possible
    pathCounts[i] = 0;
  }
  
  ct.reset();
  router = new Router(map, width);

  for(int i = 0; i < width; i++)
    delete [] map[i];
  delete [] map;

  router->findRoutes((const Coordinates*) cityPos, cityCount, paths, pathCounts);
  double time = ct.cur_CPUTime();
  int trainTime = checkRoutes(map2, cityPos, cityCount, paths, pathCounts, 
    width, argc);
  cout << "CPU time: " << time << " Train time: " << trainTime << endl;
  
  return 0;
}

