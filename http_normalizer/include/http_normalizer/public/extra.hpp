#pragma once
#include <http_normalizer/public/http_normalizer.hpp>

#include <memory>
#include <string>

/*
    [[ Attention this function is just a sample and isn't intend to be used ]]

    For example calling this function on the following situations

    <!-- page_url: https://dUckduckgo.com -->
    <base href="/">
    <a href="/s%21%2f/?q=hsad&t=ffab&ia=web"></a>

    // to_abs("/", "https://dUckduckgo.com", "/s%21%2f/?q=hsad&t=ffab&ia=web")
    // return: https://duckduckgo.com/s!%2F/?ia=web&q=hsad&t=ffab


    <!-- page_url: https://dUckduckgo.com -->
    <base href="google.com/sadlife/?x=y">
    <a href="/s%21%2f/?q=hsad&t=ffab&ia=web"></a>

    // to_abs("google.com/sadlife/?x=y", "https://dUckduckgo.com", "/s%21%2f/?q=hsad&t=ffab&ia=web")
    // return http://google.com/sadlife/?ia=web&t=ffab&x=y//s!//%3Fq

*/

static inline std::shared_ptr<const std::string> to_abs(const std::string &html_base_url, const std::string &origin_url, const std::string &href_content, int HTTP_NORMALIZER_FLAGS = 0) {

    std::string cpy;

    // prende anche mailto:x@y.com fixed?
    static const auto &is_relative = [HTTP_NORMALIZER_FLAGS] (const std::string &s) -> bool {
        return s[0] == '/' || s[0] == '.'; // || http_normalizer::normalize(s, HTTP_NORMALIZER_FLAGS) == nullptr;
    };

    if (is_relative(href_content)) {
        try { http_normalizer n{origin_url, HTTP_NORMALIZER_FLAGS}; cpy = is_relative(html_base_url) ? *n.proto() + "://" + *n.hostname() + '/' + html_base_url + '/' + href_content : html_base_url + '/' + href_content; }
        catch (...) { return nullptr; }
    } else {
        cpy = href_content;
    }

    auto shp = http_normalizer::normalize(cpy, HTTP_NORMALIZER_FLAGS);
    if (!(shp = shp ? shp : http_normalizer::normalize( html_base_url + "/" + cpy, HTTP_NORMALIZER_FLAGS)))
        return nullptr;

    return shp;
}
