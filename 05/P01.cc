#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

int main()
{
	std::string line;
	if (!std::getline(std::cin, line))
	{
		return EXIT_FAILURE;
	}
	auto colon_idx = line.find(':');
	std::vector<std::int64_t> seed_ids;
	std::vector<std::int64_t> seed_ids_map_buffer;
	{
		std::istringstream iss{line.substr(colon_idx + 1)};
		for (std::int64_t seed_id{}; iss >> seed_id;)
		{
			seed_ids.push_back(seed_id);
		}
	}
	seed_ids_map_buffer.resize(seed_ids.size());
	std::swap(seed_ids, seed_ids_map_buffer);
	while (std::getline(std::cin, line))
	{
		if (line.empty())
		{
			std::copy(seed_ids_map_buffer.begin(), seed_ids_map_buffer.end(), seed_ids.begin());
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else
		{
			std::int64_t dst_start{};
			std::int64_t src_start{};
			std::int64_t range{};
			std::istringstream iss{line};
			iss >> dst_start >> src_start >> range;

#pragma omp parallel for
			for (std::size_t i = 0; i < seed_ids.size(); ++i)
			{
				if (src_start <= seed_ids[i] && seed_ids[i] < src_start + range)
				{
					seed_ids_map_buffer[i] = dst_start + (seed_ids[i] - src_start);
				}
			}
		}
	}
	std::swap(seed_ids, seed_ids_map_buffer);
	for (auto x : seed_ids)
	{
		std::cout << x << ' ';
	}
	std::cout << '\n';
	std::cout << *std::min_element(seed_ids.begin(), seed_ids.end()) << '\n';
}
