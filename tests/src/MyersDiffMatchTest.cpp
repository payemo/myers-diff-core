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

			AssertHelper::AssertEqual(
				"MyersDiffMatch::DiffText1: Returns left text difference.",
				"jumps over the lazy",
				mdm.DiffText1(diffs));

			AssertHelper::AssertEqual(
				"MyersDiffMatch::DiffText1: Returns right text difference.",
				"jumped over a lazy",
				mdm.DiffText2(diffs));
		}

		void MyersDiffMatchTest::RunMiddleSnakeTest()
		{
			String a = "cat";
			String b = "map";

			DiffList diffs
			{
				Diff(Operation::DELETE, "c"),
				Diff(Operation::INSERT, "m"),
				Diff(Operation::EQUAL, "a"),
				Diff(Operation::DELETE, "t"),
				Diff(Operation::INSERT, "p")
			};

			AssertHelper::AssertEqual("MyersDiffMatch::MiddleSnake: Normal.", diffs, mdm.MiddleSnake(a, b));
		}

		void MyersDiffMatchTest::RunComputeDiffTest()
		{
			DiffList diffs{};
			AssertHelper::AssertEqual("MyersDiffMatch::ComputDiff: Empty inputs.", diffs, mdm.ComputeDiff("", ""));

			diffs = { Diff{Operation::EQUAL, "abc"} };
			AssertHelper::AssertEqual("MyersDiffMatch::ComputDiff: Equal inputs.", diffs, mdm.ComputeDiff("abc", "abc"));

			diffs = { Diff{ Operation::EQUAL, "ab" }, Diff{ Operation::INSERT, "123" }, Diff{ Operation::EQUAL, "c" } };
			AssertHelper::AssertEqual("MyersDiffMatch::ComputDiff: Simple insertion.", diffs, mdm.ComputeDiff("abc", "ab123c"));

			diffs = { Diff{ Operation::EQUAL, "a" }, Diff{ Operation::DELETE, "123" }, Diff{ Operation::EQUAL, "bc" } };
			AssertHelper::AssertEqual("MyersDiffMatch::ComputDiff: Simple deletion.", diffs, mdm.ComputeDiff("a123bc", "abc"));

			diffs =
			{
				Diff{Operation::EQUAL, "a"},
				Diff{Operation::DELETE, "123"},
				Diff{Operation::EQUAL, "b"},
				Diff{Operation::DELETE, "456"},
				Diff{Operation::EQUAL, "c"}
			};
			AssertHelper::AssertEqual("MyersDiffMatch::ComputDiff: Two deletions.", diffs, mdm.ComputeDiff("a123b456c", "abc"));

			diffs = { Diff{ Operation::DELETE, "a" }, Diff{ Operation::INSERT, "b" } };
			AssertHelper::AssertEqual("MyersDiffMatch::ComputDiff: Simple diff case.", diffs, mdm.ComputeDiff("a", "b"));

			diffs =
			{
				Diff {Operation::DELETE, "Apple"},
				Diff {Operation::INSERT, "Ba"},
				Diff {Operation::INSERT, "nana"},
				Diff {Operation::EQUAL, "s are a"},
				Diff {Operation::INSERT, "lso"},
				Diff {Operation::EQUAL, " fruit."}
			};
			AssertHelper::AssertEqual(
				"MyersDiffMatch::ComputDiff: Simple case #2.",
				diffs,
				mdm.ComputeDiff("Apples are a fruit.", "Bananas are also fruit."));

			diffs =
			{
				Diff {Operation::DELETE, "1"},
				Diff {Operation::EQUAL, "a"},
				Diff {Operation::DELETE, "y"},
				Diff {Operation::EQUAL, "b"},
				Diff {Operation::DELETE, "2"},
				Diff {Operation::INSERT, "xab"}
			};
			AssertHelper::AssertEqual("MyersDiffMatch::ComputDiff: Overlap #1.", diffs, mdm.ComputeDiff("1ayb2", "abxab"));

			diffs =
			{
				Diff{Operation::INSERT, " "},
				Diff{Operation::EQUAL, "a"},
				Diff{Operation::INSERT, "n"},
				Diff{Operation::INSERT, "d"},
				Diff{Operation::EQUAL, " [[Pennsylvania]]"},
				Diff{Operation::DELETE, " "},
				Diff{Operation::DELETE, "and"},
				Diff{Operation::DELETE, " [[New"}
			};
			AssertHelper::AssertEqual(
				"MyersDiffMatch::ComputDiff: Large equality.", 
				diffs, mdm.ComputeDiff("a [[Pennsylvania]] and [[New", " and [[Pennsylvania]]"));

			diffs =
			{
				Diff {Operation::INSERT, "x"},
				Diff {Operation::EQUAL, "a"},
				Diff {Operation::INSERT, "xc"},
				Diff {Operation::INSERT, "xa"},
				Diff {Operation::EQUAL, "bc"},
				Diff {Operation::DELETE, "y"}
			};
			AssertHelper::AssertEqual("MyersDiffMatch::ComputDiff: Overlap #2.", diffs, mdm.ComputeDiff("abcy", "xaxcxabc"));
		}

		void MyersDiffMatchTest::RunMakePatchTest()
		{
			PatchList patches;
			patches = mdm.MakePatch("", "");
			AssertHelper::AssertEqual("MyersDiffMatch::MakePatch: empty inputs.", "", mdm.PatchesToText(patches));

			String text1 = "The quick brown fox jumps over the lazy dog.";
			String text2 = "That quick brown fox jumped over a lazy dog.";

			String expectedPatch = "@@ -1,8 +1,7 @@\n Th\n-at\n+e\n  qui\n@@ -21,17 +21,18 @@\n jump\n-ed\n+s\n  over \n+th\n-a\n+e\n  laz\n";
			patches = mdm.MakePatch(text2, text1);
			AssertHelper::AssertEqual("MyersDiffMatch::MakePatch: Text2+Text1 inputs", expectedPatch, mdm.PatchesToText(patches));

			expectedPatch = "@@ -1,11 +1,12 @@\n Th\n-e\n+at\n  quick b\n@@ -22,18 +22,17 @@\n jump\n-s\n+e\n+d\n  over \n+a\n-t\n-he\n  laz\n";
			patches = mdm.MakePatch(text1, text2);
			AssertHelper::AssertEqual("MyersDiffMatch::MakePatch: Text2+Text1 inputs", expectedPatch, mdm.PatchesToText(patches));
		}
	}
}