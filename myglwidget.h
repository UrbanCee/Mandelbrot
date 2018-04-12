#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLShaderProgram>
#include <QVector>

#include "mandelbrotoptions.h"


class MyGLWidget : public QOpenGLWidget, QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int, int);
    void wheelEvent(QWheelEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void showStatusBarMessage(const QString &, int);
    void sendJuliaPos(float,float);
public slots:
    void setIterations(int);
    void setAnimated(bool);
private:

    GLuint m_posAttr;
    GLuint m_fracCoordAttr;
    GLuint m_matrixUniform;
    GLuint m_iterations;
    GLuint m_line_posAttr;
    GLuint m_line_posUniform;
    GLuint m_line_scale;
    QOpenGLShaderProgram *m_program_mandelbrot;
    QOpenGLShaderProgram *m_program_line;
    enum Texture_IDs { hsvTex,greyflashTex, numTextures };
    GLuint textures[numTextures];
    int m_frame;
    float posx=-0.75;
    float posy=0.0;
    float fScale=1.5;
    bool bMoving = false;
    bool bMoveJulia = false;
    int mouseX=0;
    int mouseY=0;
    QVector<GLfloat> vecLine;

    void drawIterationLine();
    void calculateIterationLine(float,float);


};

#endif // MYGLWIDGET_H
