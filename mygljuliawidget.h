#ifndef MYGLJULIAWIDGET_H
#define MYGLJULIAWIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
#include <QOpenGLShaderProgram>
#include <QVector>

#include "mandelbrotoptions.h"

class MyGLJuliaWidget : public QOpenGLWidget, QOpenGLFunctions_4_0_Core
{
    Q_OBJECT
public:
    explicit MyGLJuliaWidget(QWidget *parent = 0);
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
public slots:
    void juliaPos(float,float);
    void setIteration(int);
private:

    GLuint m_posAttr;
    GLuint m_fracCoordAttr;
    GLuint m_matrixUniform;
    GLuint m_jul;
    GLuint m_iterations;
    QVector2D jul;
    QOpenGLShaderProgram *m_program;
    enum Texture_IDs { hsvTex, numTextures };
    GLuint textures[numTextures];
    int m_frame;
    float posx=0.0;
    float posy=0.0;
    float fScale=2.0;
    bool bMoving = false;
    int mouseX=0;
    int mouseY=0;
};

#endif // MYGLJULIAWIDGET_H
