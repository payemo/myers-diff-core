#ifndef DIFF_PATCH_UTILS_HPP
#define DIFF_PATCH_UTILS_HPP

#include "defs.hpp"
#include <algorithm>

namespace differ
{
	namespace utils
	{
		inline Size CommonPrefixLength(const String& a, const String& b)
		{
			Size l = std::min(a.size(), b.size());
			auto mms = std::mismatch(a.begin(), a.begin() + l, b.begin());
			return std::distance(a.begin(), mms.first);
		}

		inline Size CommonSuffixLength(const String& a, const String& b)
		{
			Size l = std::min(a.size(), b.size());
			auto mms = std::mismatch(a.rbegin(), a.rbegin() + l, b.rbegin());
			return std::distance(a.rbegin(), mms.first);
		}

		// Caller of that method MUST guarantee that a.size() > b.size()
		inline Int32 SearchIndexAt(const String& a, const String& b)
		{
			auto at = std::search(a.begin(), a.end(), b.begin(), b.end());
			return (at < a.end()) ? at - a.begin() : -1;
		}

		inline bool StringEndsWith(const String& src, const String& end)
		{
			auto s1 = src.size(), s2 = end.size();
			if (s1 < s2)
			{
				return false;
			}
			return std::equal(src.begin() + s1 - s2, src.end(), end.begin());
		}

		inline bool StringStartsWith(const String& src, const String& start)
		{
			if (start.size() > src.size())
			{
				return false;
			}
			return std::equal(start.begin(), start.end(), src.begin());
		}
	}
}

#endif