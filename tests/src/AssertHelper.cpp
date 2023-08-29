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

		void AssertHelper::AssertEqual(const String& testDescr, const Size s1, const Size s2)
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

		template<typename T>
		void AssertHelper::PrintErrorMessage(const String& descr, const T& exp, const T& actual)
		{
			std::cout << descr << " " << "FAIL" << std::endl;
			std::cout << "Expected: " << exp << std::endl;
			std::cout << "Actual: " << actual << std::endl;
		}

		// TEMPLATE DEFINITIONS

		template void AssertHelper::PrintErrorMessage(const String&, const String&, const String&);
		template void AssertHelper::PrintErrorMessage(const String&, const Size&, const Size&);
	}
}