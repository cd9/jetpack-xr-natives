

fn main() {
    let mut build = cxx_build::bridge("src/main.rs");
    build.file("src/sample_view.cc");
    build.file("src/rust_bridge.cc");
    build.file("src/spin_component.cc");
    build.compiler("clang++");
    build.flag("-w");

    // Required config macros for Impress headers
    build.define("IMP_RUNTIME_CONFIG", "SHIP");
    build.define("IMP_MATERIAL_API_CONFIG", "OPENGL");

    build.include("../impress_headers");
    build.include("../impress_headers/external/rules_java+/toolchains/include");

    // Rust <-> C++ bridge.
    build.include("src");

    build.compile("rust_demo_bridge");

    println!("cargo:rustc-link-search=native=../../bazel-bin/shared_library");
    println!("cargo:rustc-link-lib=dylib=impress");
    println!("cargo:rustc-link-lib=dylib=EGL");
    println!("cargo:rustc-link-lib=dylib=GL");

    println!("cargo:rerun-if-changed=src/main.rs");
    println!("cargo:rerun-if-changed=src/bridge.cc");
    println!("cargo:rerun-if-changed=src/sample_view.cc");
    println!("cargo:rerun-if-changed=src/sample_view.h");
}
