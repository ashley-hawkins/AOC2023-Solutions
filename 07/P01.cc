#include <algorithm>
#include <charconv>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <ranges>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <range/v3/to_container.hpp>
#include <range/v3/view/enumerate.hpp>

#include "../generator.hpp"

namespace r = std::ranges;
namespace v = r::views;

namespace r3 = ranges;
namespace v3 = r3::views;

enum kind
{
	unknown,
	five_of_a_kind,
	four_of_a_kind,
	full_house,
	three_of_a_kind,
	two_pair,
	one_pair,
	high_card,
};

auto classify_hand(std::string hand)
{
	{
		auto const hand_copy = hand;
		// Jokers are seen as lesser compared to other cards because they can be replaced with any other card, which
		// means that if you have the same amount of jokers as another card, the other card is better. Because you can
		// transform the jokers into that other card and then it would be the most frequent card.
		r::sort(hand, [](auto lhs, auto rhs) {
			if (lhs == 'J')
			{
				return false;
			}
			if (rhs == 'J')
			{
				return true;
			}
			return lhs < rhs;
		});
		r::stable_sort(hand, [&hand_copy](auto lhs, auto rhs) {
			if (lhs == 'J')
			{
				return false;
			}
			if (rhs == 'J')
			{
				return true;
			}
			return r::count(hand_copy, lhs) > r::count(hand_copy, rhs);
		});
	}

#ifdef PART_2
	auto best = hand[0];
	r::transform(hand, begin(hand), [best](auto c) {
		if (c == 'J')
		{
			return best;
		}
		return c;
	});
	r::stable_sort(hand, [best](auto lhs, auto rhs) { return lhs == best && rhs != best; });
#endif
	// Now the hand is sorted by count, then by value as a tie breaker.
	// This means that the first element is the most common, and the last element is the least common.

	// Five of a kind, where all five cards have the same label: AAAAA
	if (hand[0] == hand[4])
	{
		return five_of_a_kind;
	}
	// Four of a kind, where four cards have the same label and one card has a different label: AA8AA
	if (hand[0] == hand[3])
	{
		return four_of_a_kind;
	}
	// Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
	if (hand[0] == hand[2])
	{
		if (hand[3] == hand[4])
		{
			return full_house;
		}
		// Three of a kind, where three cards have the same label, and the remaining two cards are each different from
		// any
		return three_of_a_kind;
	}
	// Two pair, where two cards share one label, two other cards share a second label, and
	// the remaining card has a third label
	if (hand[0] == hand[1])
	{
		if (hand[2] == hand[3])
		{
			return two_pair;
		}
		// One pair, where two cards share one label, and the other three cards
		return one_pair;
	}
	// High card, where all cards' labels are distinct
	return high_card;
}

int main()
{
	auto lines = []() -> cppcoro::generator<std::string> {
		std::string line;
		while (std::getline(std::cin, line))
		{
			co_yield line;
		}
	}();

	auto hands = lines | v::transform([](auto line) -> std::tuple<std::string, std::int32_t, kind> {
		             auto space_idx = line.find(' ');
		             auto hand = line.substr(0, space_idx);
		             kind classification = classify_hand(hand);
		             auto bid_sv = std::string_view{line}.substr(space_idx + 1);
		             std::int32_t bid{};
		             auto res = std::from_chars(begin(bid_sv), end(bid_sv), bid);
		             if (res.ec == std::errc{})
		             {
			             if (hand.empty())
			             {
				             [] {
				             }();
			             }
			             return std::tuple{hand, bid, classification};
		             }
		             std::cerr << '\n' << bid_sv << '\n';
		             std::cerr << '\n' << static_cast<std::underlying_type_t<std::errc>>(res.ec) << '\n';
		             throw std::runtime_error{"failed to parse bid"};
	             }) |
	             ranges::to<std::vector>();

	// First sort by the secondary condition (lexicographical), then stable sort by the primary condition.
	r::sort(hands, [](auto const &lhs, auto const &rhs) {
		// A hand consists of five cards labeled one of A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2. The relative strength
		// of each card follows this order, where A is the highest and 2 is the lowest.
		static std::unordered_map<char, int> value_map{
		    {'A', 13},
		    {'K', 12},
		    {'Q', 11},
#ifdef PART_2
		    {'J', 0},
#else
		    {'J', 10},
#endif
		    {'T', 9},
		    {'9', 8},
		    {'8', 7},
		    {'7', 6},
		    {'6', 5},
		    {'5', 4},
		    {'4', 3},
		    {'3', 2},
		    {'2', 1},
		};
		auto const &[lhs_hand, lhs_bid, lhs_classification] = lhs;
		auto const &[rhs_hand, rhs_bid, rhs_classification] = rhs;
		return r::lexicographical_compare(
		    lhs_hand, rhs_hand, [](auto lhs_card, auto rhs_card) { return value_map[lhs_card] < value_map[rhs_card]; });
	});
	r::stable_sort(begin(hands), end(hands), [](auto const &lhs, auto const &rhs) {
		auto const &[lhs_hand, lhs_bid, lhs_classification] = lhs;
		auto const &[rhs_hand, rhs_bid, rhs_classification] = rhs;
		// inverted because better hands have lower numeric values
		return lhs_classification > rhs_classification;
	});
	std::uint64_t tally = r::fold_left(hands | v3::enumerate, 0, [](auto acc, auto const &hand_enumeration) {
		auto const &[i, hand] = hand_enumeration;
		auto const &[hand_str, bid, classification] = hand;
		return acc + bid * (i + 1);
	});
	std::cout << tally << '\n';
}
