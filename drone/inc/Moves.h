
#include <list>

namespace lmoves {
	// An enum representing all the different moves that the drone is able to produce
	enum tmove { DOWN = 0, UP, RIGHT, LEFT, NUM_MOVES } ;
	
	// The Moves class contains a sequence of moves (a list)
	class Moves {
		
	public : 
		// constructor 
		Moves();
		
		// getter of the sequence
		std::list<tmove> const& getSequence() const;

		// print the sequence of movements.
		void print() const;

		// add a new move to the sequence
		void add_move() ;

		//return a random move among the moves available in the enum tmove.
		tmove random_move();

	private :
		std::list<tmove> sequence ;
		
	};
}
