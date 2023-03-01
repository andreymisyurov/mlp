#include "controller.h"
#include "ui_controller.h"
#include <QDebug>

Controller::Controller(QWidget *parent): QWidget(parent),
    m_ui(new Ui::Controller),
    m_neuro_mode(new PerzeptronMtx()),
    m_validator(new QIntValidator(this)) {
  m_ui->setupUi(this);

  // копируем темп файл из ресурсов
  QString temp_file = QCoreApplication::applicationDirPath() + "/temp.bin";
  QFile::copy(":/best_weights.bin", temp_file);
  auto p = QFile(temp_file).permissions();
  QFile::setPermissions(temp_file, p | QFileDevice::ReadOwner | QFileDevice::WriteOwner);
  m_neuro_mode->import_data(temp_file.toStdString());
  QFile(temp_file).remove();

  // устанавливаем валидацию на ввод и начальный цвет сцены
  m_ui->le_learn_epoch->setValidator(m_validator);
  m_ui->le_neurons_first_layer->setValidator(m_validator);
  Assistant::getInstance().getScene()->setBackgroundBrush(Qt::white);


  // инициализируем и устанавливаем сцену
  Assistant::getInstance().getScene()->setSceneRect(0, 0,
      m_ui->graphic_scene->width() - 20, m_ui->graphic_scene->height() - 20);
  m_ui->graphic_scene->setScene(Assistant::getInstance().getScene());

  // устанавливаем начальное значение селектора и проверяем кнопки и поля
  m_ui->rb_matrix_mode->setChecked(true);
  checkGoButtons();

  // налаживаем коннект
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
  connect(m_ui->s_test_proc, &QSlider::valueChanged, this, &Controller::moveSlider);
  connect(m_ui->b_open_img, &QPushButton::clicked, this, &Controller::openImg);
}

Controller::~Controller() {
  delete m_ui;
  delete m_neuro_mode;
  delete m_validator;
}

void Controller::openImg() {
  Assistant::getInstance().openImg();
}

void Controller::moveSlider() {
  m_ui->l_test_slider->setText(QString::number(m_ui->s_test_proc->value() / 100.));
}

void Controller::doPrediction() {
  NeuronMatrix temp = Assistant::getInstance().sceneToNeuronMatrix(Assistant::getInstance().getScene());
  auto result = m_neuro_mode->predict(temp);
  m_ui->l_answer->setText(QString(QChar::fromLatin1(result + 64)));
  Assistant::getInstance().getScene()->clear();
}

void Controller::checkGoButtons() {
  m_ui->l_path_import->text().isEmpty() ? m_ui->b_do_import->setEnabled(false) : m_ui->b_do_import->setEnabled(true);
  m_ui->l_path_export->text().isEmpty() ? m_ui->b_do_export->setEnabled(false) : m_ui->b_do_export->setEnabled(true);

  if(m_ui->l_path_test->text().isEmpty()) {
    m_ui->b_do_test->setEnabled(false);
    m_ui->s_test_proc->setEnabled(false);
  } else {
    m_ui->b_do_test->setEnabled(true);
    m_ui->s_test_proc->setEnabled(true);
  }

//  if(m_ui->l_path_learn->text().isEmpty()) {
//    m_ui->b_do_learn->setEnabled(false);
//    m_ui->le_learn_epoch->setEnabled(false);
//  } else {
//    m_ui->b_do_learn->setEnabled(true);
//    m_ui->le_learn_epoch->setEnabled(true);
//  }
}

void Controller::openLearnFile() {
  m_ui->l_path_learn->setText(Assistant::getInstance().openFile("*.csv"));
  checkGoButtons();
}


void Controller::openTestFile() {
  m_ui->l_path_test->setText(Assistant::getInstance().openFile("*.csv"));
  checkGoButtons();
}

void Controller::openImportFile() {
  m_ui->l_path_import->setText(Assistant::getInstance().openFile("*_weights.bin"));
  checkGoButtons();
}

void Controller::openExportFile() {
  m_ui->l_path_export->setText(Assistant::getInstance().openSaveFile());
  checkGoButtons();
}

void Controller::createAI() {
  // дообработать по человечески эксепшн
  // добавить графовый метод в класс Assistant
  // возможно вынести QMessageBox в класс assistant
  try {
    Perceptron *temp =
        Assistant::getInstance().createPerceptron((m_ui->cb_count_hidden_layers->currentText()).toInt(), (m_ui->le_neurons_first_layer->text()).toInt(), m_ui->rb_matrix_mode->isChecked());
    std::swap(temp, m_neuro_mode);
    delete temp;
  } catch(MyException &ex) {
    Assistant::getInstance().playSound(k_ERROR_SOUND);
    QMessageBox(QMessageBox::Icon::Critical, "Danger", ex.what(), QMessageBox::StandardButton::Apply, this).exec();
  } catch(...) {
    throw;
  }
}

void Controller::do_learn() {
  // возможно доработать обработку эксепшена
  // подумать о графики функции, создать qcustomplot
  // помозговать о хранении данных в векторе, возможно уменьшить вектор размером
//  auto result = m_neuro_mode->learn("/Users/gonzo/program/school21/general/CPP7_MLP-0/datasets/one_al.txt", 1);
  try {
    auto result = m_neuro_mode->learn(m_ui->l_path_learn->text().toStdString(), m_ui->le_learn_epoch->text().toInt());
    Assistant::getInstance().createGraphic(result);
  } catch(MyException &ex) {
    Assistant::getInstance().playSound(k_ERROR_SOUND);
    QMessageBox(QMessageBox::Icon::Critical, "Danger", ex.what(), QMessageBox::StandardButton::Apply, this).exec();
  } catch(...) {
    throw;
  }
}

void Controller::do_test() {
  // возможно доработать обработку эксепшена
  // загнать логику в ассистанс
  try {
    analytical_data
        result = m_neuro_mode->test(m_ui->l_path_test->text().toStdString(), m_ui->s_test_proc->value() / 100.);
    QString analise_str = "Average accuracy = " + QString::number(result.average_accuracy) + '\n' + "Precision = "
        + QString::number(result.precision) + '\n' + "Recall = " + QString::number(result.recall) + '\n'
        + "F-measure = " + QString::number(result.f_measure) + '\n';
    QMessageBox(QMessageBox::Icon::Information, "Analytics", analise_str, QMessageBox::StandardButton::Apply, this).exec();
  } catch(MyException &ex) {
    Assistant::getInstance().playSound(k_ERROR_SOUND);
    QMessageBox(QMessageBox::Icon::Critical, "Danger", "incorrect input file", QMessageBox::StandardButton::Apply, this).exec();
  } catch(...) {
    throw;
  }
}

void Controller::do_import() {
  // возможно доработать обработку эксепшена
  try {
    m_neuro_mode->import_data(m_ui->l_path_import->text().toStdString());
    QMessageBox(QMessageBox::Icon::Information, "Import", "imported successfully", QMessageBox::StandardButton::Apply, this).exec();
  } catch(MyException &ex) {
    Assistant::getInstance().playSound(k_ERROR_SOUND);
    QMessageBox(QMessageBox::Icon::Critical, "Danger", "incorrect input file", QMessageBox::StandardButton::Apply, this).exec();
  } catch(...) {
    throw;
  }
}

void Controller::do_export() {
  // возможно доработать обработку эксепшена
  try {
    m_neuro_mode->export_data(m_ui->l_path_export->text().toStdString());
    QMessageBox(QMessageBox::Icon::Information, "Import", "exported successfully", QMessageBox::StandardButton::Apply, this).exec();
  } catch(MyException &ex) {
    Assistant::getInstance().playSound(k_ERROR_SOUND);
    QMessageBox(QMessageBox::Icon::Critical, "Danger", "incorrect input file", QMessageBox::StandardButton::Apply, this).exec();
  } catch(...) {
    throw;
  }
}

