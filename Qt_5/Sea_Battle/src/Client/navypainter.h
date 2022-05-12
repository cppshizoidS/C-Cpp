#ifndef NAVYPAINTER_H
#define NAVYPAINTER_H

#include <QWidget>
#include <navy.h>

#include <QGLWidget>

#undef ERROR
#undef NEAR

class QPaintEvent;
class QMouseEvent;

class NavyPainter : public QGLWidget, public Navy
{
    Q_OBJECT
public:
    explicit NavyPainter(QWidget *parent = 0, AutoLocation * location =0, int bg = 0);
    void setLocation(AutoLocation *);
    void shoot_my(int x, int y);

    Navy * enemy;

    void switch_cursor(bool);

    void upd();
    int getBgCode() const;

signals:

    void enemy_clicked(int x,int y);
    void my_clicked(int x,int y);

public slots:
    void changed_bg(int);

protected:

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void change(int, int);

private slots:

    void animate();
    void fire();

private:
    QPixmap rp;
    QImage bgg;
    QImage splash, boom, death;

    QTimer * timer;
    QTimer * timer_fire;

    QVector<QImage> backgrounds;

    QPointF shot; //текущие координаты ядра
    QPointF target; //координаты цели
    QPointF start;

    bool show_shell;

    qreal a, b;
    bool whose;

    bool cur_sursor;

    static const int of = 0;

    int m_x;
    int m_y;

    void shoot_enemy(int x, int y);

    int t;

    QVector<QImage> img;
    int m_s;

    QImage bg; //background

    QImage tt; //to be deleted!
    QImage rr;
    QImage mbg;

    bool useOpenGl;

    int bgcode;
private:

    void initializeGL();
    void resizeGL(int w, int h);
    void paint(); //вместо paintGL
    void drawGrid(QPainter &);

private:

    unsigned char total_texture[4 * 256 * 256];
    unsigned char alpha_texture[256 * 256];
    unsigned char caustic_texture[3 * 256 * 256];

    GLuint texture;

    int	wire_frame;
    int	normals;
    int	xold;
    int	yold;

    float rotate_x;
    float rotate_y;
    float translate_z;

    static const int RESOLUTION = 128;
    static const int m_w = 670;
    static const int m_h = 440;

    float surface[6 * RESOLUTION * (RESOLUTION + 1)];
    float normal[6 * RESOLUTION * (RESOLUTION + 1)];

    float z (const float x, const float y, const float t);
    int load_texture (const char * filename,
                                  unsigned char * dest,
                                  const int format,
                                  const unsigned int size);
    int time;

};

#endif // NAVYPAINTER_H
