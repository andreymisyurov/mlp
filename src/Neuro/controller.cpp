#include "controller.h"

Controller::Controller(QWidget *parent): QWidget(parent),
    m_ui(new Ui::Controller),
    m_neuro_mode(new PerzeptronMtx()),
    m_validator(new QIntValidator(this)) {
  m_ui->setupUi(this);

  // копируем темп файл из ресурсов в папку проекта, даем права, вытягиваем с него данные и тут же удаляем
  // тем самым выставляем начальные тренированные веса
  QString temp_file = QCoreApplication::applicationDirPath() + "/temp.bin";
  QFile::copy(":/best_weight", temp_file);
  auto p = QFile(temp_file).permissions();
  QFile::setPermissions(temp_file, p | QFileDevice::ReadOwner | QFileDevice::WriteOwner);
  m_neuro_mode->importDataBase(temp_file.toStdString());
  QFile(temp_file).remove();

  // устанавливаем валидацию на ввод и начальный цвет сцены
  m_ui->le_learn_epoch->setValidator(m_validator);
  m_ui->le_cross_valid->setValidator(m_validator);
  m_ui->le_neurons_first_layer->setValidator(m_validator);
  Assistant::getInstance().getScene()->setBackgroundBrush(Qt::white);

  // инициализируем и устанавливаем сцену, подгружаем файл из ресурсов как стартовый рисунок
  Assistant::getInstance().getScene()->setSceneRect(0, 0,
      m_ui->graphic_scene->width() - 20, m_ui->graphic_scene->height() - 20);
  QImage temp(":/start_img");
  auto el = temp.scaled(Assistant::getInstance().getScene()->sceneRect().size().toSize(),
                                          Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  Assistant::getInstance().getScene()->addPixmap(QPixmap::fromImage(el));
  m_ui->graphic_scene->setScene(Assistant::getInstance().getScene());

  // устанавливаем начальное значение селектора и проверяем активные кнопки и поля
  m_ui->rb_matrix_mode->setChecked(true);
  checkGoButtons();

  // налаживаем коннект
  connect(m_ui->b_predict, &QPushButton::clicked, this, &Controller::doPrediction);
  connect(m_ui->b_path_learn, &QPushButton::clicked, this, &Controller::openLearnFile);
  connect(m_ui->b_path_test, &QPushButton::clicked, this, &Controller::openTestFile);
  connect(m_ui->b_path_import, &QPushButton::clicked, this, &Controller::openImportFile);
  connect(m_ui->b_path_export, &QPushButton::clicked, this, &Controller::openExportFile);
  connect(m_ui->b_create_AI, &QPushButton::clicked, this, &Controller::createAI);
  connect(m_ui->b_do_learn, &QPushButton::clicked, this, &Controller::doLearn);
  connect(m_ui->b_do_cross, &QPushButton::clicked, this, &Controller::doCrossValidation);
  connect(m_ui->b_do_test, &QPushButton::clicked, this, &Controller::doTest);
  connect(m_ui->b_do_import, &QPushButton::clicked, this, &Controller::doImport);
  connect(m_ui->b_do_export, &QPushButton::clicked, this, &Controller::doExport);
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
  m_ui->l_answer->setFont(QFont("Papyrus", 52));
  m_ui->l_answer->setAlignment(Qt::AlignCenter);
  m_ui->l_answer->setText(QString(QChar::fromLatin1(static_cast<char>(result) + 64)));
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
  try {
    Perceptron *temp = Assistant::getInstance().createPerceptron((m_ui->cb_count_hidden_layers->currentText()).toInt(),
                                    (m_ui->le_neurons_first_layer->text()).toInt(), m_ui->rb_matrix_mode->isChecked());
    std::swap(temp, m_neuro_mode);
    delete temp;
    Assistant::getInstance().showWidget(k_SUCCESS_SOUND, QMessageBox::Icon::Information,
                                                                                  "Import", "created successfully");
  } catch(MyException &ex) {
    Assistant::getInstance().showWidget(k_ERROR_SOUND, QMessageBox::Icon::Critical, "Danger", ex.what());
  } catch(...) {
    throw;
  }
}

void Controller::doLearn() {
  try {
    auto result = m_neuro_mode->learn(m_ui->l_path_learn->text().toStdString(), m_ui->le_learn_epoch->text().toInt());
    Assistant::getInstance().createGraphic(result, 0);
    Assistant::getInstance().showWidget(k_SUCCESS_SOUND, std::nullopt);
  } catch(MyException &ex) {
    Assistant::getInstance().showWidget(k_ERROR_SOUND, QMessageBox::Icon::Critical, "Danger", ex.what());
  } catch(...) {
    throw;
  }
}

void Controller::doCrossValidation() {
  try {
    auto result =
        m_neuro_mode->crossValidation(m_ui->l_path_learn->text().toStdString(), m_ui->le_cross_valid->text().toInt());
    Assistant::getInstance().createGraphic(result, 1);
    Assistant::getInstance().showWidget(k_SUCCESS_SOUND, std::nullopt);
  } catch(MyException &ex) {
    Assistant::getInstance().showWidget(k_ERROR_SOUND, QMessageBox::Icon::Critical, "Danger", ex.what());
  } catch(...) {
    throw;
  }
}

void Controller::doTest() {
  try {
    analytical_data
        data = m_neuro_mode->test(m_ui->l_path_test->text().toStdString(), m_ui->s_test_proc->value() / 100.);
    Assistant::getInstance().showWidget(k_SUCCESS_SOUND, QMessageBox::Icon::Information, "Analytics",
                                                                Assistant::getInstance().createAnalyticString(data));
  } catch(MyException &ex) {
    Assistant::getInstance().showWidget(k_ERROR_SOUND, QMessageBox::Icon::Critical, "Danger", "incorrect input file");
  } catch(...) {
    throw;
  }
}

void Controller::doImport() {
  try {
    m_neuro_mode->importDataBase(m_ui->l_path_import->text().toStdString());
    Assistant::getInstance().showWidget(k_SUCCESS_SOUND, QMessageBox::Icon::Information,
                                                                                  "Import", "imported successfully");
  } catch(MyException &ex) {
    Assistant::getInstance().showWidget(k_ERROR_SOUND, QMessageBox::Icon::Critical, "Danger", "incorrect input file");
  } catch(...) {
    throw;
  }
}

void Controller::doExport() {
  try {
    m_neuro_mode->exportDataBase(m_ui->l_path_export->text().toStdString());
    Assistant::getInstance().showWidget(k_SUCCESS_SOUND, QMessageBox::Icon::Information,
                                                                                  "Import", "exported successfully");
  } catch(MyException &ex) {
    Assistant::getInstance().showWidget(k_ERROR_SOUND, QMessageBox::Icon::Critical, "Danger", "incorrect input file");
  } catch(...) {
    throw;
  }
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

  if(m_ui->l_path_learn->text().isEmpty()) {
    m_ui->b_do_cross->setEnabled(false);
    m_ui->le_cross_valid->setEnabled(false);
    m_ui->b_do_learn->setEnabled(false);
    m_ui->le_learn_epoch->setEnabled(false);
  } else {
    m_ui->b_do_cross->setEnabled(true);
    m_ui->le_cross_valid->setEnabled(true);
    m_ui->b_do_learn->setEnabled(true);
    m_ui->le_learn_epoch->setEnabled(true);
  }
}
