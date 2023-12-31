#ifndef DIFF_PATCH_UTILS_TEST_HPP
#define DIFF_PATCH_UTILS_TEST_HPP

#include "utils.hpp"
#include "AssertHelper.hpp"

namespace differ
{
	namespace tests
	{
		struct UtilsTest
		{
			void CommonPrefixTest();
			void CommonSuffixTest();
			void SearchIndexAtTest();
		};
	}
}

#endif // DIFF_PATCH_UTILS_TEST_HPP