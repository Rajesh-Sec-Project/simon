#ifndef CREDITSVIEW_H
#define CREDITSVIEW_H

#include <QWidget>
#include <memory>

namespace Ui {
    class CreditsView;
}

class CreditsView : public QWidget {
    Q_OBJECT

public:
    explicit CreditsView(QWidget* parent = 0);
    ~CreditsView();

private slots:
    void M_back();

private:
    std::unique_ptr<Ui::CreditsView> m_ui;
};

#endif // CREDITSVIEW_H
