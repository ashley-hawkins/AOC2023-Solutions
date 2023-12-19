#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <optional>
#include <string_view>
#include <tuple>
#include <utility>

#include <ranges>
namespace r = std::ranges;
namespace v = r::views;

#include "../generator.hpp"

int main()
{
	auto lines = []() -> cppcoro::generator<std::string> {
		std::string line;
		while (std::getline(std::cin, line))
		{
			co_yield line;
		}
	}();

	auto numbers =
	    lines | v::transform([](auto line) {
		    std::optional<std::int8_t> first;
		    std::optional<std::int8_t> last;
#ifdef PART_2
		    std::string_view first_search_area;
		    std::string_view last_search_area;
#endif
		    {
			    auto pred = [](const auto &x) {
				    return '0' <= x && x <= '9';
			    };
			    auto line_rev = line | v::reverse;
			    auto first_it = r::find_if(line, pred);
			    auto last_it = r::find_if(line_rev, pred);
#ifdef PART_2
			    auto first_idx = std::distance(line.begin(), first_it);
			    auto last_idx = std::distance(last_it, line_rev.end()) - 1;
			    first_search_area = std::string_view{line}.substr(0, first_idx);
			    last_search_area = std::string_view{line}.substr(last_idx + 1);
#endif
			    if (first_it != r::end(line))
			    {
				    first = *first_it - '0';
			    }
			    if (last_it != r::end(line_rev))
			    {
				    last = *last_it - '0';
			    }
		    }

#ifdef PART_2
		    {
			    auto nums =
			        std::to_array({"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"});
			    auto first_digit_value = r::fold_left(
			        nums | v::transform([&first_search_area](auto digit) { return first_search_area.find(digit); }) |
			            v::enumerate | v::filter([](auto elem) {
				            auto const &[_, idx] = elem;
				            return idx != std::string_view::npos;
			            }),
			        std::optional<std::int8_t>{},
			        [best_idx = std::string_view::size_type{0}](auto acc, auto current) mutable {
				        auto const &[numeric_value, idx] = current;
				        if (!acc || idx < best_idx)
				        {
					        best_idx = idx;
					        return std::optional{static_cast<std::int8_t>(numeric_value)};
				        }
				        return acc;
			        });
			    auto last_digit_value = r::fold_left(
			        nums | v::transform([&last_search_area](auto digit) { return last_search_area.rfind(digit); }) |
			            v::enumerate | v::filter([](auto elem) {
				            auto const &[_, idx] = elem;
				            return idx != std::string_view::npos;
			            }),
			        std::optional<std::int8_t>{},
			        [best_idx = std::string_view::size_type{0}](auto acc, auto current) mutable {
				        auto const &[numeric_value, idx] = current;
				        if (!acc || idx > best_idx)
				        {
					        best_idx = idx;
					        return std::optional{static_cast<std::int8_t>(numeric_value)};
				        }
				        return acc;
			        });
			    if (first_digit_value)
			    {
				    first = first_digit_value;
			    }
			    if (last_digit_value)
			    {
				    last = last_digit_value;
			    }
		    }
#endif

		    if (first && last)
		    {
			    auto res = *first * 10 + *last;
#ifndef NDEBUG
			    std::cerr << line << ": " << res << "\n";
#endif
			    return res;
		    }
#ifndef NDEBUG
		    std::cerr << line << ": invalid\n";
#endif
		    return 0;
	    });
	auto total = r::fold_left(numbers, 0, std::plus{});
	std::cout << total << "\n";
}
