#include "arrow.h"
#include <QPolygon>

#include <QPointF>

arrow::arrow(QPointF a,QPointF b)
{
    setLine(a.x(),a.y(),b.x(),b.y());
}

