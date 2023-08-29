#include "MyersDiffMatchTest.hpp"

namespace differ
{
	namespace tests
	{
		void MyersDiffMatchTest::RunAll()
		{
			utilityTests.CommonPrefixTest();
			utilityTests.CommonSuffixTest();
			utilityTests.SearchIndexAtTest();
		}
	}
}