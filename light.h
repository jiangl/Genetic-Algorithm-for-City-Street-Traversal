#ifndef LIGHT_H
#define LIGHT_H

#include <QGraphicsItem>

namespace Ui {
    class Light;
}

class Light : public QGraphicsItem
{
public:
    Light();
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
//    inline void drawText(int x, int y, const QString &s);
    QColor color;
};


#endif // ROAD_H
