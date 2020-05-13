#define DOCTEST_CONFIG_IMPLEMENT
#include <http_normalizer.hpp>
#include <http_tokenizer.hpp>
#include <extra.hpp>

#include <doctest/doctest.h>

DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_BEGIN
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
DOCTEST_MAKE_STD_HEADERS_CLEAN_FROM_WARNINGS_ON_WALL_END

// constexpr static int HTTP_NORMALIZER_FLAGS = http_parts::HOSTNAME_STRIP_ALL_PREFIX_WWW | http_parts::PATH_REMOVE_DIRECTORY_INDEX;
constexpr static int HTTP_NORMALIZER_FLAGS = 0;

using namespace std;

TEST_CASE("testing http_tokenizer::normalizer()") {

    // TODO: a single label hostname not allowed

    // REQUIRE(*http_normalizer::normalize("http://hostname:8/path/?x=1&y=2#frag", HTTP_NORMALIZER_FLAGS) == "http://hostname:8/path/?x=1&y=2");

    REQUIRE(*http_normalizer::normalize("http://hostname.x:8/path/?x=1&y=2#frag", HTTP_NORMALIZER_FLAGS) == "http://hostname.x:8/path/?x=1&y=2");
    REQUIRE(*http_normalizer::normalize("hTt%50S://hostname.x:8/path/?x=1&y=2#frag", HTTP_NORMALIZER_FLAGS) == "https://hostname.x:8/path/?x=1&y=2");

    // sort query parameter
    REQUIRE(*http_normalizer::normalize("hTt%50S://hostname.x:8/path/?b=2&a=1#frag", HTTP_NORMALIZER_FLAGS) == "https://hostname.x:8/path/?a=1&b=2");

    // default port removed
    REQUIRE(*http_normalizer::normalize("hTt%50://hostname.x:80/path/?b=2&a=1#frag", HTTP_NORMALIZER_FLAGS)   == "http://hostname.x/path/?a=1&b=2");
    REQUIRE(*http_normalizer::normalize("hTt%50S://hostname.x:443/path/?b=2&a=1#frag", HTTP_NORMALIZER_FLAGS) == "https://hostname.x/path/?a=1&b=2");
    REQUIRE(*http_normalizer::normalize("hTt%50S://hostname.x:80/path/?b=2&a=1#frag", HTTP_NORMALIZER_FLAGS)  == "https://hostname.x:80/path/?a=1&b=2");
    REQUIRE(*http_normalizer::normalize("hTt%50://hostname.x:443/path/?b=2&a=1#frag", HTTP_NORMALIZER_FLAGS)  == "http://hostname.x:443/path/?a=1&b=2");

    // keep fragments
    REQUIRE(*http_normalizer::normalize("hTt%50://hostname.x:443/path/?b=2&a=1#frag", HTTP_NORMALIZER_FLAGS|http_normalizer::KEEP_FRAGMENT)  == "http://hostname.x:443/path/?a=1&b=2#frag");

    REQUIRE(*http_normalizer::normalize("https://www.youtube.com:90?b=x&a=d", HTTP_NORMALIZER_FLAGS)  == "https://www.youtube.com:90/?a=d&b=x");
    REQUIRE(*http_normalizer::normalize("https://www.youtube.com:90/?b=x&a=d", HTTP_NORMALIZER_FLAGS)  == "https://www.youtube.com:90/?a=d&b=x");
    REQUIRE(*http_normalizer::normalize("https://www.youtube.com:90////?b=x&a=d", HTTP_NORMALIZER_FLAGS)  == "https://www.youtube.com:90/?a=d&b=x");
    REQUIRE(*http_normalizer::normalize("https://www.youtube.com:90//x//?b=x&a=d", HTTP_NORMALIZER_FLAGS)  == "https://www.youtube.com:90/x/?a=d&b=x");
    REQUIRE(*http_normalizer::normalize("https://www.youtube.com:90/../x/./?b=x&a=d", HTTP_NORMALIZER_FLAGS)  == "https://www.youtube.com:90/x/?a=d&b=x");
}

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

int main(int argc, char *argv[]) {

    doctest::Context context;
    context.applyCommandLine(argc, argv);

    int res = context.run(); // run doctest

    // important - query flags (and --exit) rely on the user doing this
    if (context.shouldExit()) {
        // propagate the result of the tests
        return res;
    }
}