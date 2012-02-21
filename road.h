#ifndef ROAD_H
#define ROAD_H

#include <QGraphicsItem>

namespace Ui {
    class Road;
}

class Road : public QGraphicsItem
{
public:
    Road();
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
//    inline void drawText(int x, int y, const QString &s);
    QColor color;
};


#endif // ROAD_H
