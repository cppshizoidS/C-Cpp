#include "PaintScene.h"

PaintScene::PaintScene(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setFocusPolicy(Qt::StrongFocus);
	draw = false;
}

void PaintScene::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	QPalette Pal(palette());
	Pal.setColor(QPalette::Background, Qt::black);
	setAutoFillBackground(true);
	setPalette(Pal);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(Qt::green, 10, Qt::SolidLine));
	for (int i = 0; i < vv.size(); ++i)
		if (i > 0) painter.drawEllipse(vv[i - 1], 12, 12);
}
void PaintScene::mousePressEvent(QMouseEvent* pe) {
	draw = true;
}
void PaintScene::mouseMoveEvent(QMouseEvent* pe) {
	if (draw) {
		vv.push_back(pe->pos());
	}
	update();
}
void PaintScene::mouseReleaseEvent(QMouseEvent* pe) {
	draw = false;
	QPixmap scr;
	scr = QPixmap::grabWidget(this);

	QFile mFile("test.txt");
	mFile.open(QIODevice::WriteOnly);
	QTextStream fin(&mFile);
	fin.setRealNumberPrecision(2);

	QImage img;
	img = scr.toImage();
	img = img.scaled(28, 28);
	int w = img.size().width();
	int h = img.size().height();
	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			fin << img.pixelColor(j, i).green() / 255. << " ";
		}
		fin << endl;
	}
	mFile.close();
}
void PaintScene::clear() {
	vv.clear();
	update();
}
PaintScene::~PaintScene()
{
}
