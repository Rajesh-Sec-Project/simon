#include "viewmanager.h"

std::unique_ptr<MainMenu> ViewManager::m_mainMenu;
std::unique_ptr<HighScores> ViewManager::m_highScores;
std::unique_ptr<Lost> ViewManager::m_lost;
GameWindow* ViewManager::m_window;
int ViewManager::m_score;
