# Http Normalizer

The trailing slash is always removed.

* //hostname.com => default protocol (per aggiungere sta feature è necessario modificare il tokenizzatore)
https://en.wikipedia.org/wiki/Wikipedia:Protocol-relative_URL.

http_normalizer rely on http_normalizer_parts for a comprensive list of normalization performed see:
[http_normalizer_parts](http_normalizer/lib/http_normalizer_parts).
