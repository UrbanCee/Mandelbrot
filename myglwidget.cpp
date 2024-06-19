#include "myglwidget.h"
#include <QDebug>
#include <QMainWindow>
#include <QScreen>
#include <QWheelEvent>
#include <QMouseEvent>

MyGLWidget::MyGLWidget(QWidget *parent) :
    QOpenGLWidget(parent), QOpenGLFunctions_4_0_Core(),
    m_program_mandelbrot(0), m_frame(0)
{

}

void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    QString versionString1(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    emit(showStatusBarMessage(QString("OpenGL Version: ")+versionString1,10000));

    m_program_mandelbrot = new QOpenGLShaderProgram(this);
    m_program_mandelbrot->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/mandelbrot.vert");
    qDebug() << "Vertex Shader Mandelbrot: "<< m_program_mandelbrot->log();
    m_program_mandelbrot->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/mandelbrot.frag");
    qDebug() << "Fragment Shader Mandelbrot: "<< m_program_mandelbrot->log();
    m_program_mandelbrot->link();
    qDebug() << "Linking Mandelbrot: "<< m_program_mandelbrot->log();
    m_posAttr = m_program_mandelbrot->attributeLocation("posAttr");
    m_fracCoordAttr = m_program_mandelbrot->attributeLocation("fracCoord");
    m_iterations = m_program_mandelbrot->uniformLocation("iterations");


    m_program_line = new QOpenGLShaderProgram(this);
    m_program_line->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/line.vert");
    qDebug() << "Vertex Shader Line: "<< m_program_line->log();
    m_program_line->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Shaders/line.frag");
    qDebug() << "Fragment Shader Line: "<< m_program_line->log();
    m_program_line->link();
    qDebug() << "Linking Line: "<< m_program_line->log();
    m_line_posAttr = m_program_line->attributeLocation("posAttr");
    m_line_posUniform= m_program_line->uniformLocation("mandelPos");
    m_line_scale = m_program_line->uniformLocation("fScale");

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

    int iGreyGranularity=128;
    GLfloat *greyFlashData= new GLfloat[iGreyGranularity*3];

    for (int i=0;i<iGreyGranularity;i++)
    {
        greyFlashData[i*3]=(float)(i)/(float)(iGreyGranularity-1);
        greyFlashData[i*3+1]=(float)(i)/(float)(iGreyGranularity-1);
        greyFlashData[i*3+2]=(float)(i)/(float)(iGreyGranularity-1);
    }

    glGenTextures(numTextures,textures);
    glBindTexture(GL_TEXTURE_1D, textures[hsvTex]);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,hsvGranularity,0,GL_RGB,GL_FLOAT,hsvTexData);

    glBindTexture(GL_TEXTURE_1D, textures[greyflashTex]);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage1D(GL_TEXTURE_1D,0,GL_RGB,iGreyGranularity,0,GL_RGB,GL_FLOAT,greyFlashData);

    glBindTexture(GL_TEXTURE_1D,0);}

void MyGLWidget::paintGL()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program_mandelbrot->bind();


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
    m_program_mandelbrot->setUniformValue(m_iterations, CMandelbrotOptions::iIterations);
    m_program_mandelbrot->setUniformValue("tex",0);
    m_program_mandelbrot->setUniformValue("granularity",COptions::granularity);
    m_program_mandelbrot->setUniformValue("texOffset",COptions::animationOffset);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_fracCoordAttr);
    glEnable(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, textures[hsvTex]);
    glDrawArrays(GL_QUADS, 0, 4);
    glBindTexture(GL_TEXTURE_1D,0);

    glDisableVertexAttribArray(m_fracCoordAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program_mandelbrot->release();

    if (vecLine.size()>0)
        drawIterationLine();


}

void MyGLWidget::drawIterationLine()
{
    m_program_line->bind();



    glVertexAttribPointer(m_line_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vecLine.data());
    m_program_line->setUniformValue(m_line_posUniform, QVector2D(posx,posy));
    m_program_line->setUniformValue(m_line_scale, fScale);
    m_program_line->setUniformValue("tex",0);
    m_program_line->setUniformValue("granularity",COptions::granularity);
    m_program_line->setUniformValue("texOffset",COptions::animationOffset);

    glEnableVertexAttribArray(m_line_posAttr);

    glBindTexture(GL_TEXTURE_1D, textures[greyflashTex]);
    glDrawArrays(GL_LINE_STRIP, 0, vecLine.size()/2);
    glBindTexture(GL_TEXTURE_1D,0);

    glDisableVertexAttribArray(m_line_posAttr);

    m_program_line->release();


}


void MyGLWidget::calculateIterationLine(float x,float y)
{
    double cx=x;
    double cy=y;
    double oldx;
    double oldy;
    double newx=0.0;
    double newy=0.0;
    vecLine.clear();
    vecLine.append(0.0f);
    vecLine.append(0.0f);
    int i=0;
    for(i = 0; i < CMandelbrotOptions::iLineIterations; i++)
    {
        oldx=newx;
        oldy=newy;


        newx = oldx*oldx-oldy*oldy+cx;
        newy = 2.0*oldx*oldy+cy;
        vecLine.append((float)newx);
        vecLine.append((float)newy);
        if(newx*newx+newy*newy > 1000) break;
    }
    update();
}


void MyGLWidget::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void MyGLWidget::wheelEvent(QWheelEvent *e)
{
    fScale*=1.0+(float)(e->angleDelta().y())/1000.0;
    update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (bMoving)
    {
        float dx=(float)(e->position().x()-mouseX)/(float)height()*2.0;
        float dy=(float)(e->position().y()-mouseY)/(float)height()*2.0;
        mouseX=e->position().x();
        mouseY=e->position().y();
        posx-=dx*fScale;
        posy+=dy*fScale;
        update();
    }

    if (bMoveJulia){
        float x=((float)e->position().x()-(float)width()/2.0)/(float)height()*2.0;
        float y=(float)e->position().y()/(float)height()*2.0-1.0;
        x*=fScale;
        y*=-fScale;
        x+=posx;
        y+=posy;
        emit(sendJuliaPos(x,y));
        emit(showStatusBarMessage(QString("x: %1  y: %2").arg(x).arg(y),5000));
        calculateIterationLine(x,y);
    }

}

void MyGLWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton){
        bMoving=true;
        mouseX=e->position().x();
        mouseY=e->position().y();
    }
    if (e->button()==Qt::RightButton){
        float x=((float)e->position().x()-(float)width()/2.0)/(float)height()*2.0;
        float y=(float)e->position().y()/(float)height()*2.0-1.0;
        x*=fScale;
        y*=-fScale;
        x+=posx;
        y+=posy;
        emit(sendJuliaPos(x,y));
        emit(showStatusBarMessage(QString("x: %1  y: %2").arg(x).arg(y),5000));
        bMoveJulia=true;
        calculateIterationLine(x,y);
    }

}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    bMoving=false;
    bMoveJulia=false;
}

void MyGLWidget::setIterations(int it)
{
    CMandelbrotOptions::iIterations=it;
}


void MyGLWidget::setAnimated(bool)
{

}

