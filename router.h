// Author: Kim Nguyen and Ngoc Bui

#ifndef ROUTER_H
  #define ROUTER_H

#include "RouterRunner.h"
#include "BinaryHeap.h"

using namespace std;

typedef struct
{
  short time;
  Coordinates cord;
} AdjList;

typedef struct
{
  /*for an individual city or plot*/
  short dv[20];
  int row;
  int col;
  Coordinates pv[20];
  bool known[20];
  AdjList list[4];
} Plot;

typedef struct
{
  Coordinates edge[2];
  int firstCity;
  int secCity;
  short edge_value;
} Forest;

typedef struct
{
  int value;
  int pos;
} Set;

class Router
{
public:
  Router(short **map, int width);
  void findRoutes(const Coordinates *cityPos, int cityCount, Coordinates **paths, int pathCounts[]);
  void Dijkstra(const Coordinates* cities, int pos, int count);
  void Kruskal(const Coordinates *cities, const int count, Coordinates **paths, int pathCounts[]);
  int parent(Set);
private:
  int myWidth;
  Plot plot[350][350];
  /*for all cities*/
  Forest temp[400];
  Forest *graph;
  Set *set_array;
};


#endif
