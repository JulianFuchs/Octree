// Octree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <utility>
#include <iostream>
#include <string>
#include <float.h>
#include <math.h>
#include <vector>


using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	pair<double,double> xrange;
	pair<double,double> yrange;
	pair<double,double> zrange;

	xrange = make_pair(-10.0,10.0);
	yrange = make_pair(-10.0,10.0);
	zrange = make_pair(-10.0,10.0);

	Octree octree(xrange,yrange,zrange);
	return 0;
}

class Octree
{
	public: const static int splitChildren = 3;
			
	private: 
		double minLength;
		pair<double,double> xrange;
		pair<double,double> yrange;
		pair<double,double> zrange;
	
		vector<Octree> regions;
	
		vector<vector<double>> pointsContained;
	
	public:
		Octree()
		{
			xrange = make_pair(DBL_MAX,DBL_MAX);
			yrange = make_pair(DBL_MAX,DBL_MAX);
			zrange = make_pair(DBL_MAX,DBL_MAX);
			
			regions = vector<Octree>();
			pointsContained = vector<vector<double>>();
			
		}
		
		Octree( pair<double,double> xrange,
				pair<double,double> yrange,
				pair<double,double> zrange )
		{
			minLength = 0.1;
			this -> xrange = xrange;
			this -> yrange = yrange;
			this -> zrange = zrange;
			
			regions = vector<Octree>();
			pointsContained = vector<vector<double>>();
			
		}
		
		void insert(vector<double> point)
		{
			// this node can still store points
			//if (pointsContained.size() < splitChildren 
				//	&& xrange.second-xrange.first <= minLength
					//&& yrange.second-yrange.first <= minLength
				//	&& zrange.second-zrange.first <= minLength)
			if (pointsContained.size() < splitChildren)
			{
				cout << "Inserted new point" << endl;
				pointsContained.push_back(point);
			}
			
			// this node reached its limit of points. We need to slit
			// the octree into 8 new octrees and need to distribute the
			// points contained into the new octrees
			else
			{
				// initialize the regions vector
				regions.resize(8);
				
				// find the splits
				double x2 = xrange.first + (xrange.second - xrange.first)/2;
				double y2 = yrange.first + (yrange.second - yrange.first)/2;
				double z2 = zrange.first + (zrange.second - zrange.first)/2;
				
				pair<double,double> xrange1(xrange.first,x2);
				pair<double,double> yrange1(yrange.first,y2);
				pair<double,double> zrange1(zrange.first,z2);
				
				pair<double,double> xrange2(x2,xrange.second);
				pair<double,double> yrange2(y2,yrange.second);
				pair<double,double> zrange2(z2,zrange.second);
			
				// create the new octrees
				regions[0] = Octree(xrange1,yrange1,zrange1);
				regions[1] = Octree(xrange1,yrange1,zrange2);
				regions[2] = Octree(xrange1,yrange2,zrange1);
				regions[3] = Octree(xrange1,yrange2,zrange2);
				
				regions[4] = Octree(xrange2,yrange1,zrange1);
				regions[5] = Octree(xrange2,yrange1,zrange2);
				regions[6] = Octree(xrange2,yrange2,zrange1);
				regions[7] = Octree(xrange2,yrange2,zrange2);
				
				// now insert all points contained in the current octree
				// into the newly generated ones
				
				for (int i = 0; i < pointsContained.size(); i++)
				{
					vector<double> p = pointsContained[i];
					
					bool inX2 = p[0]-x2 > 0.0;
					bool inY2 = p[1]-y2 > 0.0;
					bool inZ2 = p[2]-z2 > 0.0;
					
					int index = 4*inX2 + 2*inY2 + 1*inZ2;
					
					regions[index].insert(p);
				}
				
				//delete the points contained
				pointsContained.clear();
			}
		}
		
		void print()
		{
			if (regions.size() == 0)
			{
				cout << "This octree has no sub regions. It contains the following values:" << endl;
				
				for (int i = 0; i < pointsContained.size(); i++)
				{
					cout << pointsContained[i][1] << ", ";
				}
				
				cout << endl;
			}
			
			else
			{
				cout << "This octree has the following subregions:" << endl;
				for(int i = 0; i < 8; i++)
				{
					cout << "Region " << i << ":" << endl;
					regions[i]. print();
				}
				
				cout << endl;				
			}
		}
};

