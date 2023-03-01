#include "controller.h"
#include "./ui_controller.h"

Controller::Controller(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::Controller),
//      m_scene(new PaintScene()),
//      m_scene(Assistant::getInstanse().getScene()),
      m_neuro_mode(new PerzeptronMtx()) {
    // сделать стационарную базу в ресурсах, и поменять кусок кода
    m_neuro_mode->import_data("/Users/gonzo/program/school21/general/CPP7_MLP-0/src/data_weights/data_9e_3l_256.txt");
    m_ui->setupUi(this);
//    m_scene->setBackgroundBrush(Qt::white);
    Assistant::getInstanse().getScene()->setBackgroundBrush(Qt::white);

    Assistant::getInstanse().getScene()->setSceneRect(0,0, m_ui->graphic_scene->width() - 20, m_ui->graphic_scene->height() - 20);
    m_ui->graphic_scene->setScene(Assistant::getInstanse().getScene());
    m_ui->rb_matrix_mode->setChecked(true);
    checkGoButtons();

    connect(m_ui->b_predict, &QPushButton::clicked, this, &Controller::doPrediction);
    connect(m_ui->b_path_learn, &QPushButton::clicked, this, &Controller::openLearnFile);
    connect(m_ui->b_path_test, &QPushButton::clicked, this, &Controller::openTestFile);
    connect(m_ui->b_path_import, &QPushButton::clicked, this, &Controller::openImportFile);
    connect(m_ui->b_path_export, &QPushButton::clicked, this, &Controller::openExportFile);
    connect(m_ui->b_create_AI, &QPushButton::clicked, this, &Controller::createAI);
    connect(m_ui->b_do_learn, &QPushButton::clicked, this, &Controller::do_learn);
    connect(m_ui->b_do_test, &QPushButton::clicked, this, &Controller::do_test);
    connect(m_ui->b_do_import, &QPushButton::clicked, this, &Controller::do_import);
    connect(m_ui->b_do_export, &QPushButton::clicked, this, &Controller::do_export);
}

Controller::~Controller() {
//    delete m_scene;
    delete m_ui;
    delete m_neuro_mode;
}

void Controller::doPrediction() {
    NeuronMatrix temp = Assistant::getInstanse().sceneToNeuronMatrix(Assistant::getInstanse().getScene());
    auto result = m_neuro_mode->predict(temp);
    m_ui->l_answer->setText(QString::number(result));
    qDebug() << result;
    Assistant::getInstanse().getScene()->clear();
}

void Controller::checkGoButtons() {
    m_ui->l_path_learn->text().isEmpty() ? m_ui->b_do_learn->setEnabled(false) : m_ui->b_do_learn->setEnabled(true);
    m_ui->l_path_test->text().isEmpty() ? m_ui->b_do_test->setEnabled(false) : m_ui->b_do_test->setEnabled(true);
    m_ui->l_path_import->text().isEmpty() ? m_ui->b_do_import->setEnabled(false) : m_ui->b_do_import->setEnabled(true);
    m_ui->l_path_export->text().isEmpty() ? m_ui->b_do_export->setEnabled(false) : m_ui->b_do_export->setEnabled(true);
}

void Controller::openLearnFile() {
    m_ui->l_path_learn->setText(Assistant::getInstanse().openFile("*.csv"));
    checkGoButtons();
}

void Controller::openTestFile() {
    m_ui->l_path_test->setText(Assistant::getInstanse().openFile("*.csv"));
    checkGoButtons();
}

void Controller::openImportFile() {
    m_ui->l_path_import->setText(Assistant::getInstanse().openFile("*_weights.txt"));
    checkGoButtons();
}

void Controller::openExportFile() {
    m_ui->l_path_export->setText(Assistant::getInstanse().openSaveFile());
    checkGoButtons();
}

void Controller::createAI() {
    // дообработать по человечески эксепшн
    // добавить графовый метод в класс Assistant
    try {
        Perceptron* temp = Assistant::getInstanse().createPerceptron((m_ui->cb_count_hidden_layers->currentText()).toInt(),
                            (m_ui->le_neurons_first_layer->text()).toInt(), m_ui->rb_matrix_mode->isChecked());
        std::swap(temp, m_neuro_mode);
        delete temp;
    } catch(MyException& ex) {
        qDebug() << ex.what();
    } catch(...) {
        throw;
    }
}

void Controller::do_learn() {
    // добавить количество эпох
    // возможно доработать обработку эксепшена
    m_neuro_mode->learn(m_ui->l_path_learn->text().toStdString());
}

void Controller::do_test() {
    // возможно доработать обработку эксепшена
    analytical_data result = m_neuro_mode->test(m_ui->l_path_test->text().toStdString());

    // реализовать графиком или в доп окне аналитическую информацию
    qDebug() << result.average_accuracy << result.time;
}

void Controller::do_import() {
    // возможно доработать обработку эксепшена
    m_neuro_mode->import_data(m_ui->l_path_import->text().toStdString());
}

void Controller::do_export() {
    // возможно доработать обработку эксепшена
    m_neuro_mode->export_data(m_ui->l_path_export->text().toStdString());
}














