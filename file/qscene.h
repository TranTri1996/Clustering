#ifndef QSCENE_H
#define QSCENE_H
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QPointF>
#include <QVector>
#include <QGraphicsEllipseItem>
#include <QColor>
class qscene: public QGraphicsScene{
public:
    explicit qscene(QGraphicsScene *parent = 0);
    QVector<QGraphicsEllipseItem *> eclipinst;
    QVector <QPointF> pointct;

    QVector <QPointF> pointinst;
    QVector <QGraphicsRectItem *> rectct;
    QVector<QVector<QPointF> > Clustering;
    QVector<QColor> color;
    void OxArrow(int x,int y);
    void OyArrow(int x,int y);
    int kt=0;
    QVector<QVector<int> > index;
    void delay(int time);
    double distances(QPointF a, QPointF b);
    void kmeans();
    void random(int k);
    int Width=960*3;
    int Height=560*3;
    int dv=20;
    int dkht=12; // duong kinh cham tron
    int chv =18; //canh hinh vuong
    int ctg=20; // canh cua tam giac diem
    int test_mouse=0;
    void descartes();
    void centroid_run(int i,QPointF b);
    QPoint moving(QPointF p, double kc, int huong);
    void appear_file_txt(QVector<QPointF> train);
    void appear_file_inp(QVector<QPointF> test,QVector<int> group);
    void Knn(double x, double y,QVector<QPointF> test,QVector<int> group);
    void savefileKmean(QString filename);
    QGraphicsEllipseItem *cicle;
signals:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
public slots:
    private:

};

#endif // QSCENE_H
