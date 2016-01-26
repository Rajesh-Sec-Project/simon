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

#ifndef COMMMANAGER_H
#define COMMMANAGER_H

#include <QObject>
#include "lcomm/lcomm.h"
#include <QDebug>
#include <memory>

class CommManager : public QObject, public lcomm::Subscriber {
    Q_OBJECT

private:
    friend std::unique_ptr<CommManager>::deleter_type;

    explicit CommManager();
    virtual ~CommManager();

public:
    static CommManager& self();
    static void destroy();
    bool opened();

    void write(lcomm::PacketBase const& packet);

    void reconnect();

signals:
    void connected();
    void disconnected();
    // /!\ Par pitié Rémi, laisse des pointeurs ici sinon le signal ne marche pas
    void packetReceived(lcomm::Endpoint* ep, std::shared_ptr<lcomm::PacketBase> packet);

private:
    void notify(lcomm::Endpoint& ep, std::shared_ptr<lcomm::PacketBase> packet) override;
    static std::unique_ptr<CommManager> M_makeCommManager();

private:
    std::unique_ptr<lcomm::Endpoint> m_ep;
    static std::unique_ptr<CommManager> m_self;
};

#endif // COMMMANAGER_H
