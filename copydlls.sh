#!/usr/bin/env bash
set -euo pipefail

MINGW64_PATH="/opt/qt/6.8.0/mingw_64"
TOOLS_PATH="/opt/Tools/mingw1310_64/bin"
LIBS_PATH="$MINGW64_PATH/bin"
PLUGINS_PATH="$MINGW64_PATH/plugins"
RELEASE_PATH="${1:?Usage: $0 <release_dir>}"

mkdir -p "$RELEASE_PATH"

cp "$LIBS_PATH/Qt6Core.dll" "$RELEASE_PATH/"
cp "$LIBS_PATH/Qt6Gui.dll" "$RELEASE_PATH/"
cp "$LIBS_PATH/Qt6Widgets.dll" "$RELEASE_PATH/"

cp "$PLUGINS_PATH/platforms/qwindows.dll" "$RELEASE_PATH/"

if [ -f "$PLUGINS_PATH/styles/qmodernwindowsstyle.dll" ]; then
  cp "$PLUGINS_PATH/styles/qmodernwindowsstyle.dll" "$RELEASE_PATH/"
fi

cp "$TOOLS_PATH/libgcc_s_seh-1.dll" "$RELEASE_PATH/"
cp "$TOOLS_PATH/libstdc++-6.dll" "$RELEASE_PATH/"