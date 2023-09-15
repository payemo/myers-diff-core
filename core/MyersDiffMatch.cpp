#include "MyersDiffMatch.hpp"

#include <stdexcept>
#include <iterator>

namespace differ
{
	// PUBLIC DEFINITIONS

	DiffList MyersDiffMatch::ComputeDiff(const String& textA, const String& textB)
	{
		DiffList diffs{};

		// Check for equality
		if (textA == textB)
		{
			if (textA.size() > 0)
			{
				diffs.push_back(Diff(Operation::EQUAL, textA));
			}
			return diffs;
		}

		// Trim common left part
		UInt32 commonLength = utils::CommonPrefixLength(textA, textB);
		const String& commonPrefix = textA.substr(0, commonLength);

		String trimmedA = textA.substr(commonLength);
		String trimmedB = textB.substr(commonLength);

		// Trim common right part
		commonLength = utils::CommonSuffixLength(textA, textB);
		const String& commonSuffix = trimmedA.substr(trimmedA.size() - commonLength);

		trimmedA = trimmedA.substr(0, trimmedA.size() - commonLength);
		trimmedB = trimmedB.substr(0, trimmedB.size() - commonLength);

		diffs = Compute(trimmedA, trimmedB);

		if (!commonPrefix.empty())
		{
			diffs.push_front(Diff{ Operation::EQUAL, commonPrefix });
		}
		if (!commonSuffix.empty())
		{
			diffs.push_back(Diff{ Operation::EQUAL, commonSuffix });
		}

		if (!diffs.empty())
		{
			CleanupMerge(diffs);
		}

		return diffs;
	}

	PatchList MyersDiffMatch::MakePatch(const String& textA, const String& textB)
	{
		if (textA.empty() || textB.empty())
		{
			return {};
		}

		DiffList diffs = ComputeDiff(textA, textB);

		return MakePatch(textA, diffs);
	}

	String MyersDiffMatch::PatchesToText(const PatchList& patches)
	{
		String text;
		for (const auto& patch : patches)
		{
			text += patch.ToString();
		}
		return text;
	}

	// PROTECTED DEFINITIONS

	PatchList MyersDiffMatch::MakePatch(const String& text, const DiffList& diffs)
	{
		PatchList patches{};

		if (diffs.empty())
		{
			return patches;
		}

		Patch patch{};

		String prePatchText = text;
		Int32 prePatchCount = 0;

		String postPatchText = text;
		Int32 postPatchCount = 0;

		for (const auto& diff : diffs)
		{
			Operation diffOp = diff.GetOperation();
			Int32 diffLength = diff.GetTextLength();

			if (!patch.HasDiffs() && diffOp != Operation::EQUAL)
			{
				patch.startA = prePatchCount;
				patch.startB = postPatchCount;
			}

			switch (diff.GetOperation())
			{
			case Operation::INSERT:
				patch.Append(diff);

				patch.lengthB += diffLength;

				postPatchText = postPatchText.substr(0, postPatchCount) +
					diff.Text() +
					postPatchText.substr(postPatchCount);
				break;

			case Operation::DELETE:
				patch.Append(diff);

				patch.lengthA += diffLength;

				postPatchText = postPatchText.substr(0, postPatchCount) +
					postPatchText.substr(postPatchCount + diffLength);
				break;

			case Operation::EQUAL:
				if (diffLength <= 2 * patchMargin && patch.HasDiffs() && !(diff == diffs.back()))
				{
					patch.Append(diff);
					patch.lengthA += diffLength;
					patch.lengthB += diffLength;
				}

				if (diff.GetTextLength() >= 2 * patchMargin)
				{
					if (patch.HasDiffs())
					{
						// Add context
						AddContext(patch, prePatchText);

						patches.push_back(patch);

						patch = Patch{};
						prePatchText = postPatchText;
						prePatchCount = postPatchCount;
					}
				}
				break;
			}

			if (diffOp != Operation::INSERT)
			{
				prePatchCount += diffLength;
			}
			if (diffOp != Operation::DELETE)
			{
				postPatchCount += diffLength;
			}
		}

		if (patch.HasDiffs())
		{
			AddContext(patch, prePatchText);
			patches.push_back(patch);
		}

		return patches;
	}

	void MyersDiffMatch::AddContext(Patch& patch, const String& text)
	{
		if (text.empty())
		{
			return;
		}

		String pattern = text.substr(patch.startB, patch.lengthA);
		Int32 pad = 0;
		Int32 textSize = text.size();

		while (text.find(pattern) != text.rfind(pattern) && pattern.size() < 32 - 2 * patchMargin)
		{
			pad += patchMargin;
			pattern = text.substr(std::max(0, patch.startB - pad),
				std::min(textSize, patch.startB + patch.lengthA + pad) -
				std::max(0, patch.startA - pad));
		}

		pad += patchMargin;

		String prefix = text.substr(std::max(0, patch.startB - pad),
			patch.startB - std::max(0, patch.startB - pad));

		if (!prefix.empty())
		{
			patch.Prepend(Diff{ Operation::EQUAL, prefix });
		}

		String suffix = text.substr(patch.startB + patch.lengthA,
			std::min(textSize, patch.startB + patch.lengthA + pad) - (patch.startB + patch.lengthA));

		if (!suffix.empty())
		{
			patch.Append(Diff{ Operation::EQUAL, suffix });
		}

		patch.startA -= prefix.size();
		patch.startB -= prefix.size();

		patch.lengthA += prefix.size() + suffix.size();
		patch.lengthB += prefix.size() + suffix.size();
	}

	// PRIVATE DEFINITIONS

	DiffList MyersDiffMatch::Compute(const String& seqA, const String& seqB)
	{
		DiffList diffs{};

		if (seqA.size() == 0 && seqB.size() > 0)
		{
			diffs.push_back(Diff(Operation::INSERT, seqB));
			return diffs;
		}

		if (seqB.size() == 0 && seqA.size() > 0)
		{
			diffs.push_back(Diff(Operation::DELETE, seqA));
			return diffs;
		}

		{
			const String& longer = seqA.size() > seqB.size() ? seqA : seqB;
			const String& shorter = seqA.size() > seqB.size() ? seqB : seqA;
			Int32 i = utils::SearchIndexAt(longer, shorter);

			if (i != -1)
			{
				// shortest text is a substring of a longer one.
				Operation op = (seqA.size() > seqB.size()) ? Operation::DELETE : Operation::INSERT;
				diffs.push_back(Diff(op, longer.substr(0, i)));
				diffs.push_back(Diff(Operation::EQUAL, shorter));
				diffs.push_back(Diff(op, longer.substr(i + shorter.size())));
				return diffs;
			}

			if (shorter.size() == 1)
			{
				diffs.push_back(Diff(Operation::DELETE, seqA));
				diffs.push_back(Diff(Operation::INSERT, seqB));
				return diffs;
			}
		}

		return MiddleSnake(seqA, seqB);
	}

	DiffList MyersDiffMatch::MiddleSnake(const String& seqA, const String& seqB)
	{
		const Int32 m = seqA.size();
		const Int32 n = seqB.size();
		const Int32 delta = m - n;
		// when delta is odd, check for overlap only while extending forward paths
		// when delta is even, check for overlap only while extending reverse paths
		const bool deltaIsOdd = (delta % 2 != 0);
		const Int32 maxD = (m + n + 1) / 2;
		const Int32 vOffset = maxD;
		const Int32 vLength = maxD * 2;
		Int32 fStart = 0, fEnd = 0;
		Int32 rStart = 0, rEnd = 0;

		std::vector<Int32> fV(vLength, -1);
		fV[vOffset + 1] = 0;

		std::vector<Int32> rV(vLength, -1);
		rV[vOffset + 1] = 0;

		for (Int32 d = 0; d < maxD; ++d)
		{
			// Find the end of the furthest reaching forward D-path in diagonal k.
			for (Int32 k = -d + fStart; k <= d - fEnd; k += 2)
			{
				Int32 kOffset = vOffset + k;
				Int32 x;
				if (k == -d || (k != d && fV[kOffset - 1] < fV[kOffset + 1]))
				{
					x = fV[kOffset + 1];
				}
				else
				{
					x = fV[kOffset - 1] + 1;
				}

				Int32 y = x - k;

				while (x < m && y < n && seqA[x] == seqB[y])
				{
					++x, ++y;
				}

				fV[kOffset] = x;

				if (x > m)
				{
					fEnd += 2;
				}
				else if (y > n)
				{
					fStart += 2;
				}
				else if (deltaIsOdd)
				{
					/*
						Check if the path overlaps the furthest reaching reverse(D - 1)-path in diagonal k,
							where k ∈[∆ − (D − 1), ∆ + (D − 1)]
					*/
					Int32 rOffset = vOffset + delta - k; // furthest reaching point of a REVERSED (D - 1)-path of k
					if (rOffset >= 0 && rOffset < vLength && rV[rOffset] != -1)
					{
						/*
							(feasibility) u+v >= ceil(D/2) and x+y <= N + M − floor(D/2), and
							(overlap) x-y = u-v and x >= u.
						*/
						Int32 u = m - rV[rOffset];
						if (x >= u)
						{ // overlaps
							return Partition(seqA, seqB, x, y);
						}
					}
				}
			}

			// Find the end of the furthest reaching forward D-path in diagonal k + delta.
			for (Int32 k = -d + rStart; k <= d - rEnd; k += 2)
			{
				Int32 kOffset = vOffset + k;
				Int32 u;
				if (k == -d || (k != d && (rV[kOffset - 1] < rV[kOffset + 1])))
				{
					u = rV[kOffset + 1];
				}
				else
				{
					u = rV[kOffset - 1] + 1;
				}

				Int32 v = u - k;

				while (u < m && v < n && seqA[m - u - 1] == seqB[n - v - 1])
				{
					++u, ++v;
				}

				rV[kOffset] = u;

				if (u > m)
				{
					rEnd += 2;
				}
				else if (v > n)
				{
					rStart += 2;
				}
				else if (!deltaIsOdd)
				{
					Int32 fOffset = vOffset + delta - k;
					if (fOffset >= 0 && fOffset < vLength && fV[fOffset] != -1)
					{
						Int32 x = fV[fOffset];
						Int32 y = vOffset + x - fOffset;

						// Check overlapping
						if (x >= (m - u))
						{
							return Partition(seqA, seqB, x, y);
						}
					}
				}
			}
		}

		DiffList diffs;
		diffs.push_back(Diff(Operation::DELETE, seqA));
		diffs.push_back(Diff(Operation::INSERT, seqB));

		return diffs;
	}

	DiffList MyersDiffMatch::Partition(const String& a, const String& b, Int32 x, Int32 y)
	{
		String text1a = a.substr(0, x);
		String text2a = b.substr(0, y);
		String text1b = a.substr(x);
		String text2b = b.substr(y);

		DiffList diffsA = ComputeDiff(text1a, text2a);
		DiffList diffsB = ComputeDiff(text1b, text2b);

		diffsA.insert(diffsA.end(), diffsB.begin(), diffsB.end());
		return diffsA;
	}

	void MyersDiffMatch::CleanupMerge(DiffList& diffs)
	{
		diffs.push_back(Diff{ Operation::EQUAL, "" });
		DiffList::iterator ptr = diffs.begin();
		DiffList::iterator tmp = ptr;

		int cntDelete = 0, cntInsert = 0;
		String textDelete = "", textInsert = "";

		Diff* thisDiff = &(*tmp++);
		Diff* prevEq = nullptr;
		int commonLength;

		while (thisDiff != nullptr)
		{
			auto op = thisDiff->GetOperation();

			switch (op)
			{
			case Operation::DELETE:
				++cntDelete;
				textDelete += thisDiff->Text();
				prevEq = nullptr;
				break;

			case Operation::INSERT:
				++cntInsert;
				textInsert += thisDiff->Text();
				prevEq = nullptr;
				break;

			case Operation::EQUAL:
				if (cntDelete + cntInsert > 1)
				{
					bool both = cntDelete != 0 && cntInsert != 0;
					tmp = std::prev(tmp);

					while (cntDelete-- > 0)
					{
						tmp = std::prev(tmp);
						tmp = diffs.erase(tmp);
					}

					while (cntInsert-- > 0)
					{
						tmp = std::prev(tmp);
						tmp = diffs.erase(tmp);
					}

					if (both)
					{
						commonLength = utils::CommonPrefixLength(textInsert, textDelete);

						if (commonLength != 0)
						{
							if (tmp != diffs.begin())
							{
								thisDiff = &(*std::prev(tmp--));
								if (thisDiff->GetOperation() != Operation::EQUAL)
								{
									throw "Previous diff should have been an equality";
								}
								thisDiff->AppendText(textInsert.substr(0, commonLength));
							}
							else
							{
								tmp = diffs.insert(tmp, Diff{ Operation::EQUAL, textInsert.substr(0, commonLength) });
							}
							textInsert = textInsert.substr(commonLength);
							textDelete = textDelete.substr(commonLength);
						}

						commonLength = utils::CommonSuffixLength(textInsert, textDelete);
						if (commonLength != 0)
						{
							thisDiff = &(*std::next(tmp++));
							thisDiff->PrependText(textInsert.substr(textInsert.size() - commonLength));

							textInsert = textInsert.substr(0, textInsert.size() - commonLength);
							textDelete = textDelete.substr(0, textDelete.size() - commonLength);
						}
					}
					// insert merged records
					if (!textDelete.empty())
					{
						tmp = diffs.insert(tmp, Diff{ Operation::DELETE, textDelete });
						tmp++;
					}
					if (!textInsert.empty())
					{
						tmp = diffs.insert(tmp, Diff{ Operation::INSERT, textInsert });
						tmp++;
					}

					// Step forward to the equality
					thisDiff = (tmp != diffs.end()) ? &(*tmp++) : nullptr;
				}
				else if (prevEq != nullptr)
				{
					// Merge equality with the previous one
					prevEq->AppendText(thisDiff->Text());
					tmp = diffs.erase(--tmp);

					thisDiff = &(*std::prev(tmp));
				}
				cntDelete = cntInsert = 0;
				textDelete = textInsert = "";
				prevEq = thisDiff;
				break;
			}
			
			thisDiff = (tmp != diffs.end()) ? &(*(tmp++)) : nullptr;
		}

		if (diffs.back().Text().empty())
		{
			diffs.pop_back();
		}

		bool changes = false;
		tmp = diffs.begin();

		Diff* prevDiff = (tmp != diffs.end()) ? &(*tmp++) : nullptr;
		thisDiff = (tmp != diffs.end()) ? &(*tmp++) : nullptr;
		Diff* nextDiff = (tmp != diffs.end()) ? &(*tmp++) : nullptr;

		while (nextDiff != nullptr)
		{
			if (prevDiff->GetOperation() == Operation::EQUAL &&
				nextDiff->GetOperation() == Operation::EQUAL)
			{
				// Single edit surrounded by equalities
				if (utils::StringEndsWith(thisDiff->Text(), prevDiff->Text()))
				{
					String newText = prevDiff->Text() + 
						thisDiff->Text().substr(0, thisDiff->GetTextLength() - prevDiff->GetTextLength());

					thisDiff->ReplaceText(std::move(newText));

					nextDiff->PrependText(prevDiff->Text());

					tmp = std::prev(tmp);
					tmp = std::prev(tmp);
					tmp = std::prev(tmp);
					tmp = diffs.erase(tmp);
					tmp = std::next(tmp);
					thisDiff = &(*tmp++);
					nextDiff = (tmp != diffs.end()) ? &(*tmp++) : nullptr;
					changes = true;
				}
				else if (utils::StringStartsWith(thisDiff->Text(), nextDiff->Text()))
				{
					prevDiff->AppendText(nextDiff->Text());

					String replace = thisDiff->Text().substr(nextDiff->GetTextLength()) + nextDiff->Text();
					thisDiff->ReplaceText(std::move(replace));

					tmp = diffs.erase(--tmp);
					nextDiff = (tmp != diffs.end()) ? &(*tmp++) : nullptr;
					changes = true;
				}
			}
			prevDiff = thisDiff;
			thisDiff = nextDiff;
			nextDiff = (tmp != diffs.end()) ? &(*tmp++) : nullptr;
		}

		if (changes)
		{
			CleanupMerge(diffs);
		}
	}

	const String MyersDiffMatch::DiffText1(const DiffList& diffs) const
	{
		String out;

		for (auto di = diffs.begin(); di != diffs.end(); ++di)
		{
			const auto& d = *di;

			if (d.GetOperation() != Operation::INSERT)
			{
				out += d.Text();
			}
		}

		return out;
	}

	const String MyersDiffMatch::DiffText2(const DiffList& diffs) const
	{
		String out;

		for (auto di = diffs.begin(); di != diffs.end(); ++di)
		{
			const auto& d = *di;

			if (d.GetOperation() != Operation::DELETE)
			{
				out += d.Text();
			}
		}

		return out;
	}
}