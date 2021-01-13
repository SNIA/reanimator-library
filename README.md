strace2ds-library - a bridge between strace and DataSeries trace format
=======================================================================

This library connects the [strace] code (written in C) to the [DataSeries] API
(written in C++). It acts as a glue so that C++ functions are made callable from
C code. Our fork of strace, [reanimator-strace], takes advantage of this library to
output traces in the DataSeries format.

Dependencies
------------

Currently, only Ubuntu 16 and 18 are officially supported.

- [Lintel] - general utility library for DataSeries
- [DataSeries] - data format for structured serial data
- [tcmalloc] - high-performance, multi-threaded `malloc()` implementation
- libtool
- libboost-dev (v1.58 only)
- libboost-thread-dev (v1.58 only)
- libboost-program-options-dev (v1.58 only)
- build-essential
- libxml2-dev
- zlib1g-dev

Build Instructions
------------------

Install the following required programs and libraries. On Ubuntu 16 and 18, all the above requirements are available through the APT package manager.:

```plaintext
git cmake perl autoconf automake gcc g++ libtool libboost-dev libboost-thread-dev libboost-program-options-dev build-essential libxml2-dev zlib1g-dev
```

### Automated Build

After installing the required programs and libraries, run `build-reanimator-library.sh` to automatically build strace2ds. All arguments are optional:

| Argument             | Description                                                    |
| -------------------- | -------------------------------------------------------------- |
| `--install`          | Install libraries and binaries under `/usr/local`              |
| `--install-dir DIR`  | Install libraries and binaries to DIR (ignored if `--install`) |
| `--config-args ARGS` | Append ARGS to every `./configure` command                     |
| `-h, --help`         | Display a help message                                         |


### Manual Build

Manually building strace2ds closely resembles the steps taken in `build-reanimator-library.sh`.

1. Install [Lintel] by running cmake . && make && make install at the root of the Lintel repository.

1. Install [DataSeries] by running cmake . && make && make install at the root of the DataSeries repository.

1. Install [tcmalloc] from the gperftools repository. See the gperftools [`INSTALL`](https://github.com/gperftools/gperftools/blob/master/INSTALL) file for detailed instructions.

1. Navigate to the `tables/` subdirectory and run `perl gen-xml-enums.pl` to update the .xml files.

1. Create a subdirectory named `build` in the strace2ds-library directory and navigate to it. Run `cp -r ../xml ./` to copy the xml folder into the build directory.

1. `cd` into your build directory and run `cmake -DCMAKE_INSTALL_PREFIX:PATH=<Install Here> ../`. Specifying a custom install directory for `make install` via `-DCMAKE_INSTALL_PREFIX:PATH=<Install Here>` is optional.

1. Run `make` and `make install` to build and install strace2ds.

[strace]: https://strace.io
[DataSeries]: https://github.com/dataseries/dataseries
[reanimator-strace]: https://github.com/SNIA/reanimator-strace
[Lintel]: https://github.com/dataseries/lintel
[tcmalloc]: https://github.com/gperftools/gperftools
