#pragma once
#include <http_normalizer.hpp>

#include <memory>
#include <string>

// cout << *to_abs("/", "https://dUckduckgo.com", "/s%21%2f/?q=hsad&t=ffab&ia=web") << endl;
// return: https://duckduckgo.com/s!%2F/?ia=web&q=hsad&t=ffab

/*
	for (const auto &s : parts)
		cout << s << endl;

	cout << *to_abs("/", "https://dUckduckgo.com", "/s%21%2f/?q=hsad&t=ffab&ia=web") << endl;
	// http://google.com/sadlife/?ia=web&t=ffab&x=y//s!//%3Fq
	cout << *to_abs("google.com/sadlife/?x=y", "https://dUckduckgo.com", "/s%21%2f/?q=hsad&t=ffab&ia=web") << endl;
*/

// arguments: base_url, current_page_url, href_content
static inline std::shared_ptr<const std::string> to_abs(const std::string &node_base, const std::string &node_url, const std::string &anchor, int HTTP_NORMALIZER_FLAGS = 0) {

    std::string cpy;

    // TODO: prende anche mailto:x@y.com
    static const auto &is_relative = [HTTP_NORMALIZER_FLAGS] (const std::string &s) -> bool {
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