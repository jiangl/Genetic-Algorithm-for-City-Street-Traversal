#include <QtGui/QApplication>
#include <QTimer>
#include "mainwindow.h"
#include "road.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <ctime>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

#include "environment.h"
#include "path.h"
#include "algorithm.h"

#define PI 3.14159

void Algorithm::nap(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

// Environment constructor
Environment::Environment() {
}


// Debugging Constructor
Environment::Environment(int n){
    grid_dim.x = 10;
    grid_dim.y = 10;
    loc_start.x = 2;
    loc_start.y = 1;
    loc_end.x = 9;
    loc_end.y = 9;
    num = 80;
    trials = 4;
    gen = 100;
    freq = 3;
    n = 0;
}


// Sets stoplight values in grid[ ][ ] vector.
// The sign stands for (-) green, (+) red.
// The number stands for time left before it changes.
void Environment::setLights(){

        cout << "Making city grid... ";

        for (int i = 0; i < grid_dim.x; i++) {
                vector<signed int> row; // temp 1-d vector
                for (int j = 0; j < grid_dim.y; j++) { // the next bit is nasty, but works
                        row.push_back(0);
                } // note the -1^rand() for sign
                grid.push_back(row); // store result in grid vector array
        }

    // nap(500);
        cout << "Done." << endl;

        cout << "Setting stoplights... ";

        for (int i = 0; i < num; i++) {
                int rnd_int = (int)((float)(rand() % freq + 1) * (pow ((float)(-1), (float)(rand() % 2 + 1))));
                int rnd_x = rand() % grid_dim.x;
                int rnd_y = rand() % grid_dim.y;
                while (grid[rnd_x][rnd_y] != 0) {
                        rnd_x = rand() % grid_dim.x;
                        rnd_y = rand() % grid_dim.y;
                }
                grid[rnd_x][rnd_y] = rnd_int;
        }

    // nap(500);
        cout << "Done." << endl;
}

//const char* Environment::printGrid(Path *p) {     // Lisa's Version to print to textEdit
//    string buffer;
//    string temp;
//    stringstream sstemp;

//    for (int i = 0; i < 3; i++) {
//        buffer.append(" ");
//    }

//    string l_bumper, r_bumper; // make if/else statements easy to write
//    Point curr_loc = loc_start; // just initializes curr_loc, its not constant!
//    if(p != NULL) // NULL means there's no path yet, used at beginning
//        curr_loc = p->last->loc;
//    for(int j = 0; j < grid_dim.y; j++){ // where the magic happens
//        for (int i = 0; i < grid_dim.x; i++){
//            if((curr_loc.x == i) && (curr_loc.y == j)) {
//                r_bumper = ">";
//                (grid[i][j] >= 0) ? l_bumper = "<+" : l_bumper = "<";
//            }
//            else if((loc_end.x == i) && (loc_end.y == j)) {
//                r_bumper = "]";
//                (grid[i][j] >= 0) ? l_bumper = "[+" : l_bumper = "[";
//            }
//            else {
//                r_bumper = " ";
//                (grid[i][j] >= 0) ? l_bumper = " +" : l_bumper = " ";
//            }

//            temp = "";
//            sstemp.str("");
//            sstemp.clear();

//            buffer.append(l_bumper);
//            sstemp << grid[i][j];
//            sstemp >> temp;

//            buffer.append(temp);
//            buffer.append(r_bumper);
//        }
//        buffer.append("\n");
//    }

//    for (int i = 0; i < 3; i++) {
//        buffer.append(" ");
//    }

//    const char * c = buffer.c_str();
//    return c;

//}

// Prints the grid[ ][ ].
// "< >" for car location, "[ ]" for goal.
// Takes a Path *p so that this function can be called anytime throught the program.
QString Environment::printGrid(Path *p) {      // Don's version to print to consoleEdit

    for (int i = 0; i < 3; i++) {
        cout << endl;
    }

 string l_bumper, r_bumper; // make if/else statements easy to write
 Point curr_loc = loc_start; // just initializes curr_loc, its not constant!
 if(p != NULL) // NULL means there's no path yet, used at beginning
 curr_loc = p->last->loc;
 for(int j = 0; j < grid_dim.y; j++){ // where the magic happens
 for (int i = 0; i < grid_dim.x; i++){
 if((curr_loc.x == i) && (curr_loc.y == j)) {
 r_bumper = ">";
 (grid[i][j] >= 0) ? l_bumper = "<+" : l_bumper = "<";
 }
 else if((loc_end.x == i) && (loc_end.y == j)) {
 r_bumper = "]";
 (grid[i][j] >= 0) ? l_bumper = "[+" : l_bumper = "[";
 }
 else {
 r_bumper = " ";
 (grid[i][j] >= 0) ? l_bumper = " +" : l_bumper = " ";
 }
 cout << l_bumper << grid[i][j] << r_bumper;
 }
 cout << endl;
 }

 for (int i = 0; i < 3; i++) {
 cout << endl;
 }

}

// Increments "time" variable for each stoplight by 1 (sometimes up, sometimes down).
// red lights (+) count down.
// green lights (-) count up.
// when a light (of either color) reaches 0, it changes color (sign).
void Environment::changeLights() {
        int k;
        for (int i = 0; i < grid_dim.x; i++) {
                for (int j = 0; j < grid_dim.y; j++) {
                        k = (int)grid[i][j];
                        if (k < 0) { // I'm a green light, so
                                (k != -1) ? k++ : k = freq; // increment up!
                        }
                        else if (k > 0) { // I'm a red light, so
                                (k != 1) ? k-- : k = -freq; // increment down!
                        }
                        else if (k == 0)
                                k = k;
                        else { // Should never see this.
                                cout << "Error in changeLights()!" << endl;
                        }
                        grid[i][j] = k; // Save changes.
                }
        }
}

// Path constructor (default constructor), initializing everything to NULL/0
Path::Path() : first(NULL), last(NULL), length(0), time(0) {}

// Path constructor (copy constructor).
// You give me a starting point, I give you a Head + 1st Node. Booyah!
Path::Path(Point start) {
        Stoplight *s = new Stoplight(start); // The first node.
        first = last = s; // Which is pointed to by both the first & last.
        length = 1; // And the lenght is 1.
        time = 0; // But the time is 0.
}

////Path destructor
//Path::~Path(){
//Stoplight* die = first->next;

//while(die != NULL){
//delete(first);
//first = die;
//die = first->next;
//}

//}

//Path destructor
/*Path::~Path(){
Stoplight* die = first->next;
delete(last);
while(die != NULL){
delete(first);
first = die;
die = first->next;
}

}*/

// Stoplight constructor (copy constructor).
Stoplight::Stoplight(Point point) : next(NULL), loc(point) {}

// Moves car (but what does this mean!?).
// Basically, pushes a node onto the back of the linked list.
// i.e. takes a point, makes a new Stoplight object, puts at end of Path object.
void Path::moveCar(Point point, Environment *environ) { // point is where we move to
Stoplight *s = new Stoplight(point); // new node
if (environ->grid[last->loc.x][last->loc.y] <= 0) // am I green or red?
s->t_here = 0; // ...I'm green, so I don't wait.
else
s->t_here = environ->grid[last->loc.x][last->loc.y]; // ...I'm red, so I do wait.
last->next = s;
last = s;
length++; // Path is one node longer.
time = time + last->t_here + 1; // new time takes red light into consideration.
}

Point* Path::get(int index) {
Stoplight *s = this->first;
for (int i = 0; i < index; i++)
s = s->next;
return &s->loc;
}



// Neighbor constructor (copy constructor).
Neighbor::Neighbor(Point *point) {
here = *point;
up = Point(point->x, point->y - 1); // point above current location
down = Point(point->x, point->y + 1); // point below...
left = Point(point->x - 1, point->y); // to the left...
right = Point(point->x + 1, point->y); // and to the right.
u_prob = d_prob = l_prob = r_prob = 25; // give everyone the same 25% chance initially
}

Algorithm::Algorithm(Environment *e) {

    environ = e;



    for (int i = 0; i < e->grid_dim.x + 2; i++) {
        vector<int> row;
        for (int j = 0; j < e->grid_dim.y + 2; j++) {
            if ((i == 0) || (j == 0) || (i == e->grid_dim.x + 1) || (j == e->grid_dim.y + 1))
                row.push_back(0);
            else
                row.push_back(1);
        }
        bonus.push_back(row);
    }

for (int i = 0; i < environ->grid_dim.x; i++) {
vector<char> row;
for (int j = 0; j < environ->grid_dim.y; j++) {
row.push_back('_');
}
display.push_back(row);
}
}

void Algorithm::addBonus(Neighbor *n) {
n->u_prob *= bonus[n->up.x + 1][n->up.y + 1];
n->r_prob *= bonus[n->right.x + 1][n->right.y + 1];
n->d_prob *= bonus[n->down.x + 1][n->down.y + 1];
n->l_prob *= bonus[n->left.x + 1][n->left.y + 1];
float p_sum = (float)(n->u_prob + n->r_prob + n->d_prob + n->l_prob);
n->u_prob = (int)( 100*(float)n->u_prob / p_sum );
n->r_prob = (int)( 100*(float)n->r_prob / p_sum );
n->d_prob = (int)( 100*(float)n->d_prob / p_sum );
n->l_prob = (int)( 100*(float)n->l_prob / p_sum );
}

void Algorithm::noBack(Neighbor *n) {
if ((n->up.x == curr_path.prev->x) && (n->up.x == curr_path.prev->x))
n->u_prob = 0;
else if ((n->right.x == curr_path.prev->x) && (n->right.x == curr_path.prev->x))
n->r_prob = 0;
else if ((n->down.x == curr_path.prev->x) && (n->down.x == curr_path.prev->x))
n->d_prob = 0;
else if ((n->left.x == curr_path.prev->x) && (n->left.x == curr_path.prev->x))
n->l_prob = 0;
float p_sum = (float)(n->u_prob + n->r_prob + n->d_prob + n->l_prob);
n->u_prob = (int)( 100*(float)n->u_prob / p_sum );
n->r_prob = (int)( 100*(float)n->r_prob / p_sum );
n->d_prob = (int)( 100*(float)n->d_prob / p_sum );
n->l_prob = (int)( 100*(float)n->l_prob / p_sum );
}


// Makes a choice for movement!
// Needs to know about neighbors to make choice (where are they? probabilities?)
// Returns point to be moved to.
Point Algorithm::randNeighbor(Neighbor *n) {
Point p; // the point to be returned
int rnd = rand() % 100; // random number between 0 and 99
int dir = 4; // four directions: 0 - 3
// 4 means there's an error
(rnd < n->u_prob) ? dir = 0 : rnd -= n->u_prob;
((rnd < n->r_prob) && (dir==4)) ? dir = 1 : rnd -= n->r_prob; // this makes the
((rnd < n->d_prob) && (dir==4)) ? dir = 2 : rnd -= n->d_prob; // weighted choice
((rnd < n->l_prob) && (dir==4)) ? dir = 3 : rnd -= n->l_prob;

switch (dir) { // checks if point is valid!
case 0:
p = n->up;
break;
case 1:
p = n->right;
break;
case 2:
p = n->down;
break;
case 3:
p = n->left;
break;
case 4:
p = n->here;
}
return p;
}

// The workhorse of the program!
// This makes a path, picks directions, and moves the car until it reaches the goal.
// Returns path for this trial.
Path* Algorithm::runTrial() {

    curr_path = Path(environ->loc_start); // temp path

    Neighbor n = Neighbor(&environ->loc_start); // neighbor (to be reused a lot here)

    // cout << path.last->loc.toString() << endl;
    // cout << environ->loc_end.toString() << endl;

    while ( curr_path.last->loc != environ->loc_end ) { // Q. Am I at the goal?

        n = Neighbor(&(curr_path.last->loc)); // find neighbors of current location
        findProb(&n, environ); // compute their probabilities
        addBonus(&n);
        curr_path.moveCar(randNeighbor(&n), environ); // pick a direction, and move the car!
        // environ->printGrid(&path);
        printPath(&curr_path);
        environ->changeLights();
        nap(1000);
    }
    return &curr_path;
}

void Algorithm::runGen() {

    for(curr_trial = 0; curr_trial < environ->trials; curr_trial++) { // trial loop
        Path* path = runTrial(); // run trial, find solution
        top_gen.insert(path); // store in set
};

    best_path = *(top_gen.begin()); // pick best one
    top_all.insert(*(top_gen.begin()));
    record.push_back((*top_gen.begin())->time);

    for(int i = 1; i < best_path->length; i++) // mod bonus vector
        bonus[best_path->get(i)->x + 1][best_path->get(i)->y + 1] += 1;

}

void Algorithm::printPath(Path *p) {

    char c;
    for (int i = 0; i < environ->grid_dim.x; i++) {
        for (int j = 0; j < environ->grid_dim.y; j++) {

            if (environ->grid[i][j] <= 0) { // I'm a green light, so
                c = ' ';//'*';
            }
            else if (environ->grid[i][j] > 0) { // I'm a red light, so
                c = ' ';//'%';
            }
            else { // Should never see this.
                cout << "Error in printPath()!" << endl;
            }
            display[i][j] = c; // Save changes.
        }
}

    display[environ->loc_start.x][environ->loc_start.y] = 'A';
    display[environ->loc_end.x][environ->loc_end.y] = 'B';


    for(int i = 1; i < p->length - 1; i++) {
        switch (display[p->get(i)->x][p->get(i)->y]) {
        case ' ':
            display[p->get(i)->x][p->get(i)->y] = '0';
            break;
        case '0':
            display[p->get(i)->x][p->get(i)->y] = '1';
            break;
        case '1':
            display[p->get(i)->x][p->get(i)->y] = '2';
            break;

        case '2':
            display[p->get(i)->x][p->get(i)->y] = '3';
            break;

        default:
            display[p->get(i)->x][p->get(i)->y] = 'X';
            break;
        }
    }
// cout << "Fastest time: " << best_path->time << endl;
// cout << "Fastest path is..." << endl;

    cout << "\n\n\n\n\n\n\n\n\n" << endl;
    for(int j = 0; j < environ->grid_dim.y; j++){
        for (int i = 0; i < environ->grid_dim.x; i++){
            cout << " " << display[i][j];
        }
//        cout << "\t\t";
//        for (int i = 1; i < environ->grid_dim.x; i++) {

//            cout << " " << bonus[i][j + 1];
//        }
        cout << endl;
    }

cout << "\n\nTrial " << curr_trial + 1 << endl;
cout << "Generation " << curr_gen + 1 << endl;

}


void MoveAngle::findProb(Neighbor *n, Environment *environ) {
if (curr_path.last->loc != environ->loc_end) {
double y = (double)(environ->loc_end.y - curr_path.last->loc.y); // y_goal - y_current
double x = (double)(environ->loc_end.x - curr_path.last->loc.x); // x_goal - x_current
double angle = atan2 (y, x); // angle between goal and "Eastward"
double u_diff, d_diff, l_diff, r_diff; // angles between goal and directions
u_diff = fabs( angle + PI/2 ); // uh.. well not really angles
d_diff = fabs( angle - PI/2 ); // more like the inverse
l_diff = fabs( angle - PI ); // of the absolute value of them
r_diff = fabs( angle );

if ((int)(u_diff*180/PI) == 0) {
n->u_prob = 90;
n->l_prob = n->r_prob = 4;
n->d_prob = 2;
}
else if ((int)(d_diff*180/PI) == 0) {
n->d_prob = 90;
n->l_prob = n->r_prob = 4;
n->u_prob = 2;
}
else if ((int)(l_diff*180/PI) == 0) {
n->l_prob = 90;
n->u_prob = n->d_prob = 4;
n->r_prob = 2;
}
else if ((int)(r_diff*180/PI) == 0) {
n->r_prob = 90;
n->u_prob = n->d_prob = 4;
n->l_prob = 2;
}
else {
u_diff = 1/u_diff;
d_diff = 1/d_diff;
l_diff = 1/l_diff;
r_diff = 1/r_diff;
double sum_diff = u_diff + d_diff + l_diff + r_diff;
n->u_prob = (int)((u_diff / sum_diff) * 100);
n->d_prob = (int)((d_diff / sum_diff) * 100);
n->l_prob = (int)((l_diff / sum_diff) * 100);
n->r_prob = (int)((r_diff / sum_diff) * 100);
}
}
else
n->u_prob = n->d_prob = n->l_prob = n->r_prob = 0;
}

void MoveWeight::findProb(Neighbor *n, Environment *environ) {
if (curr_path.last->loc != environ->loc_end) {
int y = (environ->loc_end.y - curr_path.last->loc.y); // y_goal - y_current
int x = (environ->loc_end.x - curr_path.last->loc.x); // x_goal - x_current

int corner_high = (int) ((float)weight / 2);
int corner_low = (int) ((float)(100-weight) / 2);
int parallel_high = weight;
int parallel_low = (int) ((float)(100-weight) / 3);

if (x > 0) {
if (y > 0) {
n->r_prob = n->d_prob = corner_high;
n->l_prob = n->u_prob = corner_low;
}
else if (y < 0) {
n->r_prob = n->u_prob = corner_high;
n->l_prob = n->d_prob = corner_low;
}
else {
n->r_prob = parallel_high;
n->l_prob = n->d_prob = n->u_prob = parallel_low;
}
}
else if (x < 0) {
if (y > 0) {
n->l_prob = n->d_prob = corner_high;
n->r_prob = n->u_prob = corner_low;
}
else if (y < 0) {
n->l_prob = n->u_prob = corner_high;
n->r_prob = n->d_prob = corner_low;
}
else {
n->l_prob = parallel_high;
n->r_prob = n->d_prob = n->u_prob = parallel_low;
}
}
else if (x == 0) {
if (y > 0) {
n->d_prob = parallel_high;
n->r_prob = n->l_prob = n->u_prob = parallel_low;;
}
else if (y < 0) {
n->u_prob = parallel_high;
n->r_prob = n->d_prob = n->l_prob = parallel_low;
}
else
cout << "Error in MoveWeight::findProb()" << endl;
}
else
cout << "Error in MoveWeight::findProb()" << endl;
}
else
n->u_prob = n->d_prob = n->l_prob = n->r_prob = 0;
}

void MoveRand::findProb(Neighbor *n, Environment *environ) {
    if (curr_path.last->loc != environ->loc_end)
        n->u_prob = n->d_prob = n->l_prob = n->r_prob = 25;
    else
        n->u_prob = n->d_prob = n->l_prob = n->r_prob = 0;
}

int main(int argc, char *argv[]){

    QApplication app(argc, argv);;
    MainWindow window;
    window.setWindowTitle("Stoplights");
    window.show();

    return app.exec();
}
