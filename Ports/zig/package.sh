#!/usr/bin/env -S bash ../.port_include.sh

port='zig'
version='0.15.0-dev.33+539f3effd'
files=(
    'https://github.com/ziglang/zig-bootstrap/archive/11e20c3717ffdc8b2d44aaea24703c434bbdea6c.tar.gz#fa19a22bac6c93ee4f60a3c2f3e4962dd76a2e248cb715764f56df868c61c5d5'
    'https://github.com/ziglang/zig/archive/539f3effd38fac03cacd81338949650297071491.tar.gz#ad48a4837d8bae8aed81b408addad82249ad4db1afe5f2c4ccf06cd92f66e29d'
)

# The actual directory to build in.
workdir='zig-bootstrap-11e20c3717ffdc8b2d44aaea24703c434bbdea6c'
# The newer Zig directory we move into the workdir.
zigdir='zig-539f3effd38fac03cacd81338949650297071491'

post_fetch() {
    # NOTE: Running this multiple times is a massive footgun as patches only get applied once, the
    #       next time we'd end up with a clean copy of the original Zig sources.
    if [ -f .post-fetch-executed ]; then
        return
    fi
    run touch .post-fetch-executed

    # Move the newer version of Zig into the bootstrap
    run rm -rf zig
    run cp -r "../${zigdir}" zig

    # Copy libSystem definitions which are required on macOS, once we set $ZIG_LIBC it will no
    # longer be found in its original place
    run cp zig/lib/libc/darwin/libSystem.tbd "${DESTDIR}/usr/lib/"
}

build() {
    host_env
    cd "${workdir}"
    ./build "${SERENITY_ARCH}-serenity-none" "native"
}

install() {
    zig_install_dir="${workdir}/out/zig-${SERENITY_ARCH}-serenity-none-native"

    mkdir -p "${DESTDIR}/usr/local/bin/."
    mkdir -p "${DESTDIR}/usr/local/lib/."
    cp -rv "${zig_install_dir}/zig" "${DESTDIR}/usr/local/bin/"
    cp -rv "${zig_install_dir}/lib/"* "${DESTDIR}/usr/local/lib/"
}
