#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWheelEvent>
#include <QMainWindow>
#include <QMouseEvent>
#include <qscene.h>
#include <QGraphicsSceneMouseEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void wheelEvent(QWheelEvent *event);
    qscene * scene;
    int K;
    void interface();
    QVector<QPointF> test;
    QVector<int> group;
    void reset();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private slots:

    void on_btnReset_clicked();
    void on_tableWidget_cellClicked(int row, int column);
    void on_open_kmean_file_clicked();
    void on_open_knn_file_clicked();
    void on_savekmeanfile_clicked();
    void on_Clustering_clicked();
    void on_run_knn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
