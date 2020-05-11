#include <http_parts/public/http_parts.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>

enum: uint_fast8_t {
	PROTO,
	HOST,
	PORT,
	PATH,
	QUERY,
	FRAG
};


std::vector<std::string> parse(const std::string &http_url) {

	using state_t = uint_fast8_t;

	const static std::array<state_t(*const)(char), FRAG+1> fun {

		[] ([[maybe_unused]] char c) -> state_t { return 0xff; }, // NOP
		[] (char c) -> state_t {
			switch (c) {
				case ':': return PORT;
				case '/': return PATH;
				case '?': return QUERY;
				case '#': return FRAG;
			}

			return HOST;
		},

		[] (char c) -> state_t {
			switch (c) {
				case '/': return PATH;
				case '?': return QUERY;
				case '#': return FRAG;
			}

			return PORT;
		},

		[] (char c) -> state_t {
			switch (c) {
				case '?': return QUERY;
				case '#': return FRAG;
			}

			return PATH;
		},

		[] (char c) -> state_t { return c != '#' ? QUERY : FRAG; },
		[] ([[maybe_unused]] char c) -> state_t { return FRAG; } // NOP
	};

	std::vector<std::string> parts(FRAG+1, "");
	size_t istr = 0;

	if (const size_t index = http_url.find("://"); index != std::string::npos) {
		parts.at(PROTO) = http_url.substr(0, index);
		istr = index + 3;
	}

	state_t status = HOST;
	for (const auto len = http_url.length(); istr < len; ++istr) {

		const char c = http_url.at(istr);
		const state_t n_status = fun.at(status)(c);

		if (n_status != status) {
			status = n_status;
			continue;
		}

		parts.at(status).push_back(c);

	}

	return parts;
}

// const static std::array<state_t(*const)(char), FRAG+1> fun;


// std::string normalize(const std::vector<std::string> &parts, std::vector<std::function<std::string(const std::string &p)>> &norms);

using namespace std;

int main() {

	const string url = "http://hostname:8/path/?x=1&y=2#frag";
	const auto &parts = parse(url);

	for (const auto &s : parts)
		cout << s << endl;

	// apply_if(parts, functions);
}
