# httpgd

[![R build status](https://github.com/nx10/httpgd/workflows/build/badge.svg)](https://github.com/nx10/httpgd/actions)

Asynchronous http server graphics device for R.

## Features

* SVG over HTTP.
* Automatic resizing.
* Plot history.
* HTML live server.
* Stateless API.
* Multiple concurrent device server.
* Multiple concurrent clients for each device.
* Fast plot history navigation (caching).

## Demo

![demo](https://user-images.githubusercontent.com/33600480/83944385-6587fa80-a803-11ea-8f4a-7808d144309d.gif)

## Installation

```R
devtools::install_github("nx10/httpgd")
```

Depends on `Rcpp`, `later` and `gdtools`.

SVG rendering (especially font rendering) based on `svglite` (https://github.com/r-lib/svglite).

Includes `cpp-httplib` (https://github.com/yhirose/cpp-httplib).

## Usage

Initialize graphics device and start live server with:

```R
httpgd::httpgd()
```

Plot what ever you want.

```R
x = seq(0, 3 * pi, by = 0.1)
plot(x, sin(x), type = "l")
```

Every plotting library will work.

```R
library(ggplot2)
ggplot(mpg, aes(displ, hwy, colour = class)) +
  geom_point()
```

The client will refresh automatically and detect window size changes.

Stop the server with:

```R
dev.off()
```

### Keyboard shortcuts

| Keys | Result |
|:----:|--------|
| <kbd>&#8592;</kbd> <kbd>&#8594;</kbd> <kbd>&#8593;</kbd> <kbd>&#8595;</kbd> | Navigate plot history. |
| <kbd>del</kbd> | Remove current plot. |
| <kbd>+</kbd> / <kbd>-</kbd> | Zoom in and out. |
| <kbd>0</kbd> | Reset zoom level. |
| <kbd>ctrl</kbd> + <kbd>S</kbd> | Download the currently visible plot. |


### API

![structure](docs/httpgd_structure.svg)

httpgd can be accessed both from R and from HTTP:

* [R API](docs/RApi.md)
* [HTTP API](docs/HttpApi.md)

### Security

A security token can be set when starting the device: 
```R
httpgd(..., token = "secret")
```
When set, each API request has to include this token inside the header `X-HTTPGD-TOKEN` or as a query param `?token=secret`.
`token` is by default set to `TRUE` to generate a random 8 character alphanumeric token. If it is set to a number, a random token of that length will be generated. `FALSE` deactivates the security token.

CORS is off by default but can be enabled on startup:

```R
httpgd(..., cors = TRUE)
```

## Planned features

* Use websockets to push changes directly/faster.
* Generate optimized (as small as possible) SVGs.
* Render raster graphics.

## Note

Any advice and suggestions are welcome!

## Mac OS issue

~~It seems like a recent mac OS update (Catalina) removed shared libraries used by the package `systemfonts` we depend on (see: https://github.com/r-lib/systemfonts/issues/17).~~

~~You can install XQuartz as a workaround (https://www.xquartz.org/).~~

Users have reported that Apple has released an update that fixes this issue.

## License

This project is licensed GPL v2.0. (Because it currently includes some code parts that are licensed this way. When I get around to rewrite these parts I will consider switching to a more permissive license. This is however very low priority right now. Contact me if you have further questions.)

The html client includes [Material Design icons by Google](https://github.com/google/material-design-icons).
