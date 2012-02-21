#ifndef StopLights_Environment_h

using namespace std;

struct Point {
	int x, y;
	Point() { this->x = x; this->y = y; }
	Point(int a, int b) : x(a), y(b) {}
	bool operator< (Point &p) {
		return (x < p.x) && (y < p.y);
	}
	bool operator<= (Point &p) {
		return (x < p.x) || (y < p.y);
	}
	bool operator> (Point &p) {
		return (x > p.x) && (y > p.y);
	}
	bool operator>= (Point &p) {
		return (x > p.x) || (y > p.y);
	}
	bool operator!= (Point &p) {
		return ( !((x == p.x) && (y == p.y)) );
	}
	bool operator== (Point &p) {
		return (x == p.x) && (y == p.y);
	}

};

class Path;

class Environment {
public:
	Point grid_dim;                     // x,y of grid dimensions
	Point loc_start;                    // start point
	Point loc_end;                      // end point
	int trials;                         // current number of trials
    int gen;
	int freq;
    int num;                            // number of stoplights
	vector< vector<signed int> > grid;  // 2-d vector grid
    Environment();                      // constructor
    Environment(int);					// debugging constructor
    void setLights();                   // adds stoplights, distributes randomly
    QString printGrid(Path *);                   // prints grid
	void changeLights();
};

#define StopLights_Environment_h
#endif
