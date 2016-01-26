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

#include "gameelement.h"
#include "gamesystem.h"
#include <cxxabi.h>

GameElement::GameElement(GameSystem& system)
        : m_system(system) {
}

GameElement::~GameElement() {
}

void GameElement::gameInit() {
}

void GameElement::gameLoop() {
}

void GameElement::M_trace(std::string const& msg) {
    m_system.trace(M_className(), msg);
}

void GameElement::M_message(std::string const& msg) {
    m_system.message(M_className(), msg);
}

void GameElement::M_warning(std::string const& msg) {
    m_system.warning(M_className(), msg);
}

void GameElement::M_error(std::string const& msg) {
    m_system.error(M_className(), msg);
}

std::string GameElement::M_className() const {
    char* demangled = abi::__cxa_demangle(typeid(*this).name(), 0, 0, 0);
    std::string name = demangled;
    free(demangled);
    return name;
}
