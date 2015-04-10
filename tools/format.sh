export PATH=$PATH:$HOME/Library/Application\ Support/Developer/Shared/Xcode/Plug-ins/ClangFormat.xcplugin/Contents/Resources

script_dir="$(cd "$(dirname "${BASH_SOURCE:-$0}")"; pwd)"

clang-format -version
clang-format -i -style=Google $script_dir/../include/arpp/*.h
clang-format -i -style=Google $script_dir/../src/*.cpp
echo "Done reformat"
