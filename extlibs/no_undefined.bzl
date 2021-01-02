"""Overrides cc_library to generate an empty binary in addition to the library.

This to ensure completeness of linkopts and dependencies for libraries. The
empty binary will fail to link if there are any undefined references.
"""

load("@rules_cc//cc:defs.bzl", "cc_binary", _cc_library = "cc_library")

def cc_library(name,
               hdrs = [],
               srcs = [],
               linkopts = [],
               copts = [],
               deps = [],
               visibility = None,
               strip_include_prefix = None,
               **kwargs):
    _cc_library(
        name = name,
        srcs = srcs,
        hdrs = hdrs,
        linkopts = linkopts,
        copts = copts,
        deps = deps,
        visibility = visibility,
        strip_include_prefix = strip_include_prefix,
        **kwargs
    )
    native.genrule(
        name = "%s_empty_main" % name,
        outs = ["%s_empty_main.c" % name],
        cmd = "echo 'int main(void) { return 0; }' > $@",
    )
    _cc_library(
        name = "%s_lib" % name,
        hdrs = hdrs,
        linkopts = linkopts,
        copts = copts,
        visibility = visibility,
        strip_include_prefix = strip_include_prefix,
        deps = deps,
    )
    cc_binary(
        name = "%s_bin" % name,
        srcs = srcs + [":%s_empty_main.c" % name],
        linkstatic = True,
        linkopts = linkopts,
        copts = copts,
        deps = ["%s_lib" % name],
        **kwargs
    )
