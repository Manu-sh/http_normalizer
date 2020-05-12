#pragma once
#include <string>
#include <vector>
#include <array>
#include <cstdint>

// TODO: usare optional?

struct http_tokenizer {

    explicit http_tokenizer() = delete;

    enum parts: uint_fast8_t {
        PROTO,
        HOST,
        PORT,
        PATH,
        QUERY,
        FRAG
    };

    static std::vector<std::string> parse(const std::string &http_url) {

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

};