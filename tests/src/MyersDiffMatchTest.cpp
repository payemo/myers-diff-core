#include "MyersDiffMatchTest.hpp"
#include "AssertHelper.hpp"

namespace differ
{
	namespace tests
	{
		void MyersDiffMatchTest::RunDiffText()
		{
			DiffList diffs
			{
				Diff{Operation::EQUAL, "jump"},
				Diff{Operation::DELETE, "s"},
				Diff{Operation::INSERT, "ed"},
				Diff{Operation::EQUAL, " over "},
				Diff{Operation::DELETE, "the"},
				Diff{Operation::INSERT, "a"},
				Diff{Operation::EQUAL, " lazy"}
			};

			tests::AssertHelper::AssertEqual(
				"MyersDiffMatch::DiffText1: Returns left text difference.",
				"jumps over the lazy",
				mdm.DiffText1(diffs));

			tests::AssertHelper::AssertEqual(
				"MyersDiffMatch::DiffText1: Returns right text difference.",
				"jumped over a lazy",
				mdm.DiffText2(diffs));
		}
	}
}