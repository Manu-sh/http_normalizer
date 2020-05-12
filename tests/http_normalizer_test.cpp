#define DOCTEST_CONFIG_IMPLEMENT

#include <http_normalizer.hpp>
#include <http_tokenizer.hpp>

#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END


// constexpr static int HTTP_NORMALIZER_FLAGS = http_parts::OPT::HOSTNAME_STRIP_ALL_PREFIX_WWW | http_parts::OPT::PATH_REMOVE_DIRECTORY_INDEX;
constexpr static int HTTP_NORMALIZER_FLAGS = 0;

 // cout << *to_abs("/", "https://dUckduckgo.com", "/s%21%2f/?q=hsad&t=ffab&ia=web") << endl;
 // return: https://duckduckgo.com/s!%2F/?ia=web&q=hsad&t=ffab

// arguments: base_url, current_page_url, href_content
static inline std::shared_ptr<const std::string> to_abs(const std::string &node_base, const std::string &node_url, const std::string &anchor) {

    std::string cpy;

    // TODO: prende anche mailto:x@y.com
    static const auto &is_relative = [] (const std::string &s) -> bool {
        return s[0] == '/' || s[0] == '.' || http_normalizer::normalize(s, HTTP_NORMALIZER_FLAGS) == nullptr;
    };

    if (is_relative(anchor)) {
        try { http_normalizer n{node_url, HTTP_NORMALIZER_FLAGS}; cpy = is_relative(node_base) ? *n.proto() + "://" + *n.hostname() + '/' + node_base + '/' + anchor : node_base + '/' + anchor; }
        catch (...) { return nullptr; }
    } else {
        cpy = anchor;
    }

    auto shp = http_normalizer::normalize(cpy, HTTP_NORMALIZER_FLAGS);
    if (!(shp = shp ? shp : http_normalizer::normalize( node_base + "/" + cpy, HTTP_NORMALIZER_FLAGS)))
        return nullptr;

    return shp;
}

using namespace std;

TEST_CASE("testing http_tokenizer::parse()") {

    {
        const auto parts = http_tokenizer::parse("http://hostname:8/path/?x=1&y=2#frag");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "8");
        REQUIRE(parts[http_tokenizer::PATH]  == "path/");
        REQUIRE(parts[http_tokenizer::QUERY] == "x=1&y=2");
        REQUIRE(parts[http_tokenizer::FRAG]  == "frag");
    }

     {
        const auto parts = http_tokenizer::parse("://hostname:8/path/?x=1&y=2#frag");
        REQUIRE(parts[http_tokenizer::PROTO] == "");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "8");
        REQUIRE(parts[http_tokenizer::PATH]  == "path/");
        REQUIRE(parts[http_tokenizer::QUERY] == "x=1&y=2");
        REQUIRE(parts[http_tokenizer::FRAG]  == "frag");
    }

    {
        const auto parts = http_tokenizer::parse("//hostname:8/path/?x=1&y=2#frag");
        REQUIRE(parts[http_tokenizer::PROTO] == "");
        REQUIRE(parts[http_tokenizer::HOST]  == "");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "/hostname:8/path/");
        REQUIRE(parts[http_tokenizer::QUERY] == "x=1&y=2");
        REQUIRE(parts[http_tokenizer::FRAG]  == "frag");
    }

    {
        const auto parts = http_tokenizer::parse("http://hostname/path/?x=1&y=2#frag");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "path/");
        REQUIRE(parts[http_tokenizer::QUERY] == "x=1&y=2");
        REQUIRE(parts[http_tokenizer::FRAG]  == "frag");
    }

    {
        const auto parts = http_tokenizer::parse("http://hostname/?x=1&y=2#frag");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "");
        REQUIRE(parts[http_tokenizer::QUERY] == "x=1&y=2");
        REQUIRE(parts[http_tokenizer::FRAG]  == "frag");
    }

    {
        const auto parts = http_tokenizer::parse("http://hostname?x=1&y=2#frag");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "");
        REQUIRE(parts[http_tokenizer::QUERY] == "x=1&y=2");
        REQUIRE(parts[http_tokenizer::FRAG]  == "frag");
    }

    {
        const auto parts = http_tokenizer::parse("http://hostname/path/#frag");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "path/");
        REQUIRE(parts[http_tokenizer::QUERY] == "");
        REQUIRE(parts[http_tokenizer::FRAG]  == "frag");
    }

    {
        const auto parts = http_tokenizer::parse("http://hostname/path/");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "path/");
        REQUIRE(parts[http_tokenizer::QUERY] == "");
        REQUIRE(parts[http_tokenizer::FRAG]  == "");
    }

    {
        const auto parts = http_tokenizer::parse("http://hostname/path");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "path");
        REQUIRE(parts[http_tokenizer::QUERY] == "");
        REQUIRE(parts[http_tokenizer::FRAG]  == "");
    }

    {
        const auto parts = http_tokenizer::parse("http://hostname#frag");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "");
        REQUIRE(parts[http_tokenizer::QUERY] == "");
        REQUIRE(parts[http_tokenizer::FRAG]  == "frag");
    }

    {
        const auto parts = http_tokenizer::parse("http://hostname");
        REQUIRE(parts[http_tokenizer::PROTO] == "http");
        REQUIRE(parts[http_tokenizer::HOST]  == "hostname");
        REQUIRE(parts[http_tokenizer::PORT]  == "");
        REQUIRE(parts[http_tokenizer::PATH]  == "");
        REQUIRE(parts[http_tokenizer::QUERY] == "");
        REQUIRE(parts[http_tokenizer::FRAG]  == "");
    }

}

/*
int main() {
    doctest::Context context;
	const string url = "http://hostname:8/path/?x=1&y=2#frag";
	const auto &parts = http_tokenizer::parse(url);
}*/

int main(int argc, char **argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);

    int res = context.run(); // run doctest

    // important - query flags (and --exit) rely on the user doing this
    if (context.shouldExit()) {
        // propagate the result of the tests
        return res;
    }
}



/*
	for (const auto &s : parts)
		cout << s << endl;

	cout << *to_abs("/", "https://dUckduckgo.com", "/s%21%2f/?q=hsad&t=ffab&ia=web") << endl;
	// http://google.com/sadlife/?ia=web&t=ffab&x=y//s!//%3Fq
	cout << *to_abs("google.com/sadlife/?x=y", "https://dUckduckgo.com", "/s%21%2f/?q=hsad&t=ffab&ia=web") << endl;
*/