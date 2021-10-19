load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "jukebox_proto",
    srcs = ["jukebox.proto"],
    deps = [
        "@com_google_protobuf//:timestamp_proto",
        "@com_google_googleapis//google/api:annotations_proto",
        "@tagger//:tagger_proto",
    ],
)

cc_proto_library(
    name = "jukebox_cpp_proto",
    deps = [":jukebox_proto"],
)

cc_grpc_library(
    name="jukebox_cpp_grpc",
    srcs=[":jukebox_proto"],
    grpc_only = True,
    deps = [":jukebox_cpp_proto"],
    visibility = ["//visibility:public"],
)