workspace(name = "nes_vis")

load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "git_repository",
    "new_git_repository",
)
load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)

git_repository(
    name = "nes",
    commit = "d2b8a8377631ec94a3b0fb01cab60e1628d7c3d9",
    remote = "https://github.com/evilcorpltd/n_e_s",
    shallow_since = "1651336399 +0200",
)

# Third-party
# =========================================================

http_archive(
    name = "sfml",
    build_file = "@nes_vis//extlibs:BUILD.sfml",
    sha256 = "6124b5fe3d96e7f681f587e2d5b456cd0ec460393dfe46691f1933d6bde0640b",
    strip_prefix = "SFML-2.5.1",
    urls = ["https://github.com/SFML/SFML/archive/2.5.1.zip"],
)

new_git_repository(
    name = "imgui",
    build_file = "@nes_vis//extlibs:BUILD.imgui",
    commit = "377b864bb4a9ed9a25674b04f64f63baf221060d",
    remote = "https://github.com/ocornut/imgui.git",
    shallow_since = "1648052494 +0100",
)

new_git_repository(
    name = "imgui-sfml",
    build_file = "@nes_vis//extlibs:BUILD.imgui-sfml",
    commit = "48d527d0ca444aa546108103cc0ad07b0949bdd6",
    remote = "https://github.com/eliasdaler/imgui-sfml.git",
    shallow_since = "1648849033 +0300",
)

git_repository(
    name = "gtest",
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e",
    remote = "https://github.com/google/googletest",
    shallow_since = "1570114335 -0400",
)

new_git_repository(
    name = "fmtlib",
    build_file = "@nes_vis//extlibs:BUILD.fmtlib",
    commit = "4fe0b11195b7cd71f39253c44db2c9dddf6b82d4",
    remote = "https://github.com/fmtlib/fmt",
    shallow_since = "1603647862 -0700",
)

# Misc tools
# =========================================================

http_archive(
    name = "hedron_compile_commands",
    sha256 = "89cf5a306d25ab14559c95e82d0237638a01eb45e8f4f181304540f97e4d66fe",
    strip_prefix = "bazel-compile-commands-extractor-d3cbc6220320e8d2fce856d8487b45e639e57758",
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/d3cbc6220320e8d2fce856d8487b45e639e57758.tar.gz",
)

load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")

hedron_compile_commands_setup()
