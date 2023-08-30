#include "GeneralTest.hpp"

using namespace differ;
using namespace tests;

int main(int argc, char** argv)
{
	try
	{
		GeneralTest tests;
		tests.RunAll();
	}
	catch (String caseDescr)
	{
		std::cout << "Test failed: " + caseDescr << std::endl;
	}

	return 0;
}