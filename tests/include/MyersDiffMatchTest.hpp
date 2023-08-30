#ifndef DIFF_PATCH_MYERS_DIFF_MATCH_TEST_HPP
#define DIFF_PATCH_MYERS_DIFF_MATCH_TEST_HPP

#include "MyersDiffMatch.hpp"

// TODO: resolve friends conflicts with [test] namespace
namespace differ
{
	class MyersDiffMatchTest
	{
	public:
		void RunDiffText();

	private:
		MyersDiffMatch mdm{};
	};
}

#endif // DIFF_PATCH_MYERS_DIFF_MATCH_TEST_HPP