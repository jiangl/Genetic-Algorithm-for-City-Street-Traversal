#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <string>
#include "road.h"

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    QGraphicsScene *road_scene;
    QGraphicsScene *light_scene;
    Road *pathr;
	void toConsole(string *);

private slots:
    void startClicked();

//private:
//    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
