#include "navypainter.h"
#include <QPainter>
#include <QTimer>

#include <QtGlobal>
#include <QMouseEvent>
#include <QMessageBox>

#include "randomlocation.h"
#include <time.h>

#include "noise.h"

#include <jpeglib.h>
#include <jerror.h>
#include <stdio.h>
#include <math.h>

#include <GL/glu.h>

int NavyPainter::getBgCode() const
{
    return bgcode;
}


NavyPainter::NavyPainter(QWidget *parent, AutoLocation * location, int bgm) :
    QGLWidget(parent), Navy(location)
{
    setAutoFillBackground(false);

    if(bgm >= 3 || bgm < 0)
        bgm = 0;

    bgcode = bgm;

    t=0;

    backgrounds.push_back(QImage(":/Skin1.png"));
    backgrounds.push_back(QImage(":/Skin2.png"));
    backgrounds.push_back(QImage(":/Island3min.png"));

    bg= backgrounds[bgcode];

    tt.load(":/reflection.jpg");
    rr.load(":/boom.png");
    mbg.load(":/images/background.png");

    resize(m_w,m_h);
    setMinimumSize(m_w,m_h);
    setMaximumSize(m_w,m_h);

    splash.load(":/images/splash.png");
    boom.load(":/images/boom.png");
    death.load(":/images/death.png");

    timer = new QTimer(this);
    timer->setInterval(25);

    timer_fire = new QTimer(this);
    //timer_fire->setInterval(100);

    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    //connect(timer_fire, SIGNAL(timeout()), this, SLOT(fire()));

    qsrand(::time(NULL));
    timer->start();

    show_shell = false;

    setMouseTracking(true);

    enemy = new Navy();
    cur_sursor = false;

    InitNoise();

    wire_frame = normals = xold = yold = 0;

    rotate_y = 35;
    rotate_x = 10;

    translate_z=2.5;

    time = 0;

    int flags = QGLFormat::openGLVersionFlags();
    useOpenGl = (flags & QGLFormat::OpenGL_Version_2_0);

    if(!useOpenGl)
        timer->stop();
}

void NavyPainter::changed_bg(int q)
{
    bg= backgrounds[q];
    bgcode = q;
    if(!useOpenGl)
        update();
}

void NavyPainter::drawGrid(QPainter & painter)
{
    painter.save();

    painter.translate(50,85);

    QPen pen(Qt::DashLine);
    pen.setColor(Qt::white);
    painter.setPen(pen);

    const double w = m_w/2.5;
    const double s = w/10;

    QString alpha = tr("АБВГДЕЖЗИК ");

    for(int i=0; i<= 10; i++)
    {
        painter.drawLine(i* s, 0, i * s , w);
        painter.drawText(i* s + 10, -10, QString(alpha[i]));
    }
    for(int i=0; i<= 10; i++)
    {
        painter.drawLine(0, i* s, w, i * s );
        if(i<10)
        painter.drawText(-15, i*s + 20, QString::number(i+1));
    }

    painter.translate(300,0);

    for(int i=0; i<= 10; i++)
    {
        painter.drawLine(i* s, 0, i * s , w);
        painter.drawText(i* s + 10, -10, QString(alpha[i]));
    }
    for(int i=0; i<= 10; i++)
    {
        painter.drawLine(0, i* s, w, i * s );
        if(i<10)
        painter.drawText(-15, i*s + 20, QString::number(i+1));
    }
    painter.restore();
}


void NavyPainter::setLocation(AutoLocation * a)
{
    m_location = a;
}

void NavyPainter::change(int x, int y)
{

}

void NavyPainter::shoot_my(int x, int y)
{

    whose = false;

    m_x = x;
    m_y = y;

    fire();
}

void NavyPainter::shoot_enemy(int x, int y)
{
    whose = true;

    m_x = x;
    m_y = y;

    fire();
}

void NavyPainter::upd()
{
    update();
}

void NavyPainter::paintEvent(QPaintEvent * ev)
{
    QPainter painter(this);

    if(useOpenGl)
    {

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glEnable (GL_TEXTURE_GEN_S);
    glEnable (GL_TEXTURE_GEN_T);


       glShadeModel(GL_SMOOTH);
       glEnable(GL_DEPTH_TEST);
       glEnable(GL_CULL_FACE);
       glEnable(GL_LIGHTING);
       glEnable(GL_LIGHT0);

    paint();

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glMatrixMode(GL_MODELVIEW);

    glDisable (GL_TEXTURE_GEN_S);
    glDisable (GL_TEXTURE_GEN_T);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glFlush();
}
    else
    {
        //рисуем просто картинку
        painter.drawImage(0,0,mbg);
    }


    painter.drawImage(0,0, bg);

    drawGrid(painter);

    const double w = m_w/2.5;
    const double s = w/10;

    //корабли

    painter.save();
    painter.translate(51,86);

    for(int i =0; i< ships.size(); ++i)
    {
        Ship ship = ships[i];
        ship.paint(&painter, s);
    }

    painter.restore();

    painter.translate(2*m_w/2.5/10, 3*m_w/2.5/10+6);
    for(int i=0; i< N; ++i)
    {
        for(int j=0; j< N; ++j)
        {
            double x = m_w - of - m_w / 2.5  +  i * m_w / 2.5 /10;
            double y = s/2 + of +  j * width() / 2.5 /10 ;

            QPointF t1(x+s/2-115,y+s/2-25);
            enum lState d = enemy->getState(i,j);
            switch(d)
            {
                case EMPTY:
                    painter.drawImage(t1,splash);
                    break;
                case KILLED:
                    painter.drawImage(t1,death);
                    break;
                case SHOT:
                    painter.drawImage(t1,boom);
                    break;
            }

            x = s/2 + of +  i * width() / 2.5 /10 ;


            QPointF t(x - s/2+2,y -s/2+2);

            d = getState(i,j);
            switch(d)
            {
                case EMPTY:
                    painter.drawImage(t,splash);
                    break;
                case KILLED:
                    painter.drawImage(t,death);
                    break;
                case SHOT:
                    //painter.setBrush(QBrush(QColor(Qt::yellow)));
                    painter.drawImage(t,boom);
                    break;
            }
        }
    }

    if(useOpenGl)
    {
        glBindTexture (GL_TEXTURE_2D, texture);
        bindTexture(tt);
    }

    swapBuffers();
}

void NavyPainter::switch_cursor(bool o)
{
    cur_sursor = o;
}

void NavyPainter::mouseMoveEvent(QMouseEvent * evt)
{
    int x = evt->x();
    int y = evt->y();

    int e_x = 350;

    if(cur_sursor && x > e_x && x < e_x + m_w / 2.5  && y > 85 && y < m_w / 2.5 + 85)
        setCursor(QCursor(QPixmap(":/images/cursor.png")));
    else
        setCursor(QCursor(Qt::ArrowCursor));

}

void NavyPainter::mousePressEvent(QMouseEvent * evt)
{
    if(!cur_sursor)
        return;

    int x = evt->x();
    int y = evt->y();

    int e_x = 350;

    if(x > e_x && x < e_x + m_w / 2.5  && y > 85 && y < m_w / 2.5 + 85)
    {
        x -= e_x;
        y -= 85;

        //вражеское поле
        int step = width() / 2.5 / 10;

        int m_x = x / step;
        int m_y = y / step;

        if(m_x < 10) m_x++;
        if(m_y < 10) m_y++;

        switch_cursor(false);
        shoot_enemy(m_x, m_y);

    }

}

void NavyPainter::animate()
{
    t++;

    if(m_s>=9) m_s=0;
    else m_s++;

    time+=13;
    update();

}

void NavyPainter::fire()
{
    timer_fire->stop();
    show_shell = false;
    if(whose)
        emit enemy_clicked(m_x-1, m_y-1);
    else
        emit my_clicked(m_x-1, m_y -1);
    update();
    return;

    ///
    const int dx = 10;

    int direction = target.x() > shot.x() ? 1 : -1;

    if( abs(target.x() -shot.x()) <= dx || abs(target.y() - shot.y()) <= dx)
        target=shot;
    else
    {
        if(shot.x() == target.x())
            target.setY( target.y() -dx*direction );
        else if(shot.y() == target.y())
            target.setX (target.x() - dx*direction);
        else
        {
            target.setX ( target.x() - dx*direction );
            target.setY ( a * target.x() + b );
        }
    }

    update();

    if(target == shot)
    {
        timer_fire->stop();
        show_shell = false;
        if(whose)
            emit enemy_clicked(m_x-1, m_y-1);
        else
            emit my_clicked(m_x-1, m_y -1);
        update();
    }
}

void NavyPainter::initializeGL()
{
    setAutoBufferSwap(false);

    glClearColor (0, 0, 0, 0);

    glGenTextures (1, &texture);
      if (load_texture ("alpha.jpg", alpha_texture, GL_ALPHA, 256) != 0 ||
          load_texture ("reflection.jpg", caustic_texture, GL_RGB, 256) != 0)
        return;

      for (int i = 0; i < 256 * 256; i++)
        {
          total_texture[4 * i] = caustic_texture[3 * i];
          total_texture[4 * i + 1] = caustic_texture[3 * i + 1];
          total_texture[4 * i + 2] = caustic_texture[3 * i + 2];
          total_texture[4 * i + 3] = alpha_texture[i];
        }

      glBindTexture (GL_TEXTURE_2D, texture);
      gluBuild2DMipmaps (GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_RGBA,
                         GL_UNSIGNED_BYTE,  total_texture);

      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

      glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
      glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

}

void NavyPainter::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity ();
    gluPerspective (20, width / (float) height, 0.1, 15);
    glViewport (0, 0, width, height);

    glMatrixMode(GL_MODELVIEW);
}

void NavyPainter::paint()
{


    const float t = time / 1000.;
    const float delta = 2. / RESOLUTION;
    const unsigned int length = 2 * (RESOLUTION + 1);
    const float xn = (RESOLUTION + 1) * delta + 1;
    unsigned int i;
    unsigned int j;
    float x;
    float y;
    unsigned int indice;
    unsigned int preindice;

    /* Yes, I know, this is quite ugly... */
    float v1x;
    float v1y;
    float v1z;

    float v2x;
    float v2y;
    float v2z;

    float v3x;
    float v3y;
    float v3z;

    float vax;
    float vay;
    float vaz;

    float vbx;
    float vby;
    float vbz;

    float nx;
    float ny;
    float nz;

    float l;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glLoadIdentity ();
    glTranslatef (0, 0, -translate_z);
    glRotatef (rotate_y, 1, 0, 0);
    glRotatef (rotate_x, 0, 1, 0);

    /* Vertices */
    for (j = 0; j < RESOLUTION; j++)
      {
        y = (j + 1) * delta - 1;
        for (i = 0; i <= RESOLUTION; i++)
          {
            indice = 6 * (i + j * (RESOLUTION + 1));

            x = i * delta - 1;
            surface[indice + 3] = x;
            surface[indice + 4] = z (x, y, t);
            surface[indice + 5] = y;
            if (j != 0)
              {
                /* Values were computed during the previous loop */
                preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
                surface[indice] = surface[preindice + 3];
                surface[indice + 1] = surface[preindice + 4];
                surface[indice + 2] = surface[preindice + 5];
              }
            else
              {
                surface[indice] = x;
                surface[indice + 1] = z (x, -1, t);
                surface[indice + 2] = -1;
              }
          }
      }

    /* Normals */
    for (j = 0; j < RESOLUTION; j++)
      for (i = 0; i <= RESOLUTION; i++)
        {
          indice = 6 * (i + j * (RESOLUTION + 1));

          v1x = surface[indice + 3];
          v1y = surface[indice + 4];
          v1z = surface[indice + 5];

          v2x = v1x;
          v2y = surface[indice + 1];
          v2z = surface[indice + 2];

          if (i < RESOLUTION)
            {
              v3x = surface[indice + 9];
              v3y = surface[indice + 10];
              v3z = v1z;
            }
          else
            {
              v3x = xn;
              v3y = z (xn, v1z, t);
              v3z = v1z;
            }

          vax =  v2x - v1x;
          vay =  v2y - v1y;
          vaz =  v2z - v1z;

          vbx = v3x - v1x;
          vby = v3y - v1y;
          vbz = v3z - v1z;

          nx = (vby * vaz) - (vbz * vay);
          ny = (vbz * vax) - (vbx * vaz);
          nz = (vbx * vay) - (vby * vax);

          l = sqrtf (nx * nx + ny * ny + nz * nz);
          if (l != 0)
            {
              l = 1 / l;
              normal[indice + 3] = nx * l;
              normal[indice + 4] = ny * l;
              normal[indice + 5] = nz * l;
            }
          else
            {
              normal[indice + 3] = 0;
              normal[indice + 4] = 1;
              normal[indice + 5] = 0;
            }


          if (j != 0)
            {
              /* Values were computed during the previous loop */
              preindice = 6 * (i + (j - 1) * (RESOLUTION + 1));
              normal[indice] = normal[preindice + 3];
              normal[indice + 1] = normal[preindice + 4];
              normal[indice + 2] = normal[preindice + 5];
            }
          else
            {
  /* 	    v1x = v1x; */
              v1y = z (v1x, (j - 1) * delta - 1, t);
              v1z = (j - 1) * delta - 1;

  /* 	    v3x = v3x; */
              v3y = z (v3x, v2z, t);
              v3z = v2z;

              vax = v1x - v2x;
              vay = v1y - v2y;
              vaz = v1z - v2z;

              vbx = v3x - v2x;
              vby = v3y - v2y;
              vbz = v3z - v2z;

              nx = (vby * vaz) - (vbz * vay);
              ny = (vbz * vax) - (vbx * vaz);
              nz = (vbx * vay) - (vby * vax);

              l = sqrtf (nx * nx + ny * ny + nz * nz);
              if (l != 0)
                {
                  l = 1 / l;
                  normal[indice] = nx * l;
                  normal[indice + 1] = ny * l;
                  normal[indice + 2] = nz * l;
                }
              else
                {
                  normal[indice] = 0;
                  normal[indice + 1] = 1;
                  normal[indice + 2] = 0;
                }
            }
        }

    glTranslatef (0, 0.2, 0);

    /* Render wireframe? */
    if (wire_frame != 0)
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

    /* The water */
    glEnable (GL_TEXTURE_2D);
    glColor3f (1, 1, 1);

    glEnableClientState (GL_NORMAL_ARRAY);
    glEnableClientState (GL_VERTEX_ARRAY);
    glNormalPointer (GL_FLOAT, 0, normal);
    glVertexPointer (3, GL_FLOAT, 0, surface);

    for (i = 0; i < RESOLUTION; i++)
      glDrawArrays (GL_TRIANGLE_STRIP, i * length, length);

    glDisable (GL_TEXTURE_2D);
    /* End */

    glFlush ();
}

float	NavyPainter::z (const float x, const float y, const float t)
{
  const float x2 = x - 3;
  const float y2 = y + 1;
  const float xx = x2 * x2;
  const float yy = y2 * y2;
  return ((2 * sinf (20 * sqrtf (xx + yy) - 4 * t) +
           Noise (10 * x, 10 * y, t, 0)) / 200);
}



int NavyPainter::load_texture (const char * filename,
                              unsigned char * dest,
                              const int format,
                              const unsigned int size)
{
  FILE *fd;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  unsigned char * line;

  cinfo.err = jpeg_std_error (&jerr);
  jpeg_create_decompress (&cinfo);

  if (0 == (fd = fopen(filename, "rb")))
    return 1;

  jpeg_stdio_src (&cinfo, fd);
  jpeg_read_header (&cinfo, TRUE);
  if ((cinfo.image_width != size) || (cinfo.image_height != size))
    return 1;

  if (GL_RGB == format)
    {
      if (cinfo.out_color_space == JCS_GRAYSCALE)
        return 1;
    }
  else
    if (cinfo.out_color_space != JCS_GRAYSCALE)
      return 1;

  jpeg_start_decompress (&cinfo);

  while (cinfo.output_scanline < cinfo.output_height)
    {
      line = dest +
        (GL_RGB == format ? 3 * size : size) * cinfo.output_scanline;
      jpeg_read_scanlines (&cinfo, &line, 1);
    }
  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);

  return 0;
}

