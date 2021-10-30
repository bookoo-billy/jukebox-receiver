load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

http_archive(
    name = "rules_proto",
    strip_prefix = "rules_proto-4.0.0",
    urls = [
        "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0.tar.gz",
    ],
    sha256 = "66bfdf8782796239d3875d37e7de19b1d94301e8972b3cbd2446b332429b4df1",
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

http_archive(
    name = "com_github_grpc_grpc",
    urls = [
        "https://github.com/grpc/grpc/archive/refs/tags/v1.41.0.tar.gz"
    ],
    sha256 = "e5fb30aae1fa1cffa4ce00aa0bbfab908c0b899fcf0bbc30e268367d660d8656",
    strip_prefix = "grpc-1.41.0",
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()

GOOGLEAPIS_GIT_SHA = "052b274138fce2be80f97b6dcb83ab343c7c8812"
GOOGLEAPIS_SHA = "e31dc9f889bf53e001998d16385881b507c8cc1455bbe5618b16f0f8cb0fd46f"

http_archive(
    name = "com_google_googleapis",
    sha256 = GOOGLEAPIS_SHA,
    strip_prefix = "googleapis-" + GOOGLEAPIS_GIT_SHA,
    urls = ["https://github.com/googleapis/googleapis/archive/" + GOOGLEAPIS_GIT_SHA + ".tar.gz"],
)

load("@com_google_googleapis//:repository_rules.bzl", "switched_rules_by_language")

switched_rules_by_language(
    name = "com_google_googleapis_imports",
)

http_archive(
    name = "com_google_protobuf",
    sha256 = "9111bf0b542b631165fadbd80aa60e7fb25b25311c532139ed2089d76ddf6dd7",
    strip_prefix = "protobuf-3.18.1",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.18.1.tar.gz"],
)

TAGGER_WORKSPACE_CONTENT="""
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_protobuf",
    sha256 = "9111bf0b542b631165fadbd80aa60e7fb25b25311c532139ed2089d76ddf6dd7",
    strip_prefix = "protobuf-3.18.1",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.18.1.tar.gz"],
)
"""

TAGGER_BUILD_CONTENT="""
proto_library(
    name="tagger_proto",
    srcs=["tagger/tagger.proto"],
    visibility=["//visibility:public"],
    deps = [
        "@com_google_protobuf//:descriptor_proto",
    ]
)
"""

new_git_repository(
    name = "tagger",
    remote = "https://github.com/srikrsna/protoc-gen-gotag.git",
    commit = "329b36eb1ae8a363fc26f4b64c8f38c3c43f9b81",
    shallow_since = "1631821832 +0530",
    build_file_content = TAGGER_BUILD_CONTENT,
    workspace_file_content = TAGGER_WORKSPACE_CONTENT,
)

SDL2_BUILD_CONTENT="""
cc_library(
    name = "sdl2",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    linkopts = ["/LIBPATH C:/SDL2/lib/x64/SDL2.lib"],
    visibility = ["//visibility:public"],
)
"""

http_archive(
    name = "sdl2",
    build_file_content = SDL2_BUILD_CONTENT,
    sha256 = "e6a7c71154c3001e318ba7ed4b98582de72ff970aca05abc9f45f7cbdc9088cb",
    strip_prefix = "SDL2-2.0.8",
    urls = ["https://www.libsdl.org/release/SDL2-2.0.8.zip"],
)