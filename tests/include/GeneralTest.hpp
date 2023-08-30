#ifndef DIFF_PATCH_GENERAL_TEST_HPP
#define DIFF_PATCH_GENERAL_TEST_HPP

#include "UtilsTest.hpp"
#include "MyersDiffMatchTest.hpp"

namespace differ
{
	namespace tests
	{
		class GeneralTest
		{
		public:
			void RunAll();

		private:
			UtilsTest utilsTest;
			MyersDiffMatchTest myersDiffMatchTest;
		};
	}
}

#endif // DIFF_PATCH_GENERAL_TEST_HPP