#!/usr/bin/env -S bash ../.port_include.sh

port='zig'
version='0.14.0-dev.2050+4adf63aef'
files=(
    'https://github.com/ziglang/zig-bootstrap/archive/cafebd74b6c664a45989f8dd6fec07a64708df06.tar.gz#d5894ad221cb1bbc925c901c9baef322c6520840a98d651152d2a488dc7f5daf'
    'https://github.com/ziglang/zig/archive/4adf63aefc62efb301c27d9f6ec71f92f52c23ca.tar.gz#38725f8632b14d2b9acca85d00be184c828d62eee958fbbcad44a23eaa1ff622'
)

# The actual directory to build in.
workdir='zig-bootstrap-cafebd74b6c664a45989f8dd6fec07a64708df06'
# The newer Zig directory we move into the workdir.
zigdir='zig-4adf63aefc62efb301c27d9f6ec71f92f52c23ca'

post_fetch() {
    # Move the newer version of Zig into the bootstrap
    run rm -rf zig
    run mv "../${zigdir}" zig

    # Copy the scripts that the build process will use
    run mkdir -p out
    run cp -r "${PORT_META_DIR}/scripts" out/
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
