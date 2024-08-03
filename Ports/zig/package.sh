#!/usr/bin/env -S bash ../.port_include.sh

port='zig'
version='0.14.0-dev.1860+2e2927735'
files=(
    'https://github.com/ziglang/zig-bootstrap/archive/cafebd74b6c664a45989f8dd6fec07a64708df06.tar.gz#d5894ad221cb1bbc925c901c9baef322c6520840a98d651152d2a488dc7f5daf'
    'https://github.com/ziglang/zig/archive/2e2927735d26fc6047343f0c620f20e9048ebaa5.tar.gz#98087f1ad80c915ef01b4a0db4a199f939e6489fe91237ca8849bb56b7e6f79b'
)

# The actual directory to build in.
workdir='zig-bootstrap-cafebd74b6c664a45989f8dd6fec07a64708df06'
# The newer Zig directory we move into the workdir.
zigdir='zig-2e2927735d26fc6047343f0c620f20e9048ebaa5'

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
