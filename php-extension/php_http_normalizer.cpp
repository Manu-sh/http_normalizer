#include <http_normalizer/public/http_normalizer.hpp>
#include <http_normalizer/public/extra.hpp>

#include <phpcpp.h>

// constexpr static int HTTP_NORMALIZER_FLAGS = http_parts::OPT::HOSTNAME_STRIP_ALL_PREFIX_WWW | http_parts::OPT::PATH_REMOVE_DIRECTORY_INDEX;
constexpr static int HTTP_NORMALIZER_FLAGS = 0;

Php::Value http_normalize_to_abs(Php::Parameters &params) {
    Php::Value node_base_str = params[0];
    Php::Value node_url_str = params[1];
    Php::Value node_anchor_str = params[2];

    const auto &shp = to_abs(node_base_str, node_url_str, node_anchor_str, ::HTTP_NORMALIZER_FLAGS);
    return shp ? *shp : "";
}

Php::Value http_normalize(Php::Parameters &params) {
	Php::Value string_val = params[0];
    const auto &shp = http_normalizer::normalize(string_val);
	return shp ? *shp : "";
}

extern "C" {

    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("libphp_http_normalizer", "1.0");
        
        extension.add<http_normalize_to_abs>("http_normalize_to_abs", {
            Php::ByVal("input_part_0", Php::Type::String, true),
            Php::ByVal("input_part_1", Php::Type::String, true),
            Php::ByVal("input_part_2", Php::Type::String, true)
        });

        extension.add<http_normalize>("http_normalize", {
            Php::ByVal("input_part", Php::Type::String, true)
        });

        return extension;
    }

}
