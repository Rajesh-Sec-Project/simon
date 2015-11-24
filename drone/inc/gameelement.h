#ifndef SIMON_GAMEELEMENT_H
#define SIMON_GAMEELEMENT_H

#include <string>

class GameSystem;

//! A generic game element class
//! Every component of the Game System should
//!   inherit this class.
//! It provides (for the moment) logging abstractions
class GameElement
{
public:
	GameElement(GameSystem& system);
	virtual ~GameElement();

protected:
	//! Send a trace log to the host
	void M_trace(std::string const& msg);
	//! Send a message log to the host
	void M_message(std::string const& msg);
	//! Send a warning log to the host
	void M_warning(std::string const& msg);
	//! Send an error log to the host
	void M_error(std::string const& msg);

private:
	std::string M_className() const;

protected:
	GameSystem& m_system;
};

#endif // SIMON_GAMEELEMENT_H
