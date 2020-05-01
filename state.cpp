#include <iostream>
#include <string>
#include <vector>
#include <array>

enum: unsigned char {
	PROTO,
	HOST,
	PORT,
	PATH,
	QUERY,
	FRAG
};


std::vector<std::string> parse(const std::string &http_url) {

	const static std::array<int(*const)(char), FRAG+1> fun {

		[] ([[maybe_unused]] char c) -> int { return 0xdeadbeef; }, // NOP
		[] (char c) -> int {
			switch (c) {
				case ':': return PORT;
				case '/': return PATH;
				case '?': return QUERY;
				case '#': return FRAG;
			}

			return HOST;
		},

		[] (char c) -> int {
			switch (c) {
				case '/': return PATH;
				case '?': return QUERY;
				case '#': return FRAG;
			}

			return PORT;
		},

		[] (char c) -> int {
			switch (c) {
				case '?': return QUERY;
				case '#': return FRAG;
			}

			return PATH;
		},

		[] (char c) -> int { return c != '#' ? QUERY : FRAG; },
		[] ([[maybe_unused]] char c) -> int { return FRAG; } // NOP
	};

	std::vector<std::string> parts(FRAG+1, "");
	int status = HOST;
	size_t istr = 0;

	if (const size_t index = http_url.find("://"); index != std::string::npos) {
		parts.at(PROTO) = http_url.substr(0, index);
		istr = index + 3;
	}
	
	for (const auto len = http_url.length(); istr < len; ++istr) {

		const char c = http_url.at(istr);
		int n_status = fun.at(status)(c);

		if (n_status != status) {
			status = n_status;
			continue;
		}

		parts.at(status).push_back(c);

	}

	return parts;
}

using namespace std;

int main() {

	const string url = "http://hostname:8/path/?x=1&y=2#frag";
	const auto &parts = parse(url);

	for (const auto &s : parts)
		cout << s << endl;

}
