#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
QT_FORWARD_DECLARE_CLASS(QTimer)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionProperties_triggered();
    void animationTriggered();

    void on_groupAnimate_toggled(bool arg1);

    void on_lineEdit_granularity_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QTimer *animationTimer;
};

#endif // MAINWINDOW_H
