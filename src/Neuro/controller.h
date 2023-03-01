#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QResizeEvent>

#include <paintscene.h>
#include <assistant.h>
#include <mtx_method/perzeptron_mtx.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

class Controller : public QWidget
{
    Q_OBJECT

public:
    explicit Controller(QWidget *parent = nullptr);
    ~Controller();

public:
    void predict();

private:
    Ui::Controller *m_ui;
    // попытаться вынести сцену и перцептрон в класс ассистант
    PaintScene *m_scene;
    Perceptron *m_neuro_mode;
};
#endif // CONTROLLER_H
