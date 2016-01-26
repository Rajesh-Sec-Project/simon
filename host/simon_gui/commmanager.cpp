/*
 * Copyright (c) 2015 Kadambari Melatur, Alexandre Monti, Rémi Saurel and Emma Vareilles
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "commmanager.h"
#include <memory>
#include <QDebug>

std::unique_ptr<CommManager> CommManager::m_self;

CommManager::CommManager()
        : QObject(0)
        , m_ep(std::make_unique<lcomm::Endpoint>(std::make_unique<lcomm::ClientSocket>("192.168.1.1", 50001))) {
    m_ep->registerSubscriber(*this);
}

CommManager::~CommManager() {
}

CommManager& CommManager::self() {
    if(!m_self)
        m_self = M_makeCommManager();
    return *m_self;
}

void CommManager::destroy() {
    m_self = nullptr;
}

std::unique_ptr<CommManager> CommManager::M_makeCommManager() {
    return std::unique_ptr<CommManager>{new CommManager};
}

bool CommManager::opened() {
    return m_ep->socket().opened();
}

void CommManager::notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) {
    emit packetReceived(&ep, packet);
}

void CommManager::reconnect() {
    auto sub = m_ep->subscribers();
    m_ep = std::make_unique<lcomm::Endpoint>(std::make_unique<lcomm::ClientSocket>("192.168.1.1", 50001));
    for(auto s : sub) {
        m_ep->registerSubscriber(*s);
    }
}

void CommManager::write(lcomm::PacketBase const& packet) {
    qDebug() << "write " << &packet;
    while(true) {
        try {
            m_ep->write(packet);
            break;
        } catch(std::exception const& e) {
            std::cerr << "CommManager::write failed: " << e.what() << std::endl;
            this->reconnect();
        }
    }
}
