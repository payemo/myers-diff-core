#ifndef DIFF_PATCH_ASSERT_HELPER_HPP
#define DIFF_PATCH_ASSERT_HELPER_HPP

#include "defs.hpp"
#include "Diff.hpp" // DiffList

#include <iostream>

namespace differ
{
	namespace tests
	{
		class AssertHelper
		{
		public:
			static void AssertEqual(const String& testCase, const String& s1, const String& s2);
			static void AssertEqual(const String& testCase, const Int32 s1, const Int32 s2);
			static void AssertEqual(const String& testCase, const DiffList& list1, const DiffList& list2);

		private:
			template<typename T>
			static void PrintErrorMessage(const String& descr, const T& exp, const T& actual);
		};
	}
}

#endif // DIFF_PATCH_ASSERT_HELPER_HPP