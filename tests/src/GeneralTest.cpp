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

			myersDiffMatchTest.RunCleanupMergeTest();
			myersDiffMatchTest.RunDiffText();
			myersDiffMatchTest.RunMiddleSnakeTest();
			myersDiffMatchTest.RunComputeDiffTest();
			myersDiffMatchTest.RunMakePatchTest();
		}
	}
}