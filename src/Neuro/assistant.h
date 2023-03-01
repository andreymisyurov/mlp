#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QPainter>
#include <iostream>
#include <QDebug>
#include <QFileDialog>
#include <QSoundEffect>
#include <QString>

#include "paintscene.h"
#include "neuron_matrix.h"
#include "perzeptron_mtx.h"
#include "learnanswer.h"
#include "parser.h"


class Assistant {
public:
    static Assistant& getInstance();

    NeuronMatrix sceneToNeuronMatrix(PaintScene *in_scene);
    QString openFile(const QString &in_tail);
    QString openSaveFile();
    Perceptron *createPerceptron(int in_layers, int in_first_layer, bool in_mode);
    PaintScene* getScene();
    LearnAnswer* getGraph() { return m_graph_er;};
    void createGraphic(std::pair<int, std::vector<double>> in_data);
    void openImg();
    void playSound(const QString& path);
    std::string getFileFromResources(const QString& in_res_name, const QString& in_filename);

private:
    Assistant():m_scene(new PaintScene), m_graph_er(new LearnAnswer) {};
    PaintScene* m_scene;
    LearnAnswer* m_graph_er;
    QSoundEffect m_effect;

public:
    ~Assistant() {
        delete m_scene;
        delete m_graph_er;
    };
};

#endif // ASSISTANT_H
