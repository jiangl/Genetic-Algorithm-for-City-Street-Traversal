#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "road.h"
#include "light.h"
#include "environment.h"
#include "algorithm.h"
#include <set>
#include <sstream>
#include <iostream>
#include <ctime>
#include <qstring.h>
#include <cstring>
#include <string>
#include <streambuf>

extern stringstream holder;
//extern QGraphicsScene *scene;
//extern QGraphicsView *view;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

//void MainWindow::toConsole(string *s) {
//	ui->consoleEdit->setPlainText(QString::fromStdString(*s));
//}

void MainWindow::startClicked(){
//    Environment city = Environment(0);   //debugger constructor
    cout << "Hello!" << endl;
//	toConsole(&holder.str());

 Environment city;

 city.grid_dim.x = (int) ui->spinBox->value();
 city.grid_dim.y = (int) ui->spinBox_2->value();

 city.num = (int) ui->spinBox_3->value();
 city.freq = (int) ui->spinBox_4->value();

 city.loc_start.x = (int) ui->spinBox_5->value();
 city.loc_start.y = (int) ui->spinBox_6->value();
 city.loc_end.x = (int) ui->spinBox_7->value();
 city.loc_end.y = (int) ui->spinBox_8->value();

 city.gen = (int) ui->spinBox_9->value();
 city.trials = (int) ui->spinBox_10->value();

/********* Move Angle ***********/
    if(ui->comboBox->currentIndex()==0){

        QStringList list;
        list <<"Time";
        QTableWidget *widget = ui->tableWidget;
        widget->setRowCount(city.gen);
        widget->setColumnCount(1);
        widget->setHorizontalHeaderLabels(list);

        srand( (unsigned int)time(NULL) ); // seed rand() with runtime
        set<Path>::iterator it;
        city.setLights(); // set lights

    /********* Running Trials/Generations ***********/

              MoveAngle solution = MoveAngle(&city); // load city into algorithm
			  
              for (int i = 0; i < city.gen; i++) {   // for loop mimics runGen()
			   
                   ui->lcdNumber->setSegmentStyle(QLCDNumber::Filled);
                   ui->lcdNumber->display(i+1);
                   for(solution.curr_trial = 0; solution.curr_trial < city.trials; solution.curr_trial++) { // trial loop
                       Path* path = solution.runTrial(); // run trial, find solution
                       solution.top_gen.insert(path); // store in set


  /********* Graphics for StopLights ***********/

                       QGraphicsScene *lighttemp = new QGraphicsScene;
                       this->light_scene = lighttemp;
                       int xmax = city.grid_dim.x;
                       int ymax = city.grid_dim.y;
                       light_scene->setSceneRect(0, 0, xmax*50, ymax*50);
                       light_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
					   
                       for(int x=0; x<xmax; x++){
                           for(int y=0; y<ymax; y++){
                               Light *stoplight = new Light;
                               stoplight->setPos(x*50,y*50);

                               QColor light;
                               if(city.grid[x][y] > 0){
                                   light = Qt::red;   }          // red light
                               else if(city.grid[x][y] < 0){
                                   light = Qt::green;      }      // green light
                               else{ light.setHsv(0, 0, 255, 0);}  // clear = no light

                               stoplight->color = light;
                               light_scene->addItem(stoplight);
                           } // end light y brace
                       } // end light x brace

                       ui->graphicsView2->setCacheMode(QGraphicsView::CacheBackground);
                       ui->graphicsView2->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
                       ui->graphicsView2->setDragMode(QGraphicsView::ScrollHandDrag);
                       ui->graphicsView2->resize(xmax*50+2,ymax*50+2);
                       ui->tabWidget->setMinimumSize(xmax*50+10,ymax*50+40);
                       ui->tabWidget->setMaximumSize(xmax*50+10,ymax*50+40);
                       ui->graphicsView2->setScene(light_scene);
                       QApplication::processEvents();
                     // END LIGHT GRAPHICS

                   } // trial loop

                   solution.best_path = *(solution.top_gen.begin()); // pick best one
                   solution.top_all.insert(*(solution.top_gen.begin()));
                   solution.record.push_back((*solution.top_gen.begin())->time);

				   QLabel *tableItem = new QLabel();
				   tableItem->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
				   tableItem->setNum(solution.best_path->time);
				   ui->tableWidget->setCellWidget(i,0,tableItem);

                   for(int i = 1; i < solution.best_path->length; i++) // mod bonus vector
                       solution.bonus[solution.best_path->get(i)->x + 1][solution.best_path->get(i)->y + 1] += 1;

      /********* Road Graphics  ***********/

                   QGraphicsScene *temp = new QGraphicsScene;
                   this->road_scene = temp;

                   int xmax = city.grid_dim.x;
                   int ymax = city.grid_dim.y;
                   int colormax = city.gen;      // predict that the highest weight will be the number of generation
                   road_scene->setSceneRect(0, 0, xmax*50, ymax*50);
                   road_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

                   for(int x=0; x<xmax; x++){
                       for(int y=0; y<ymax; y++){
                           Road *road = new Road;
                           road->setPos(x*50,y*50);

                           int shade = solution.bonus[x+1][y+1];  // amount to change shade by
                           if(shade>colormax)
                               shade = colormax;                  // in case weight goes over predicted colormax
                           road->color.setHsv(0, 0, 255-(shade*(255/colormax)));

                           if((x==city.loc_start.x && y==city.loc_start.y) ||
                              (x==city.loc_end.x && y==city.loc_end.y))
                               road->color = Qt::green;    // color start and end roads green

                           road_scene->addItem(road);      // add roads to road_scene

                       } // end road y brace
                   }//end road x brace
                   ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
                   ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
                   ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
                   ui->graphicsView->resize(xmax*50+2,ymax*50+2);
                   ui->tabWidget->setMinimumSize(xmax*50+10,ymax*50+40);
                   ui->tabWidget->setMaximumSize(xmax*50+10,ymax*50+40);
                   ui->graphicsView->setScene(road_scene);
                   QApplication::processEvents();  // END ROAD GRAPHICS
                   city.changeLights();
               } // end gen loop
    } // end angle brace

/********* Move Weight ***********/
    if(ui->comboBox->currentIndex()==1){

        QStringList list;
        list <<"Time";
        QTableWidget *widget = ui->tableWidget;
        widget->setRowCount(city.gen);
        widget->setColumnCount(1);
        widget->setHorizontalHeaderLabels(list);

        srand( (unsigned int)time(NULL) ); // seed rand() with runtime
        set<Path>::iterator it;
        city.setLights(); // set lights

    /********* Running Trials/Generations ***********/

              MoveWeight solution = MoveWeight(&city, 90); // load city into algorithm

              for (int i = 0; i < city.gen; i++) {   // for loop mimics runGen()

                   ui->lcdNumber->setSegmentStyle(QLCDNumber::Filled);
                   ui->lcdNumber->display(i+1);
                   for(solution.curr_trial = 0; solution.curr_trial < city.trials; solution.curr_trial++) { // trial loop
                       Path* path = solution.runTrial(); // run trial, find solution
                       solution.top_gen.insert(path); // store in set


  /********* Graphics for StopLights ***********/

                       QGraphicsScene *lighttemp = new QGraphicsScene;
                       this->light_scene = lighttemp;
                       int xmax = city.grid_dim.x;
                       int ymax = city.grid_dim.y;
                       light_scene->setSceneRect(0, 0, xmax*50, ymax*50);
                       light_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

                       for(int x=0; x<xmax; x++){
                           for(int y=0; y<ymax; y++){
                               Light *stoplight = new Light;
                               stoplight->setPos(x*50,y*50);

                               QColor light;
                               if(city.grid[x][y] > 0){
                                   light = Qt::red;   }          // red light
                               else if(city.grid[x][y] < 0){
                                   light = Qt::green;      }      // green light
                               else{ light.setHsv(0, 0, 255, 0);}  // clear = no light

                               stoplight->color = light;
                               light_scene->addItem(stoplight);
                           } // end light y brace
                       } // end light x brace

                       ui->graphicsView2->setCacheMode(QGraphicsView::CacheBackground);
                       ui->graphicsView2->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
                       ui->graphicsView2->setDragMode(QGraphicsView::ScrollHandDrag);
                       ui->graphicsView2->resize(xmax*50+2,ymax*50+2);
                       ui->tabWidget->setMinimumSize(xmax*50+10,ymax*50+40);
                       ui->tabWidget->setMaximumSize(xmax*50+10,ymax*50+40);
                       ui->graphicsView2->setScene(light_scene);
                       QApplication::processEvents();
                     // END LIGHT GRAPHICS

                   } // trial loop

                   solution.best_path = *(solution.top_gen.begin()); // pick best one
                   solution.top_all.insert(*(solution.top_gen.begin()));
                   solution.record.push_back((*solution.top_gen.begin())->time);

                   QLabel *tableItem = new QLabel();
                   tableItem->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                   tableItem->setNum(solution.best_path->time);
                   ui->tableWidget->setCellWidget(i,0,tableItem);

                   for(int i = 1; i < solution.best_path->length; i++) // mod bonus vector
                       solution.bonus[solution.best_path->get(i)->x + 1][solution.best_path->get(i)->y + 1] += 1;

      /********* Road Graphics  ***********/

                   QGraphicsScene *temp = new QGraphicsScene;
                   this->road_scene = temp;

                   int xmax = city.grid_dim.x;
                   int ymax = city.grid_dim.y;
                   int colormax = city.gen;      // predict that the highest weight will be the number of generations
                   road_scene->setSceneRect(0, 0, xmax*50, ymax*50);
                   road_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

                   for(int x=0; x<xmax; x++){
                       for(int y=0; y<ymax; y++){
                           Road *road = new Road;
                           road->setPos(x*50,y*50);

                           int shade = solution.bonus[x+1][y+1];  // amount to change shade by
                           if(shade>colormax)
                               shade = colormax;                  // in case weight goes over predicted colormax
                           road->color.setHsv(0, 0, 255-(shade*(255/colormax)));

                           if((x==city.loc_start.x && y==city.loc_start.y) ||
                              (x==city.loc_end.x && y==city.loc_end.y))
                               road->color = Qt::green;    // color start and end roads green

                           road_scene->addItem(road);      // add roads to road_scene

                       } // end road y brace
                   }//end road x brace
                   ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
                   ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
                   ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
                   ui->graphicsView->resize(xmax*50+2,ymax*50+2);
                   ui->tabWidget->setMinimumSize(xmax*50+10,ymax*50+40);
                   ui->tabWidget->setMaximumSize(xmax*50+10,ymax*50+40);
                   ui->graphicsView->setScene(road_scene);
                   QApplication::processEvents();  // END ROAD GRAPHICS
                   city.changeLights();
               } // end weight loop
    } // end angle brace


/********* Move Rand ***********/
        if(ui->comboBox->currentIndex()==2){

            QStringList list;
            list <<"Time";
            QTableWidget *widget = ui->tableWidget;
            widget->setRowCount(city.gen);
            widget->setColumnCount(1);
            widget->setHorizontalHeaderLabels(list);

            srand( (unsigned int)time(NULL) ); // seed rand() with runtime
            set<Path>::iterator it;
            city.setLights(); // set lights


        /********* Running Trials/Generations ***********/

                  MoveRand solution = MoveRand(&city); // load city into algorithm

                  for (int i = 0; i < city.gen; i++) {   // for loop mimics runGen()

                       ui->lcdNumber->setSegmentStyle(QLCDNumber::Filled);
                       ui->lcdNumber->display(i+1);
                       for(solution.curr_trial = 0; solution.curr_trial < city.trials; solution.curr_trial++) { // trial loop
                           Path* path = solution.runTrial(); // run trial, find solution
                           solution.top_gen.insert(path); // store in set


      /********* Graphics for StopLights ***********/

                           QGraphicsScene *lighttemp = new QGraphicsScene;
                           this->light_scene = lighttemp;
                           int xmax = city.grid_dim.x;
                           int ymax = city.grid_dim.y;
                           light_scene->setSceneRect(0, 0, xmax*50, ymax*50);
                           light_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

                           for(int x=0; x<xmax; x++){
                               for(int y=0; y<ymax; y++){
                                   Light *stoplight = new Light;
                                   stoplight->setPos(x*50,y*50);

                                   QColor light;
                                   if(city.grid[x][y] > 0){
                                       light = Qt::red;   }          // red light
                                   else if(city.grid[x][y] < 0){
                                       light = Qt::green;      }      // green light
                                   else{ light.setHsv(0, 0, 255, 0);}  // clear = no light

                                   stoplight->color = light;
                                   light_scene->addItem(stoplight);
                               } // end light y brace
                           } // end light x brace

                           ui->graphicsView2->setCacheMode(QGraphicsView::CacheBackground);
                           ui->graphicsView2->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
                           ui->graphicsView2->setDragMode(QGraphicsView::ScrollHandDrag);
                           ui->graphicsView2->resize(xmax*50+2,ymax*50+2);
                           ui->tabWidget->setMinimumSize(xmax*50+10,ymax*50+40);
                           ui->tabWidget->setMaximumSize(xmax*50+10,ymax*50+40);
                           ui->graphicsView2->setScene(light_scene);
                           QApplication::processEvents();
                         // END LIGHT GRAPHICS

                       } // trial loop

                       solution.best_path = *(solution.top_gen.begin()); // pick best one
                       solution.top_all.insert(*(solution.top_gen.begin()));
                       solution.record.push_back((*solution.top_gen.begin())->time);

                       QLabel *tableItem = new QLabel();
                       tableItem->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
                       tableItem->setNum(solution.best_path->time);
                       ui->tableWidget->setCellWidget(i,0,tableItem);

                       for(int i = 1; i < solution.best_path->length; i++) // mod bonus vector
                           solution.bonus[solution.best_path->get(i)->x + 1][solution.best_path->get(i)->y + 1] += 1;

          /********* Road Graphics  ***********/

                       QGraphicsScene *temp = new QGraphicsScene;
                       this->road_scene = temp;

                       int xmax = city.grid_dim.x;
                       int ymax = city.grid_dim.y;
                       int colormax = city.gen;      // predict that the highest weight will be the number of generations
                       road_scene->setSceneRect(0, 0, xmax*50, ymax*50);
                       road_scene->setItemIndexMethod(QGraphicsScene::NoIndex);

                       for(int x=0; x<xmax; x++){
                           for(int y=0; y<ymax; y++){
                               Road *road = new Road;
                               road->setPos(x*50,y*50);

                               int shade = solution.bonus[x+1][y+1];  // amount to change shade by
                               if(shade>colormax)
                                   shade = colormax;                  // in case weight goes over predicted colormax
                               road->color.setHsv(0, 0, 255-(shade*(255/colormax)));

                               if((x==city.loc_start.x && y==city.loc_start.y) ||
                                  (x==city.loc_end.x && y==city.loc_end.y))
                                   road->color = Qt::green;    // color start and end roads green

                               road_scene->addItem(road);      // add roads to road_scene

                           } // end road y brace
                       }//end road x brace
                       ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
                       ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
                       ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
                       ui->graphicsView->resize(xmax*50+2,ymax*50+2);
                       ui->tabWidget->setMinimumSize(xmax*50+10,ymax*50+40);
                       ui->tabWidget->setMaximumSize(xmax*50+10,ymax*50+40);
                       ui->graphicsView->setScene(road_scene);
                       QApplication::processEvents();  // END ROAD GRAPHICS
                       city.changeLights();
                   } // end gen loop
        } // end rand brace


//               solution.printPath(solution.best_path);

} // end startClicked



