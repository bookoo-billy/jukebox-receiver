load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "jukebox_receiver",
    srcs = [
        "receiver.cc",
        "jukebox-receiver.cc",
        "jukebox-receiver.h",
    ],
    visibility = ["//main:__pkg__"],
    deps = ["//lib:jukebox_client"]
)