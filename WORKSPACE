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
    commit = "04ee440699c45129fb21fc0ac374d0374e75003c",
    remote = "https://github.com/evilcorpltd/n_e_s",
    shallow_since = "1648627894 +0200",
)

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
    commit = "958e58b06b35ca845a2e8f5bd6032debd3cec0c0",
    remote = "https://github.com/ocornut/imgui.git",
    shallow_since = "1601551988 +0200",
)

new_git_repository(
    name = "imgui-sfml",
    build_file = "@nes_vis//extlibs:BUILD.imgui-sfml",
    commit = "5ab660a13bc997225587eea88755f22f999f7ba9",
    remote = "https://github.com/eliasdaler/imgui-sfml.git",
    shallow_since = "1598978060 +0300",
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
