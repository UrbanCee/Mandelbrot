#include "mygljuliawidget.h"

#include "myglwidget.h"
#include <QDebug>
#include <QMainWindow>
#include <QScreen>
#include <QWheelEvent>
#include <QMouseEvent>

MyGLJuliaWidget::MyGLJuliaWidget(QWidget *parent) :
    QOpenGLWidget(parent), QOpenGLFunctions_4_0_Core(),
    m_program(0), m_frame(0)
{

}

void MyGLJuliaWidget::initializeGL()
{
    initializeOpenGLFunctions();
    QString versionString1(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    emit(showStatusBarMessage(QString("OpenGL Version: ")+versionString1,10000));

    m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/julia.vert");
        qDebug() << "Vertex Shader: "<< m_program->log();
        m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/julia.frag");
        qDebug() << "Fragment Shader: "<< m_program->log();
        m_program->link();
        qDebug() << "Linking: "<< m_program->log();
        m_posAttr = m_program->attributeLocation("posAttr");
        m_fracCoordAttr = m_program->attributeLocation("fracCoord");
        m_jul = m_program->uniformLocation("jul");
        m_iterations = m_program->uniformLocation("iterations");

        int hsvGranularity=12;
        GLfloat *hsvTexData= new GLfloat[hsvGranularity*3];

        for (int i=0;i<hsvGranularity;i++)
        {
            QColor col;
            col.setHsv((i*360)/hsvGranularity,255,255);
            hsvTexData[i*3]=col.redF();
            hsvTexData[i*3+1]=col.greenF();
            hsvTexData[i*3+2]=col.blueF();
        }

        glGenTextures(numTextures,textures);
        glBindTexture(GL_TEXTURE_1D, textures[hsvTex]);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,12,0,GL_RGB,GL_FLOAT,hsvTexData);
        glBindTexture(GL_TEXTURE_1D,0);
}

void MyGLJuliaWidget::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    m_program->setUniformValue(m_jul,jul);


    GLfloat vertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f
    };

    float fScaleX=fScale*(float)width()/(float)height();
    GLfloat *vecPositions= new GLfloat[8];
    vecPositions[0]=posx-fScaleX;
    vecPositions[1]=posy-fScale;
    vecPositions[2]=posx+fScaleX;
    vecPositions[3]=posy-fScale;
    vecPositions[4]=posx+fScaleX;
    vecPositions[5]=posy+fScale;
    vecPositions[6]=posx-fScaleX;
    vecPositions[7]=posy+fScale;



    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_fracCoordAttr, 2, GL_FLOAT, GL_FALSE, 0, vecPositions);
    m_program->setUniformValue(m_iterations, CJuliaOptions::iIterations);
    m_program->setUniformValue("tex",0);
    m_program->setUniformValue("granularity",COptions::granularity);
    m_program->setUniformValue("texOffset",COptions::animationOffset);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_fracCoordAttr);

    glEnable(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, textures[hsvTex]);
    glDrawArrays(GL_QUADS, 0, 4);
    glBindTexture(GL_TEXTURE_1D,0);

    glDisableVertexAttribArray(m_fracCoordAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program->release();


}

void MyGLJuliaWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void MyGLJuliaWidget::wheelEvent(QWheelEvent *e)
{
    fScale*=1.0+(float)(e->delta())/1000.0;
    update();
}

void MyGLJuliaWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (bMoving)
    {
        float dx=(float)(e->x()-mouseX)/(float)height()*2.0;
        float dy=(float)(e->y()-mouseY)/(float)height()*2.0;
        mouseX=e->x();
        mouseY=e->y();
        posx-=dx*fScale;
        posy+=dy*fScale;
        update();
    }
}

void MyGLJuliaWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton){
        bMoving=true;
        mouseX=e->x();
        mouseY=e->y();
    }
}

void MyGLJuliaWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    bMoving=false;
}

void MyGLJuliaWidget::juliaPos(float x, float y)
{
   jul = QVector2D(x,y);
   update();
}

void MyGLJuliaWidget::setIterations(int it)
{
    CJuliaOptions::iIterations=it;
}


