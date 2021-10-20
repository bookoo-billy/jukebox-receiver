load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "jukebox_client",
    srcs = ["jukebox-grpc.cc"],
    hdrs = ["jukebox-grpc.h"],
    visibility = ["//main:__pkg__"],
    deps = [
        "//proto:jukebox_cpp_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ]
)