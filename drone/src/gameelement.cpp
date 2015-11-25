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
