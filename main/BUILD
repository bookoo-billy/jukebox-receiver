load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    visibility = ["//main:__pkg__"],
    deps = [
        "//receiver:receiver",
    ]
)