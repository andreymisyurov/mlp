#ifndef SRC_NEURO_ASSISTANT_CONTROLLER_H
#define SRC_NEURO_ASSISTANT_CONTROLLER_H

#include <QResizeEvent>

#include "ui_controller.h"
#include "paintscene.h"
#include "assistant.h"
#include "learnanswer.h"

using namespace s21;

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

class Controller: public QWidget {
 Q_OBJECT

 public:
  explicit Controller(QWidget *parent = nullptr);
  ~Controller() override;

 public:
  void doPrediction();
  void doCrossValidation();
  void doLearn();
  void doTest();
  void doImport();
  void doExport();
  void openLearnFile();
  void openTestFile();
  void openImportFile();
  void openExportFile();
  void openImg();
  void createAI();
  void moveSlider();

 private:
  void checkGoButtons();

 private:
  QIntValidator *m_validator;
  Perceptron *m_neuro_mode;
  Ui::Controller *m_ui;
  const QString k_ERROR_SOUND = ":/crit_sound";
  const QString k_SUCCESS_SOUND = ":/success_sound";
};

#endif // SRC_NEURO_ASSISTANT_CONTROLLER_H
