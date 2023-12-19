// This is literally the only way I could complete this because my laptop
// doens't have enough memory to brute force it. This is definitely better
// than the bruteforce, but the code is pretty messy.

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

struct range
{
	enum class intersection_description
	{
		outside_left,
		outside_right,
		overlaps_left,
		overlaps_right,
		contains,
		contained,
		identical
	};
	std::int64_t start;
	std::int64_t length;

	std::pair<intersection_description, std::optional<range>> intersects(const range &other) const
	{
		auto intersected_range = range{};
		intersected_range.start = std::max(start, other.start);
		intersected_range.length = std::min(start + length, other.start + other.length) - intersected_range.start;
		if (start == other.start && length == other.length)
		{
			return {intersection_description::identical, intersected_range};
		}
		if (start + length <= other.start)
		{
			return {intersection_description::outside_left, std::nullopt};
		}
		if (other.start + other.length <= start)
		{
			return {intersection_description::outside_right, std::nullopt};
		}
		if (start <= other.start && other.start + other.length <= start + length)
		{
			return {intersection_description::contains, intersected_range};
		}
		if (other.start <= start && start + length <= other.start + other.length)
		{
			return {intersection_description::contained, intersected_range};
		}
		if (start <= other.start)
		{
			return {intersection_description::overlaps_left, intersected_range};
		}
		return {intersection_description::overlaps_right, intersected_range};
	}
};

int main()
{
	std::string line;
	if (!std::getline(std::cin, line))
	{
		return EXIT_FAILURE;
	}
	auto colon_idx = line.find(':');
	std::vector<range> seed_ranges;
	decltype(seed_ranges) seed_ranges_map_buffer;
	{
		std::istringstream iss{line.substr(colon_idx + 1)};
		for (std::int64_t seed_id{}, seed_range{}; iss >> seed_id >> seed_range;)
		{
			seed_ranges.emplace_back(seed_id, seed_range);
		}
	}
	seed_ranges_map_buffer.resize(seed_ranges.size());
	std::swap(seed_ranges, seed_ranges_map_buffer);
	while (std::getline(std::cin, line))
	{
		if (line.empty())
		{
			std::copy(seed_ranges_map_buffer.begin(), seed_ranges_map_buffer.end(), seed_ranges.begin());
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			std::int64_t dst_start{};
			std::int64_t src_start{};
			std::int64_t length{};
			std::istringstream iss{line};
			iss >> dst_start >> src_start >> length;
			range src_range{src_start, length};

			for (std::size_t i = 0; i < seed_ranges.size(); ++i)
			{
				const auto seed_range = seed_ranges[i];
				auto [intersection_description, intersection] = seed_range.intersects(src_range);
				if (intersection_description != range::intersection_description::outside_left &&
				    intersection_description != range::intersection_description::outside_right)
				{
					{
						auto mapped_range = *intersection;
						mapped_range.start = dst_start + (intersection->start - src_range.start);
						if (mapped_range.length > 0)
						{
							seed_ranges_map_buffer[i] = mapped_range;
							// set the source range to an empty range so that it doesn't get
							// processed again
							seed_ranges[i] = range{};
						}
					}
					if (intersection_description == range::intersection_description::identical ||
					    intersection_description == range::intersection_description::contained)
					{
					}
					else
					{
						if (intersection_description == range::intersection_description::overlaps_left ||
						    intersection_description == range::intersection_description::contains)
						{
							auto leftover = range{seed_range.start, intersection->start - seed_range.start};
							// This shouldn't need to be here but something is wrong with my code
							if (leftover.length > 0)
							{
								seed_ranges.push_back(leftover);
								seed_ranges_map_buffer.push_back(leftover);
							}
						}
						if (intersection_description == range::intersection_description::overlaps_right ||
						    intersection_description == range::intersection_description::contains)
						{
							auto leftover = range{intersection->start + intersection->length,
							                      seed_range.start + seed_range.length - intersection->start -
							                          intersection->length};
							if (leftover.length > 0)
							{
								seed_ranges.push_back(leftover);
								seed_ranges_map_buffer.push_back(leftover);
							}
						}
					}
				}
			}
		}
	}
	std::swap(seed_ranges, seed_ranges_map_buffer);

	std::cout << std::min_element(
	                 seed_ranges.begin(), seed_ranges.end(), [](auto a, auto b) { return a.start < b.start; })
	                 ->start
	          << '\n';
}
