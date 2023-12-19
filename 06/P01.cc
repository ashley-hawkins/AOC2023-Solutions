#include <algorithm>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

struct record
{
	std::int64_t time;
	std::int64_t distance;
};

std::optional<std::pair<double, double>> get_quadratic_roots(double a, double b, double c)
{
	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		return std::nullopt;
	}
	if (discriminant == 0)
	{
		auto res = -b / (2 * a);
		return std::pair{res, res};
	}

	auto sqrt_discriminant = std::sqrt(discriminant);
	return std::pair{(-b + sqrt_discriminant) / (2 * a), (-b - sqrt_discriminant) / (2 * a)};
}

int main()
{
	std::vector<record> records;

	{
		std::string line;
		{
			if (!std::getline(std::cin, line))
			{
				return EXIT_FAILURE;
			}
			auto colon_idx = line.find(':');

#ifdef PART_2
			auto times = line.substr(colon_idx + 1);
			std::erase_if(times, [](auto c) { return std::isspace(c, std::locale::classic()); });
#else
			auto times = std::string_view{line}.substr(colon_idx + 1);
#endif

			std::istringstream iss{std::string{times}};
			for (std::int64_t time{}; iss >> time;)
			{
				records.push_back({time, 0});
			}
		}
		{
			if (!std::getline(std::cin, line))
			{
				return EXIT_FAILURE;
			}
			auto colon_idx = line.find(':');

#ifdef PART_2
			auto distances = line.substr(colon_idx + 1);
			std::erase_if(distances, [](auto c) { return std::isspace(c, std::locale::classic()); });
#else

			auto distances = std::string_view{line}.substr(colon_idx + 1);
#endif
			std::istringstream iss{std::string{distances}};
			{
				std::size_t i{};
				for (std::int64_t distance{}; iss >> distance;)
				{
					records.at(i).distance = distance;
					++i;
				}
			}
		}
	}

	std::int64_t tally{1};
	for (auto [time, distance] : records)
	{
		// To find inputs that exceed the record `distance` D in the allotted `time` T:
		// -i^2 + Ti - D > 0
		// So:
		// a = -1
		// b = T
		// c = -D
		auto roots = get_quadratic_roots(-1, time, -distance);
		if (roots)
		{
			auto [min, max] = *roots;
			// Get the count of integers in the interval (min, max)
			auto count = std::max(std::int64_t{0}, std::int64_t(std::ceil(max)) - std::int64_t(std::floor(min)) - 1);
			tally *= count;
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
	std::cout << tally << '\n';
}
