load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "receiver",
    srcs = ["receiver.cc"],
    hdrs = ["receiver.h"],
    visibility = ["//main:__pkg__"],
    deps = [
        "//proto:jukebox_cpp_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ]
)