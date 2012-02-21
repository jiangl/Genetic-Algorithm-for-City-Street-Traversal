#include "light.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QColor>

Light::Light(){

    color.setHsv(0, 0, 255);
    // H -> Hue = 0 (value doesnt matter)
    // S -> Saturation = 0
    // V -> Value edit this !
    //   -> 0 = black , 255 = white
}

void Light::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *){
    painter->setBrush(color);
    //Draws a circle with upper left corner at (x, y) and with the given width and height.
    painter->drawEllipse(10, 10, 25, 25);
}


QRectF Light::boundingRect() const{
    return QRectF();
}



