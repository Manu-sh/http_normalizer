#include <http_normalizer/public/http_normalizer.hpp>
#include <http_normalizer/private/http_tokenizer.hpp>
#include <http_parts/public/http_parts.hpp>

#include <sstream>
#include <iostream>
#include <string_view>
#include <stdexcept>
#include <iterator>


http_normalizer::http_normalizer(const std::string &u, int flags): m_url{std::make_shared<std::string>(u)}, m_flags{flags} {

	using std::string, std::invalid_argument, std::to_string;
	using std::cout, std::endl;

	static const auto &sh_str = [] (std::string &&s) { return s.empty() ? nullptr : std::make_shared<std::string>(s); };
	static const auto URL_MAX = 2048;

	if (m_url->length() > URL_MAX)
		throw invalid_argument("too long");


	const auto &parts = http_tokenizer::parse(*m_url); // TODO dangerous temporaney?
	std::string tmp = parts[http_tokenizer::parts::PROTO];

	if (tmp.empty()) {

		// use http as protocol if no protocol is found but there is a valid hostname
		m_proto = sh_str("http");

	} else {

		if (!(m_proto = sh_str(http_parts::normalize_protocol(tmp))))
			throw invalid_argument("invalid or missing protocol");

		m_is_https = *m_proto == "https";
	}

	if (!(m_hostname = sh_str(http_parts::normalize_hostname(parts[http_tokenizer::parts::HOST], m_flags))))
		throw invalid_argument("invalid or missing hostname");

	if (!(tmp = parts[http_tokenizer::parts::PORT]).empty()) {
		int int_tmp = http_parts::normalize_port(tmp, m_is_https);
		if (int_tmp < 0) throw invalid_argument("invalid port");
		m_port = int_tmp > 0 ? sh_str(to_string(int_tmp)) : m_port;
	}

	if (!(tmp = parts[http_tokenizer::parts::PATH]).empty())
		m_path  = sh_str( http_parts::normalize_path(tmp, m_flags) );

	if (!(tmp = parts[http_tokenizer::parts::QUERY]).empty())
		m_query = sh_str( http_parts::normalize_query(tmp) );

	// fragments usually are ignored
	if (!(tmp = parts[http_tokenizer::parts::FRAG]).empty())
		m_fragment = sh_str( std::move(tmp) );
}

/* get only the normalized url */
std::shared_ptr<const std::string> http_normalizer::normalized() const {

	using std::literals::string_view_literals::operator""sv;
	static const auto &sh_str = [] (std::string &&s) { return s.empty() ? nullptr : std::make_shared<std::string>(s); };

	static const std::initializer_list<std::pair<std::string_view, decltype(&http_normalizer::proto)>> pairs{
		{""sv,    &http_normalizer::proto}, /* string to prepend, function to call */
		{"://"sv, &http_normalizer::hostname}, 
		{":"sv,   &http_normalizer::port},
		{"/"sv,   &http_normalizer::path},
		{"/?"sv,  &http_normalizer::query}
		/* fragments are ignored */
	};

	// i know this is ugly but i have not so much time
	static const std::initializer_list<std::pair<std::string_view, decltype(&http_normalizer::proto)>> pairs_keep_frag{
		{""sv,    &http_normalizer::proto}, /* string to prepend, function to call */
		{"://"sv, &http_normalizer::hostname}, 
		{":"sv,   &http_normalizer::port},
		{"/"sv,   &http_normalizer::path},
		{"/?"sv,  &http_normalizer::query},
		{"#"sv,   &http_normalizer::fragment}
	};
	
	if (m_normalized != nullptr)
		return m_normalized;

	std::ostringstream ret;

	try {
		for (const auto &pair : !(m_flags & KEEP_FRAGMENT) ? pairs : pairs_keep_frag)
			if (auto cb = pair.second; (*this.*cb)() != nullptr) 
				ret << pair.first << *(*this.*cb)();
	} catch (...) { return nullptr; }

	return (m_normalized = sh_str( ret.str() ));
}

std::shared_ptr<const std::string> http_normalizer::normalize(const std::string &u, int flags) noexcept try {
	http_normalizer ht{u, flags};
	return ht.normalized();
} catch (...) {
	return nullptr;
}

/* get all infos */
std::string http_normalizer::dbg_info() const {

	static const std::initializer_list<std::pair<std::string_view, decltype(&http_normalizer::proto)>> pairs{
		 {"scheme:   ", &http_normalizer::proto}, 
		 {"hostname: ", &http_normalizer::hostname}, 
		 {"port:     ", &http_normalizer::port},
		 {"path:     ", &http_normalizer::path}, 
		 {"query:    ", &http_normalizer::query},
		 {"fragment: ", &http_normalizer::fragment}
	};

	std::ostringstream os; 
	for (const auto &pair : pairs)
		if (const auto &cb = pair.second; (*this.*cb)() )
			os << "function (" << &cb << ") " 
				<< pair.first << *(*this.*cb)() << '\n';

	return (os << "normalized -> " << *this->normalized() << '\n'), os.str();
}
