#ifndef DIFF_PATH_MYERS_DIFF_MATCH_TEST_HPP
#define DIFF_PATH_MYERS_DIFF_MATCH_TEST_HPP

#include "UtilsTest.hpp"
#include "MyersDiffMatch.hpp"

namespace differ
{
	namespace tests
	{
		class MyersDiffMatchTest
		{
		public:
			void RunAll();

		private:
			MyersDiffMatch mdm;

			UtilsTest utilityTests;
		};
	}
}

#endif // DIFF_PATH_MYERS_DIFF_MATCH_TEST_HPP