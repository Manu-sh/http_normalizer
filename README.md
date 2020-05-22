# Http Normalizer

```bash
# installing dependencies for tests
yay -S --noconfirm doctest
mkdir -p build && cd build
cmake ..
make && make test
```

for building the php extension [phpcpp](http://www.php-cpp.com/documentation/install) is required

The trailing slash is always removed.

* //hostname.com => default protocol (per aggiungere sta feature è necessario modificare il tokenizzatore)
https://en.wikipedia.org/wiki/Wikipedia:Protocol-relative_URL.

http_normalizer rely on http_normalizer_parts for a comprensive list of normalization performed see [http_normalizer_parts](http_normalizer/lib/http_normalizer_parts).

###### Copyright © 2020, [Manu-sh](https://github.com/Manu-sh), s3gmentationfault@gmail.com. Released under the [MIT license](LICENSE).
