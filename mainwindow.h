#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <complex>
#include <QMessageBox>
#include "basicsymbolic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void draw();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QImage *image;
};

#endif // MAINWINDOW_H
