#ifndef SIMON_CONFIGMANAGER_H
#define SIMON_CONFIGMANAGER_H

#include "gameelement.h"
#include "lcomm/clientsocket.h"

class GameSystem;

class ConfigManager : public GameElement
{
public:
	ConfigManager(GameSystem& system);
	~ConfigManager();

	void init();
	std::string getConfig();

private:
	void M_clearAck();

private:
	lcomm::ClientSocket m_control;
};

#endif // SIMON_CONFIGMANAGER_H
