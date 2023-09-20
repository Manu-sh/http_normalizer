# Http Normalizer

```bash
# installing dependencies for tests
yay -S --noconfirm doctest

# clone the project and fetch any submodules
git clone git@github.com:Manu-sh/http_normalizer.git
cd http_normalizer
git submodule update --init --recursive 




# to build only http_normalizer
cd http_normalizer
mkdir -p build && cd build
cmake ..
make -j`nproc --all` && make test

# to build and install the php-extension
cd php-extension
make
sudo make install
```

for building the php extension [phpcpp](http://www.php-cpp.com/documentation/install) is required

The trailing slash is always removed.

* //hostname.com => default protocol (per aggiungere sta feature è necessario modificare il tokenizzatore)
https://en.wikipedia.org/wiki/Wikipedia:Protocol-relative_URL.

http_normalizer rely on http_normalizer_parts **for a comprensive list of normalization performed see** [http_normalizer_parts](http_normalizer/lib/http_normalizer_parts). you can find more examples [here](http_normalizer/examples).

###### Copyright © 2020, [Manu-sh](https://github.com/Manu-sh), s3gmentationfault@gmail.com. Released under the [MIT license](LICENSE).
