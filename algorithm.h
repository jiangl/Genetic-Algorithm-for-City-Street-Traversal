#ifndef StopLights_Algorithm_h
#include <set>
#include <QGraphicsScene>
#include "path.h"

class Algorithm {
public:
	Environment *environ;
    MainWindow *window;
	vector< vector<signed int> > bonus;
	vector< vector<char> > display;
	set<Path*> top_all;
	set<Path*> top_gen;
	vector<int> record;
	Path* best_path;
	Path curr_path;
	int curr_trial;
	int curr_gen;
	MainWindow* win;
    Algorithm(Environment *e);
	virtual void findProb(Neighbor *, Environment *) = 0;
	void addBonus(Neighbor *);
	void noBack(Neighbor *);
	Point randNeighbor(Neighbor *);
    void nap(unsigned int mseconds);

    Path* runTrial();
	void runGen();
    void printPath(Path *);
};


class MoveAngle : public Algorithm {
public:
    MoveAngle(Environment *e) : Algorithm(e) {}
	void findProb(Neighbor *, Environment *);
};


class MoveWeight : public Algorithm {
public:
    MoveWeight(Environment *e, int w) : Algorithm(e) { weight = w; }
	void findProb(Neighbor *, Environment *);
private:
	int weight;
};


class MoveRand : public Algorithm {
public:
    MoveRand(Environment *e) : Algorithm(e) {}
	void findProb(Neighbor *, Environment *);
};

#define StopLights_Algorithm_h
#endif
