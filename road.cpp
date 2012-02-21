#include "road.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QColor>

Road::Road(){

    color.setHsv(0, 0, 255);
    // H -> Hue = 0 (value doesnt matter)
    // S -> Saturation = 0
    // V -> Value edit this !
    //   -> 0 = black , 255 = white
}

void Road::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    // Block
    painter->setBrush(color);
    //Draws a rectangle with upper left corner at (x, y) and with the given width and height.
    painter->drawRect(0, 0, 50, 50);
}


QRectF Road::boundingRect() const{
    return QRectF();
}



