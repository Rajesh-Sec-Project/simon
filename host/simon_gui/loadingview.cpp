#include "loadingview.h"
#include "ui_loadingview.h"
#include "lcomm/info_packet.h"
#include "viewmanager.h"
#include "commmanager.h"

using namespace lcomm;

LoadingView::LoadingView(QWidget* parent)
        : QWidget(parent)
        , m_ui(std::make_unique<Ui::LoadingView>()) {
    m_ui->setupUi(this);

    QObject::connect(&CommManager::self(), SIGNAL(packetReceived(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)),
                     this, SLOT(M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase>)));
}

LoadingView::~LoadingView() {
}

void LoadingView::M_receivedInfo(lcomm::Endpoint*, std::shared_ptr<lcomm::PacketBase> packet) {
    InfoPacket* info = packet->downcast<InfoPacket>();
    if(info) {
        if(info->state() & InfoPacket::AppReady) {
            ViewManager::switchToMainMenu();
        }
    }
}
