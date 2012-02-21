#ifndef StopLights_Path_h

struct Stoplight {
	Stoplight *next;     // pointer to next node
	Point loc;           // node location
	int t_here;          // time spent at node
	Stoplight(Point );         // constructor
};

struct Neighbor {
	Point here, up, down, left, right;
	int u_prob, d_prob, l_prob, r_prob;
	Neighbor(Point *);    //constructor
};

class Path {
public:
	Stoplight *first;    // pointer to first node
	Stoplight *last;     // pointer to last node
	int length;          // length of linked list
	int time;            // time
	Point *prev;
	Path();              // constructor
	Path(Point );
	void moveCar(Point , Environment *);     // create new node and push to back
	bool operator< (const Path &p) const {
		if (this->time < p.time)
			return true;
		else
			return false;
	}
	Point* get(int );
//	~Path();             // destructor
};

#define StopLights_Path_h
#endif
