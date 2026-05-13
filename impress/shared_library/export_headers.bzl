# A Bazel rule to extract all headers required to use the Impress shared library.
# Normalizes the paths so that external projects only need to include a single
# folder (the extracted directory) to resolve all dependencies natively.

# Mappings to handle specific third-party library structures:
# - jni_md.h: Flattened to sit next to jni.h (which includes it without path).
# - abseil: Strips repo name to match standard 'absl/...' includes.
# - rules_java, zlib, protobuf: Strip up to their specific source/include folders.
# - filament: Duplicated to support both deep includes (used by Impress) and
#   short includes (used by Filament itself and our bridge).
HEADER_MAPPINGS = {
    "jni_md.h": ["jni_md.h"],
    "external/abseil-cpp+/": ["/"],
    "external/rules_java+/include/": ["/"],
    "external/rules_java+/toolchains/include/": ["/"],
    "external/zlib+/zlib/": ["/"],
    "external/protobuf+/src/": ["/"],
    "filament/filament/include/": ["/", "filament/filament/include/"],
    "filament/filament/backend/include/": ["/", "filament/filament/backend/include/"],
    "filament/libs/filabridge/include/": ["/", "filament/libs/filabridge/include/"],
}

def _strip_up_to(parts, component, offset = 1):
    if component in parts:
        idx = parts.index(component)
        return "/".join(parts[idx + offset:])
    return None

def _join_from(parts, index):
    return "/".join(parts[index:])

def _get_archive_path(path):
    parts = path.split("/")

    for key, destinations in HEADER_MAPPINGS.items():
        # Keys with a dot are treated as filenames.
        if "." in key:
            if path.endswith(key):
                return destinations

            # Keys without a dot are treated as directory prefixes.
        elif key in path:
            idx = path.find(key)
            rest = path[idx + len(key):]

            results = []
            for dest in destinations:
                if dest == "/":
                    results.append(rest)
                else:
                    results.append(dest + rest)
            return results

    # Handle Bazel virtual includes. If the target name is duplicated (e.g.
    # .../my_lib/my_lib/header.h), we strip both.
    if "_virtual_includes" in parts:
        idx = parts.index("_virtual_includes")
        if len(parts) > idx + 2 and parts[idx + 1] == parts[idx + 2]:
            return [_join_from(parts, idx + 2)]
        return [_join_from(parts, idx + 1)]

    # Generated files and outputs are placed deep under bazel-out/.../bin/ or .../genfiles/.
    if path.startswith("bazel-out/"):
        for comp in ["bin", "genfiles"]:
            clean_path = _strip_up_to(parts, comp)
            if clean_path != None:
                return [clean_path]

    # External dependencies are placed in external/repo_name/path/to/header.h.
    # We skip the first two parts ('external' and the repo name) to make the path relative to the library.
    if path.startswith("external/"):
        return [_join_from(parts, 2)]

    return [path]

def _export_headers_impl(ctx):
    all_headers = []
    for dep in ctx.attr.deps:
        if CcInfo in dep:
            all_headers.append(dep[CcInfo].compilation_context.headers)

    combined_headers = depset(transitive = all_headers)

    out_dir_name = ctx.label.name
    out_files = []
    seen_paths = {}

    for header_file in combined_headers.to_list():
        paths = _get_archive_path(header_file.path)
        for p in paths:
            if p not in seen_paths:
                seen_paths[p] = True
                out_file = ctx.actions.declare_file(out_dir_name + "/" + p)
                ctx.actions.symlink(output = out_file, target_file = header_file)
                out_files.append(out_file)

    output_zip = ctx.actions.declare_file(ctx.label.name + ".zip")

    if len(out_files) > 0:
        first_file = out_files[0]
        parts = first_file.path.split("/")
        idx = parts.index(out_dir_name)
        dir_path = "/".join(parts[:idx + 1])

        ctx.actions.run_shell(
            inputs = depset(out_files),
            outputs = [output_zip],
            command = "cd {} && zip -r ../{} .".format(dir_path, output_zip.basename),
            use_default_shell_env = True,
        )
    else:
        ctx.actions.write(output = output_zip, content = "")

    return [DefaultInfo(files = depset([output_zip]))]

export_headers = rule(
    implementation = _export_headers_impl,
    attrs = {
        "deps": attr.label_list(providers = [CcInfo]),
    },
)
