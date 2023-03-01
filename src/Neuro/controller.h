#ifndef CONTROLLER_H
#define CONTROLLER_H

// TASKS
// +инвертировать матрицу, поменять цвета. чтобы было черным по белому
// +подключить обучение
// реализовать вывод с тестов
// +подключить импорт/экспорт
// перенести scene в класс assistant
// реализовать загрузку картинок с предсказанием

#include <QWidget>
#include <QResizeEvent>

#include <paintscene.h>
#include <assistant.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

class Controller : public QWidget {
    Q_OBJECT

public:
    explicit Controller(QWidget *parent = nullptr);
    ~Controller();

public:
    void doPrediction();
    void openLearnFile();
    void openTestFile();
    void openImportFile();
    void openExportFile();
    void checkGoButtons();
    void createAI();
    void do_learn();
    void do_test();
    void do_import();
    void do_export();

private:
    Ui::Controller *m_ui;
    Perceptron *m_neuro_mode;
};
#endif // CONTROLLER_H
