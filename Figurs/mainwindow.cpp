#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "figureitem.h"
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QSvgGenerator>
#include <QGraphicsView>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{  
        ui->setupUi(this);

        setWindowTitle("Figure");
        setWindowIcon(QIcon(":/images/cylinder.png"));

        toolbar = addToolBar("toolbar");
        toolbar->isRightToLeft();

        QPixmap circlepix(":/images/circle.png");
        QAction* A_createCircle = toolbar->addAction(QIcon(circlepix), "Круг");
        connect(A_createCircle, SIGNAL(triggered()), this, SLOT(slotCreateCircle()));

        QPixmap trianglepix(":/images/triangle.png");
        QAction* A_createTriangle = toolbar->addAction(QIcon(trianglepix), "Треугольник");
        connect(A_createTriangle, SIGNAL(triggered()), this, SLOT(slotCreateTriangle()));

        QPixmap ringpix(":/images/ring.png");
        QAction* A_createRing = toolbar->addAction(QIcon(ringpix), "Кольцо");
        connect(A_createRing, SIGNAL(triggered()), this, SLOT(slotCreateRing()));

        QPixmap delete_pix(":/images/delete.png");
        QAction* A_delete_ = toolbar->addAction(QIcon(delete_pix), "Удалить");
        connect(A_delete_, SIGNAL(triggered()), this, SLOT(slotDelete()));

        QPixmap colorpix(":/images/color.png");
        QAction* A_setColor = toolbar->addAction(QIcon(colorpix), "Выбрать цвет");
        connect(A_setColor, SIGNAL(triggered()), this, SLOT(slotSetColor()));

        toolbar->addSeparator();

        QPixmap savepix(":/images/save.png");
        QAction* save = toolbar->addAction(QIcon(savepix), "Сохранить");
        connect(save, SIGNAL(triggered()), this, SLOT(slotSave()));

        QPixmap quitpix(":/images/exit.png");
        QAction *quit = toolbar->addAction(QIcon(quitpix), "Выйти");
        connect(quit, &QAction::triggered, qApp, &QApplication::quit);

        scene = new QGraphicsScene();
        ui->graphicsView->setScene(scene);
        ui->graphicsView->setSceneRect(ui->graphicsView->rect());
        setCentralWidget(ui->graphicsView);

        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scene->setItemIndexMethod(QGraphicsScene::NoIndex);
        ui->graphicsView->setRenderHint(QPainter::Antialiasing);
        showFullScreen();
}

MainWindow::~MainWindow()
{
    qDeleteAll(scene->items());
    delete ui;
    delete toolbar;
    delete scene;
}

void MainWindow::slotCreateCircle()
{
    bool ok;
    int r = QInputDialog::getInt(this,"Круг", "Радиус", 0, 1, 1000, 1, &ok);
    if(ok)
    {
        CircleItem* circleItem = new CircleItem(Circle(r), QPoint(0, 0), color);
        scene->addItem(circleItem);
    }
}

void MainWindow::slotSetColor()
{
    color = QColorDialog::getColor();
}

void MainWindow::slotSave()
{
    QString newPath = QFileDialog::getSaveFileName(this, "Save SVG", path, "SVG files (*.svg)");
    if (newPath.isEmpty()) return;
    path = newPath;
    QSvgGenerator generator;
    generator.setFileName(path);
    generator.setSize(QSize(ui->graphicsView->width(), ui->graphicsView->height()));
    generator.setViewBox(QRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height()));
    generator.setTitle("SVG");
    generator.setDescription("Figure");
    QPainter painter;
    painter.begin(&generator);
    scene->render(&painter);
    painter.end();
}

void MainWindow::slotCreateTriangle()
{
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle("Треугольник");
    QFormLayout* formLayout = new QFormLayout();
    QSpinBox* X1 = new QSpinBox();
    X1->setMinimum(0);
    X1->setMaximum(1000);
    QSpinBox* Y1 = new QSpinBox();
    Y1->setMinimum(0);
    Y1->setMaximum(1000);
    QSpinBox* X2 = new QSpinBox();
    X2->setMinimum(0);
    X2->setMaximum(1000);
    QSpinBox* Y2 = new QSpinBox();
    Y2->setMinimum(0);
    Y2->setMaximum(1000);
    QSpinBox* X3 = new QSpinBox();
    X3->setMinimum(0);
    X3->setMaximum(1000);
    QSpinBox* Y3 = new QSpinBox();
    Y3->setMinimum(0);
    Y3->setMaximum(1000);
    formLayout->addRow("x1:", X1);
    formLayout->addRow("y1:", Y1);
    formLayout->addRow("x2:", X2);
    formLayout->addRow("y2:", Y2);
    formLayout->addRow("x3:", X3);
    formLayout->addRow("y3:", Y3);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));
    formLayout->addWidget(buttonBox);
    dialog->setLayout(formLayout);
    int result = dialog->exec();
    if(result == QDialog::Accepted)
    {
        try
        {
            TriangleItem* triangleItem = new TriangleItem(Triangle(X1->value(), Y1->value(),
                                                                   X2->value(), Y2->value(),
                                                                   X3->value(), Y3->value()), QPoint(0, 0), color);
            scene->addItem(triangleItem);
        }
        catch(...) {}
    }
    delete X1;
    delete Y1;
    delete X2;
    delete Y2;
    delete X3;
    delete Y3;
    delete formLayout;
    delete buttonBox;
    delete dialog;
}

void MainWindow::slotCreateRing()
{
    QDialog* dialog = new QDialog();
    dialog->setWindowTitle("Кольцо");
    QFormLayout* formLayout = new QFormLayout();
    QSpinBox* r = new QSpinBox();
    r->setMinimum(1);
    r->setMaximum(1000);
    QSpinBox* R = new QSpinBox();
    R->setMinimum(2);
    R->setMaximum(1000);
    formLayout->addRow("Маленький радиус", r);
    formLayout->addRow("Большой радиус", R);
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), dialog, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), dialog, SLOT(reject()));
    formLayout->addWidget(buttonBox);
    dialog->setLayout(formLayout);
    int result = dialog->exec();
    if(result == QDialog::Accepted)
    {
        try
        {
            RingItem* ringItem = new RingItem(Ring(r->value(),R->value()), QPoint(0, 0), color);
            scene->addItem(ringItem);
        }
        catch(...) {}
    }
    delete r;
    delete R;
    delete formLayout;
    delete buttonBox;
    delete dialog;
}

void MainWindow::slotDelete()
{
    qDeleteAll(scene->selectedItems());
}
