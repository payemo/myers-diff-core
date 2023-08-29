#include "MyersDiffMatchTest.hpp"

using namespace differ;
using namespace tests;

int main(int argc, char** argv)
{
	try
	{
		MyersDiffMatchTest mdm;
		mdm.RunAll();
	}
	catch (String caseDescr)
	{
		std::cout << "Test failed: " + caseDescr << std::endl;
	}

	return 0;
}