#ifndef DIFF_PATH_ASSERT_HELPER_HPP
#define DIFF_PATH_ASSERT_HELPER_HPP

#include "defs.hpp"

#include <iostream>

namespace differ
{
	namespace tests
	{
		class AssertHelper
		{
		public:
			static void AssertEqual(const String& testCase, const String& s1, const String& s2);
			static void AssertEqual(const String& testCase, const Size s1, const Size s2);

		private:
			template<typename T>
			static void PrintErrorMessage(const String& descr, const T& exp, const T& actual);
		};
	}
}

#endif // DIFF_PATH_ASSERT_HELPER_HPP