#ifndef CONTROLLER_H
#define CONTROLLER_H

// TASKS

// so-so, переделать апгрейдовать функцию transponce
// задокументировать функции
// причесать ресурсы и навести порядок в директориях проекта
// вынести реализацию из парсера, оформить библиотекой
// построить структуру инклюдов. почистить лишние
// собрать мейкфайл с целями all, install, uninstall, clean, dvi, dist, tests

// уточнить формулы аналитики тестирования
// добавить некий стиль к кнопкам и полям
// заменить иконки клавиш go на зеленые кнопки "play"
// добавить иконку приложения

// +оформить NeuronMatrix статическиой библиотекой
// +подгружать стартовую базу из ресурсов
// +реализовать сохранение и загрузку из бинарных файлов
// +ввод только интов в поле эпохи
// +ввод только интов в поле первого слоя
// +обработать эксепшены вылетом окна (можно со звуком)
// +заблокировать возможность расшериня окна
// +оптимизировать пути к библиотекам, обрезать пути в инклюдах
// +написать тесты на каждую функцию, подтянуть gtest в симейку
// +?(переделать на тестовую выборку или нет) составление графика изменения ошибки, посчитанной на тестовой выборке, для каждой эпохи обучения
// +?(проверить формулы) вылеталово на экран окна с аналитикой тестирования
// +реализовать загрузку img(вывод на сцену-экран, предсказание) BMP-изображения (размер изображения может достигать 512x512)
// +окошко со вводом эпох.
// +ползунок с процентом тестовой выборки
// +инвертировать матрицу, поменять цвета. чтобы было черным по белому
// +подключить обучение
// +подключить импорт/экспорт
// +перенести scene в класс assistant

#include <QWidget>
#include <QResizeEvent>
#include <QMessageBox>

#include "paintscene.h"
#include "assistant.h"
#include "learnanswer.h"

//перенести песнь в ресурсы
const QString k_ERROR_SOUND = ":/krit.wav";

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

class Controller : public QWidget {
    Q_OBJECT

public:
    explicit Controller(QWidget *parent = nullptr);
    ~Controller() override;

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
    void moveSlider();
    void openImg();


private:
    QIntValidator *m_validator;
    Ui::Controller *m_ui;
    Perceptron *m_neuro_mode;
};
#endif // CONTROLLER_H
