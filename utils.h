#pragma once
#include <string>
#include <iostream>
#include <limits>

class redirect_output_wrapper
{
    std::ostream& stream;
    std::streambuf* const old_buf;
public:
	redirect_output_wrapper(std::ostream& src)
		:old_buf(src.rdbuf()), stream(src)
    {
    }

    ~redirect_output_wrapper() {
		stream.rdbuf(old_buf);
    }
	void redirect (std::ostream& dest)
	{
		stream.rdbuf(dest.rdbuf());
	}
};

#define INPUT_LINE(in, str) getline(in >> std::ws, str); \
                            std::cerr << str << std::endl

void clearStdin();
std::string trim(const std::string& str);
std::string readLineNonEmpty(const std::string& message);
double readDouble(const std::string& message, double minVal, double maxVal);
int readInt(const std::string& message, int minVal, int maxVal);

