#!/usr/bin/env -S bash ../.port_include.sh
port='scummvm'
useconfigure='true'
version='2.9.1'
files=(
    "https://downloads.scummvm.org/frs/scummvm/${version}/scummvm-${version}.tar.xz#6a82f36afa9de758ab1dd377101a26a53f12417cbfd350bb8e5d7fd5b8c257e3"
)
depends=(
    'freetype'
    'libiconv'
    'libjpeg'
    'libmad'
    'libmpeg2'
    'libpng'
    'libtheora'
    'SDL2'
)
configopts=(
    '--disable-seq-midi'
    '--enable-engine=monkey4'
    '--enable-release'
    "--with-sdl-prefix=${SERENITY_INSTALL_ROOT}/usr/local"
)
launcher_name='ScummVM'
launcher_category='&Games'
launcher_command='/usr/local/bin/scummvm'
icon_file='icons/scummvm.ico'

export CPPFLAGS='-fvisibility=hidden'

function post_install() {
    icons_build_dir="${PORT_BUILD_DIR}/scummvm-icons"
    if [ ! -d "${icons_build_dir}" ]; then
        echo 'Downloading & building an icon pack for ScummVM...'

        # Unfortunately, `gen-set.py` prevents us from fixating on a commit ID since it
        # checks whether the git repository is up-to-date. Also, we cannot perform a
        # shallow clone since that will mess up the icon list.
        cd "$(dirname ${icons_build_dir})"
        git clone https://github.com/scummvm/scummvm-icons "$(basename ${icons_build_dir})"
        cd "$(basename ${icons_build_dir})"

        ./gen-set.py 20210825
        cp gui-icons-*.dat "${SERENITY_INSTALL_ROOT}/usr/local/share/scummvm/gui-icons.dat"
    fi
}
