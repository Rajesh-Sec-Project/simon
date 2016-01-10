#ifndef LOST_H
#define LOST_H

#include <QWidget>

namespace Ui {
class lost;
struct structScore
    {
        std::string s_name;
        int s_score;
    };
}

class Lost : public QWidget
{
    Q_OBJECT

public:
    explicit Lost(QWidget *parent = 0);
    ~Lost();

private slots :
    void M_saveName();

private:
    Ui::lost *m_ui;

};

#endif // LOST_H
