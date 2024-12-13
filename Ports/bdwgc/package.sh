#!/usr/bin/env -S bash ../.port_include.sh
port='bdwgc'
version='437c08c24537cde1fd54795782f94a575ce5ca87'
files=(
    "https://github.com/ivmai/bdwgc/archive/${version}.tar.gz#3d0794582a956ad009817f4bcd894bea4007d43d1583a7121491fadee572b48a"
)
depends=(
    'libatomic_ops'
)
workdir="bdwgc-${version}"
useconfigure='true'
configopts=(
    '--enable-threads=posix'
)

pre_configure() {
    run ./autogen.sh
}
