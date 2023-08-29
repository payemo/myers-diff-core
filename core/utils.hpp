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
		inline UInt32 SearchIndexAt(const String& a, const String& b)
		{
			auto at = std::search(a.begin(), a.end(), b.begin(), b.end());
			return (at < a.end()) ? at - a.begin() : -1;
		}
	}
}

#endif