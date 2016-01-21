#include "viewmanager.h"

std::unique_ptr<MainMenu> ViewManager::m_mainMenu;
std::unique_ptr<HighScores> ViewManager::m_highScores;
std::unique_ptr<Lost> ViewManager::m_lost;
std::unique_ptr<GameView> ViewManager::m_game;
std::unique_ptr<LoadingView> ViewManager::m_loading;
std::unique_ptr<CreditsView> ViewManager::m_credits;
std::unique_ptr<SettingsView> ViewManager::m_settings;
GameWindow* ViewManager::m_window;
int ViewManager::m_score;
