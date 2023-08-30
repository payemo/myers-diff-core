#include "GeneralTest.hpp"

namespace differ
{
	namespace tests
	{
		void GeneralTest::RunAll()
		{
			utilsTest.CommonPrefixTest();
			utilsTest.CommonSuffixTest();
			utilsTest.SearchIndexAtTest();

			myersDiffMatchTest.RunDiffText();
		}
	}
}