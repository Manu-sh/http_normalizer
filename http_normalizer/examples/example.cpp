#include <http_normalizer/public/http_normalizer.hpp>
#include <iostream>

using namespace std;
int main() {
    const auto &shp = http_normalizer::normalize("https://www.youtube.com:90/../x/./?b=x&a=d", 0);
    cout << *shp << endl;
}