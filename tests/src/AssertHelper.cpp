#include "AssertHelper.hpp"

namespace differ
{
	namespace tests
	{
		void AssertHelper::AssertEqual(const String& testDescr, const String& s1, const String& s2)
		{
			if (s1 != s2)
			{
				PrintErrorMessage(testDescr, s1, s2);
				throw testDescr;
			}
			else
			{
				std::cout << testDescr << " " << "OK" << std::endl;
			}
		}

		void AssertHelper::AssertEqual(const String& testDescr, const Int32 s1, const Int32 s2)
		{
			if (s1 != s2)
			{
				PrintErrorMessage(testDescr, s1, s2);
				throw testDescr;
			}
			else
			{
				std::cout << testDescr << " " << "OK" << std::endl;
			}
		}

		void AssertHelper::AssertEqual(const String& testCase, const DiffList& list1, const DiffList& list2)
		{
			bool fail = false;

			if (list1.size() == list2.size())
			{
				auto it1 = list1.begin();
				auto it2 = list2.begin();

				while (it1 != list1.end())
				{
					if (*it1 != *it2)
					{
						fail = true;
						break;
					}
					++it1, ++it2;
				}
			}
			else
			{
				fail = true;
			}

			if (fail)
			{
				bool first = false;
				String str1 = "(";

				auto it1 = list1.begin();
				auto it2 = list2.begin();

				while (it1 != list1.end())
				{
					if (!first)
					{
						str1 += ", ";
					}
					str1 += (*it1).ToString();
					first = false;
				}
				str1 += ")";

				first = true;
				String str2 = "(";

				while (it2 != list2.end())
				{
					if (!first)
					{
						str2 += ", ";
					}
					str2 += (*it2).ToString();
					first = false;
				}
				str2 += ")";

				std::cout << testCase << " FAIL\n" << "Expected:" << str1 << "Actual:" << str2 << std::endl;

				throw testCase;
			}

			std::cout << testCase << " OK" << std::endl;
		}

		// PRIVATE

		template<typename T>
		void AssertHelper::PrintErrorMessage(const String& descr, const T& exp, const T& actual)
		{
			std::cout << descr << " " << "FAIL" << std::endl;
			std::cout << "Expected: " << exp << std::endl;
			std::cout << "Actual: " << actual << std::endl;
		}

		// TEMPLATE DEFINITIONS

		template void AssertHelper::PrintErrorMessage(const String&, const String&, const String&);
		template void AssertHelper::PrintErrorMessage(const String&, const Int32&, const Int32&);
	}
}