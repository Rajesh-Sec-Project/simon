#include "creditsview.h"
#include "ui_creditsview.h"
#include "lcomm/info_packet.h"
#include "viewmanager.h"
#include "commmanager.h"

using namespace lcomm;

CreditsView::CreditsView(QWidget *parent) :
    QWidget(parent),
    m_ui(std::make_unique<Ui::CreditsView>())
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->backButton, SIGNAL(clicked()), this, SLOT(M_back()));
}

CreditsView::~CreditsView()
{

}

void CreditsView::M_back() {
    ViewManager::switchToMainMenu();
}
