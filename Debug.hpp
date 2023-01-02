#ifndef DEBUG_HPP
# define DEBUG_HPP

# include <iostream>
#include <stdexcept>
#include <fstream>
#include <cstdlib>
#include <sstream>

class Debug {
	private:

	public:
		Debug();
		~Debug();

		static void Log(std::string m) {
			std::string _fileName = "log.txt";

			std::ofstream file(_fileName.c_str(), std::ios::app);

			file.exceptions(std::ofstream::badbit | std::ofstream::failbit);

			try
			{
				file << m;
				if (m[m.size() - 1] != '\n')
					file << std::endl;
				file.close();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}

		template < typename T >
		static std::string ToStr(T n) {
			std::stringstream ss;

			ss << n;

			return ss.str();
		}
};

#endif