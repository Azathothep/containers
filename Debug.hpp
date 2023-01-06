#ifndef DEBUG_HPP
# define DEBUG_HPP

# include <iostream>
# include <fstream>

class Debug {
	private:

	public:
		Debug();
		~Debug();

		static std::ofstream Log;
};

std::ofstream Debug::Log("log.txt", std::ios::app);

#endif