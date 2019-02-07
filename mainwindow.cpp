#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    image = new QImage(480, 480, QImage::Format_RGB32);
    ui->graphicsView->setScene(scene);
//    scene->setSceneRect(0,0,480,480);
    ui->graphicsView->setFrameStyle(0);

    ui->graphicsView->setFixedSize(480,480);
    ui->graphicsView->setSceneRect(0,0,480,480);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete image;
    delete ui;
}

std::complex<float> Function (std::complex<float> z)
{
    return pow(z, 5) - std::complex<float>(1, 0);
}

std::complex<float> Derivative (std::complex<float> z)
{
    return std::complex<float>(5,0)*z*z*z*z;
}

void MainWindow::draw()
{
    QColor colors[11] =
    {
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::yellow,
        Qt::magenta,
        Qt::green,
        Qt::cyan,
        Qt::gray,
        Qt::darkYellow,
        Qt::darkBlue,
        Qt::black
    };

    int maxIteration = ui->lineEdit->text().toInt();
    if(maxIteration==0) maxIteration = 50; //if lineedit contain letter

    BasicSymbolic sym(ui->textEdit->toPlainText());
    sym.createFunc();

    std::vector<std::complex<float>> v;

    for(int i=0; i<480; i++)
    {
        float ii = 2-i*4/(float)480;
        for(int j=0; j<480; j++)
        {
            float jj = -2+j*4/(float)480;

            std::complex<float> z = std::complex<float>(ii, jj);

            std::complex<float> temp = sym.getFuncValue(z)/sym.getDerivativeValue(z);
            int iteration = 0;
            while((abs(z - temp) > 0.0000001) && (iteration < maxIteration+1))
            {
                temp = z;
                z -= sym.getFuncValue(z)/sym.getDerivativeValue(z);
                iteration++;
            }

            int root_index = -1;
            for(unsigned int index=0; index<v.size(); index++)
            {
                if(abs(v[index]-z)<0.0000001)
                {
                    root_index = index;
                    break;
                }
            }

            if ((z.real() != z.real()) || (iteration>maxIteration)) // if z.real() is nan
                root_index = 10;

            if(root_index==-1)
                v.push_back(z);

            image->setPixelColor(i,j, colors[root_index]);


        }
    }
    scene->addPixmap(QPixmap::fromImage(*image));

    // notify if all roots did not find correctly
    if(v.size()>sym.degree())
    {
        QMessageBox msgBox;
        msgBox.setText("Didn't find all roots correctly, try increase iteration");
        msgBox.exec();
    }
}

void MainWindow::on_pushButton_clicked()
{
    draw();
}
