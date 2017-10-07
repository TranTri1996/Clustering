#ifndef ARROW_H
#define ARROW_H
#include <QObject>
#include <QGraphicsLineItem>

#include <QPointF>
#endif // ARROW_H

class arrow: public QObject, public QGraphicsLineItem{
    Q_OBJECT
public:
    arrow(QPointF a, QPointF b);

};
