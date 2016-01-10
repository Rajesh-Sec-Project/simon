#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <QWidget>

namespace Ui {
class highscores;
}

class HighScores : public QWidget
{
    Q_OBJECT

public:
    explicit HighScores(QWidget *parent = nullptr);
    ~HighScores();
    void display_Scores();

private slots :
    void M_back();

private:
    Ui::highscores *m_ui;
};

#endif // HIGHSCORES_H
