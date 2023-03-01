#include "controller.h"
#include "./ui_controller.h"

Controller::Controller(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::Controller),
      m_scene(new PaintScene()),
      m_neuro_mode(new PerzeptronMtx())
{
    // сделать стационарную базу в ресурсах, и поменть кусок кода
    m_neuro_mode->import_data("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/data_weights/data_2e_3l_256.txt");
    m_ui->setupUi(this);
    m_scene->setBackgroundBrush(Qt::black);
    m_scene->setSceneRect(0,0, m_ui->graphic_scene->width() - 20, m_ui->graphic_scene->height() - 20);
    m_ui->graphic_scene->setScene(m_scene);
    connect(m_ui->b_predict, &QPushButton::clicked, this, &Controller::predict);
}

Controller::~Controller()
{
    delete m_scene;
    delete m_ui;
    delete m_neuro_mode;
}

void Controller::predict() {
    NeuronMatrix temp = Assistant::sceneToNeuronMatrix(m_scene);
//    std::cout << temp;
    auto result = m_neuro_mode->predict(temp);
    m_ui->l_answer->setText(QString::number(result));
    qDebug() << result;
    m_scene->clear();
}
