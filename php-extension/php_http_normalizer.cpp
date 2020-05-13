#include <http_normalizer/public/http_normalizer.hpp>
#include <phpcpp.h>

// constexpr static int HTTP_NORMALIZER_FLAGS = http_parts::OPT::HOSTNAME_STRIP_ALL_PREFIX_WWW | http_parts::OPT::PATH_REMOVE_DIRECTORY_INDEX;
constexpr static int HTTP_NORMALIZER_FLAGS = 0;

Php::Value http_normalize(Php::Parameters &params) {
	Php::Value string_val = params[0];
    const auto &shp = http_normalizer::normalize(string_val);
	return shp ? *shp : "";
}

extern "C" {
    PHPCPP_EXPORT void *get_module() {
        static Php::Extension extension("libphp_http_normalizer", "1.0");
        
        extension.add<http_normalize>("http_normalize", {
            Php::ByVal("input_part", Php::Type::String, true)
        });

        return extension;
    }
}
