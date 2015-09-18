// Author Sean Davis
#include "router.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

Router::Router(short **map, int width)
{
  myWidth = width;
  int inf = 9000000;

  short time;
  for (int row = 0; row < width; row++)
    for (int col = 0; col < width; col++)
	{
	  if (row != 0)
	  {
	    time = map[row-1][col] - map[row][col];
		time = time * time + 10;
		plot[row][col].list[0].time = time; // up
		plot[row][col].list[0].cord.row = row-1;
		plot[row][col].list[0].cord.col = col;
	  }
	  else plot[row][col].list[0].time = 0;
	  if (row != width-1)
	  {
	    time = map[row+1][col] - map[row][col];
		time = time * time + 10;
		plot[row][col].list[1].time = time; // down
		plot[row][col].list[1].cord.row = row+1;
		plot[row][col].list[1].cord.col = col;
	  }
	  else plot[row][col].list[1].time = 0;
	  if (col != 0)
	  {
	    time = map[row][col-1] - map[row][col];
		time = time * time + 10;
		plot[row][col].list[2].time = time; // left
		plot[row][col].list[2].cord.row = row;
		plot[row][col].list[2].cord.col = col-1;
	  }
	  else plot[row][col].list[2].time = 0;
	  if (col != width-1)
	  {
	    time = map[row][col+1] - map[row][col];
		time = time * time + 10;
		plot[row][col].list[3].cord.row = row;
		plot[row][col].list[3].cord.col = col+1;
		plot[row][col].list[3].time = time; // right
	  }
	  else plot[row][col].list[3].time = 0;
	  plot[row][col].row = row;
	  plot[row][col].col = col;
	  for (int i = 0; i < 20; i++)
	  {
	    plot[row][col].dv[i] = inf;
	    plot[row][col].pv[i].row = -1;
	    plot[row][col].pv[i].col = -1;
	    plot[row][col].known[i] = false;
	  }
	}

} // Router()

void Router::Dijkstra(const Coordinates* cities, int pos, int count)
{
  Coordinates source = cities[pos]; // pos = current city..

  BinaryHeap<Plot> heap(myWidth*myWidth+(myWidth/2));
  int curRow = source.row; // start at cities[pos]
  int curCol = source.col;
  Plot min;
   
  plot[curRow][curCol].dv[pos] = 0; // make dv = 0

  heap.insert(plot[curRow][curCol], pos); // put it into heap!
 
  int i = 0, j = 0, dist, row, col;
  AdjList list[4];
  while (j != count)
  {
    heap.deleteMin(pos); // delete source off heap
    plot[curRow][curCol].known[pos] = true;  // mark as true
	dist = plot[curRow][curCol].dv[pos]; // set the source's dv as the initial distance for all edges in dijkstra
    for (i = 0; i < 4; i++)
	{
	  list[i] = plot[curRow][curCol].list[i];
	  
      if (list[i].time != 0)
	  {
	    int curDist = dist + list[i].time;
		row = list[i].cord.row;
		col = list[i].cord.col;
	    if (curDist < plot[row][col].dv[pos] && plot[row][col].known[pos] == false)
		{
		  plot[row][col].pv[pos].row = curRow;
		  plot[row][col].pv[pos].col = curCol;
		  plot[row][col].dv[pos] = curDist;
	      heap.insert(plot[row][col], pos); //insert adjacent edges
		  min = heap.findMin();
		}
	  }
	}
    min = heap.findMin();
	while (plot[min.row][min.col].known[pos])
	{
	  heap.deleteMin(pos);
	  min = heap.findMin();
	}
	curRow = min.row;
	curCol = min.col;
	j = 0;
	for (int k = 0; k < count; k++)
	{
	  if (plot[cities[k].row][cities[k].col].known[pos])
	    j++;
	  else break;
	}
  }

}

int cmp(const void *ptr1, const void *ptr2)
{
  if (((Forest*) ptr1)->edge_value > ((Forest*) ptr2)->edge_value)
    return 1;
  else if (((Forest*) ptr1)->edge_value == ((Forest*) ptr2)->edge_value)
    return 0;
  return -1;
}

int Router::parent(Set set_A)
{
  if (set_A.value < 0)
    return set_A.pos;
  return parent(set_array[set_A.value]);
}

void Router::Kruskal(const Coordinates *cities, const int count, 
Coordinates **paths, int pathCounts[])
{
  int i = 0, j = 0;
  int total_time = 0;
  while (j != count-1)
  {
	int row = graph[i].edge[1].row;
	int col = graph[i].edge[1].col;
	int firstRow = graph[i].edge[0].row;
	int firstCol = graph[i].edge[0].col;
	int firstCity = graph[i].firstCity;
	int secCity = graph[i].secCity;
	int current = parent(set_array[firstCity]);
	int other = parent(set_array[secCity]);
	
	if (current != other)
	{
	  int iter = 1;
	  if (set_array[current].value < set_array[other].value)
	  {
	    set_array[current].value += set_array[other].value;
		set_array[other].value = current;
		paths[j][0].row = row;
	    paths[j][0].col = col;
		total_time += plot[row][col].dv[firstCity];
		//cout << "TIME: " << plot[row][col].dv[firstCity] << "::::" ;
	    //cout << "Path#" << j << ": (" << paths[j][0].row << ',' << paths[j][0].col << ") ";
		while (1)
		{
			row = paths[j][iter-1].row;
			col = paths[j][iter-1].col;
			if (row == firstRow && col == firstCol)
			  break;
			paths[j][iter].row = plot[row][col].pv[firstCity].row;
			paths[j][iter].col = plot[row][col].pv[firstCity].col;
			//cout << '(' << paths[j][iter].row << ',' << paths[j][iter].col << ") ";
			iter++;
		}
	  }
	  else
	  {
	    set_array[other].value += set_array[current].value;
		set_array[current].value = other;
		paths[j][0].row = row;
	    paths[j][0].col = col;
		total_time += plot[row][col].dv[firstCity];
		//cout << "TIME: " << plot[row][col].dv[firstCity] << "::::" ;
	    //cout << "Path#" << j << ": (" << paths[j][0].row << ',' << paths[j][0].col << ") ";
		while (1)
		{
			row = paths[j][iter-1].row;
			col = paths[j][iter-1].col;
			if (row == firstRow && col == firstCol)
			  break;
			paths[j][iter].row = plot[row][col].pv[firstCity].row;
			paths[j][iter].col = plot[row][col].pv[firstCity].col;
			//cout << '(' << paths[j][iter].row << ',' << paths[j][iter].col << ") ";
			iter++;
		}
		
	  }
	  pathCounts[j] = iter;
	  //cout << endl;
	  j++;
	}
	i++;
  }
  //cout << "TOTAL TIME: "<< total_time << endl;
  //pathCount[]

} // Kruskal()

void Router::findRoutes(const Coordinates *cityPos, int cityCount, 
  Coordinates **paths, int pathCounts[])
{
  int inf = 215680;
  set_array = new Set[cityCount];
  for (int k = 0; k < cityCount; k++)
  {
	for (int i = 0; i < cityCount; i++)
	{
		temp[k*cityCount+i].edge[0].row = -1;
		temp[k*cityCount+i].edge[0].col = -1;
		temp[k*cityCount+i].edge[1].row = -1;
		temp[k*cityCount+i].edge[1].col = -1;
		temp[k*cityCount+i].firstCity = -1;
		temp[k*cityCount+i].secCity = -1;
		temp[k*cityCount+i].edge_value = inf;
	}
	set_array[k].value = -1;
	set_array[k].pos = k;

  }
  int size = 0;
  for (int i = 0; i < cityCount; i++)
  {
    Dijkstra(cityPos, i, cityCount);
	for(int j = 0; j < cityCount; j++)
	{
	  if (i!= j)
	    if (plot[cityPos[j].row][cityPos[j].col].dv[i] < temp[i*cityCount+j].edge_value)
		  if (plot[cityPos[j].row][cityPos[j].col].dv[i] != temp[j*cityCount+i].edge_value)
	      {
		    temp[i*cityCount+j].edge_value = plot[cityPos[j].row][cityPos[j].col].dv[i];
		    temp[i*cityCount+j].edge[0].row = cityPos[i].row;
		    temp[i*cityCount+j].edge[0].col = cityPos[i].col;
		    temp[i*cityCount+j].edge[1].row = cityPos[j].row;
		    temp[i*cityCount+j].edge[1].col = cityPos[j].col;
		    temp[i*cityCount+j].firstCity = i;
			temp[i*cityCount+j].secCity = j;
		    /*cout << "PREV NODE CHECK: " << cityPos[i].row << ',' <<
			cityPos[i].col << " ; " << cityPos[j].row << ',' << cityPos[j].col << " :: "
			<< plot[cityPos[j].row][cityPos[j].col].pv[i].row << ','
			<< plot[cityPos[j].row][cityPos[j].col].pv[i].col << endl;*/
		    size++;
		  }
	}
  }

  qsort(temp, cityCount*cityCount, sizeof(Forest), cmp);
  graph = new Forest[size];
  memcpy(graph, temp, sizeof(Forest)*size);
  for (int i = 0; i < size; i++)
  {
    /*cout << "EDGES: " << graph[i].edge[0].row << ',' <<
	graph[i].edge[0].col << "; " << graph[i].edge[1].row << ',' <<
	graph[i].edge[1].col << "  VALUE: " << graph[i].edge_value << 
	"  FIRST CITY: " << graph[i].firstCity << endl;*/
  }
  Kruskal(cityPos, cityCount, paths, pathCounts);
} // findRoutes()









