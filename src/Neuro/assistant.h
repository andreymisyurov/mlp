#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QPainter>

#include <paintscene.h>
#include <Matrix/neuron_matrix.h>
#include <mtx_method/perzeptron_mtx.h>

#include <iostream>
#include <QDebug>
#include <QFileDialog>

class Assistant {
public:
    static Assistant& getInstanse();

    NeuronMatrix sceneToNeuronMatrix(PaintScene *in_scene);
    QString openFile(const QString &in_tail);
    QString openSaveFile();
    Perceptron *createPerceptron(int in_layers, int in_first_layer, bool in_mode);
    PaintScene* getScene();


private:
    Assistant():m_scene(new PaintScene) {};
    PaintScene* m_scene;

public:
    ~Assistant() {
        delete m_scene;
    };
};

#endif // ASSISTANT_H
