#include "UtilsTest.hpp"
#include "AssertHelper.hpp"

namespace differ
{
	namespace tests
	{
		using namespace utils;

		void UtilsTest::CommonPrefixTest()
		{
			AssertHelper::AssertEqual("utils::CommonPrefixLength: Null case.", 0, CommonPrefixLength("abc", "xyz"));
			AssertHelper::AssertEqual("utils::CommonPrefixLength: Non-null case.", 4, CommonPrefixLength("1234abcdef", "1234xyz"));
			AssertHelper::AssertEqual("utils::CommonPrefixLength: Whole case.", 4, CommonPrefixLength("1234", "1234xyz"));
		}

		void UtilsTest::CommonSuffixTest()
		{
			AssertHelper::AssertEqual("utils::CommonSuffixTest: Null case.", 0, CommonSuffixLength("abc", "xyz"));
			AssertHelper::AssertEqual("utils::CommonSuffixTest: Non-null case.", 4, CommonSuffixLength("abcdef1234", "xyz1234"));
			AssertHelper::AssertEqual("utils::CommonSuffixTest: Whole case.", 4, CommonSuffixLength("1234", "xyz1234"));
		}
	}
}