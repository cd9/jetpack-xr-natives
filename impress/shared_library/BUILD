load("@rules_cc//cc:cc_library.bzl", "cc_library")
load(":export_headers.bzl", "export_headers")
load("@rules_cc//cc:cc_binary.bzl", "cc_binary")


package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "impress",
    linkshared = 1,
    linkopts = select({
        "@com_google_impress//core:android": [
            "-lGLESv3",
            "-lEGL",
            "-landroid",
            "-lnativewindow",
        ],
        "//conditions:default": [],
    }),
    deps = [
        "//:api",
        "//:impl_full",
        "@third_party//filament:filament",
    ],
)

export_headers(
    name = "impress_headers",
    deps = [
        "//:api",
        "//:impl_full",
        "@third_party//filament:filament",
    ],
)
