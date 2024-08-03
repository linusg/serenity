#!/usr/bin/env -S bash ../.port_include.sh

port='zig'
version='0.14.0-dev.839+a931bfada'
files=(
    'https://github.com/ziglang/zig-bootstrap/archive/e3fe3904af3801454499e3a54c8f8a8c444d3a12.tar.gz#9b2f6b0a26316b4a52af682c34e11c83b4a1b84dbaf611251cedd7cccd713cf1'
    'https://github.com/ziglang/zig/archive/a931bfada5e358ace980b2f8fbc50ce424ced526.tar.gz#37c166df5f7f2f5162daa7538fac9b87480d87eb8d54b74ffd6173463d5006fc'
)

# The actual directory to build in.
workdir='zig-bootstrap-e3fe3904af3801454499e3a54c8f8a8c444d3a12'
# The newer Zig directory we move into the workdir.
zigdir='zig-a931bfada5e358ace980b2f8fbc50ce424ced526'

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
