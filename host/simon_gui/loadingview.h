#ifndef LOADINGVIEW_H
#define LOADINGVIEW_H

#include <QWidget>
#include <memory>
#include "lcomm/lcomm.h"

namespace Ui {
    class LoadingView;
}

class LoadingView : public QWidget {
    Q_OBJECT

public:
    explicit LoadingView(QWidget* parent = 0);
    ~LoadingView();

private slots:
    void M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet);

private:
    std::unique_ptr<Ui::LoadingView> m_ui;
};

#endif // LOADINGVIEW_H
