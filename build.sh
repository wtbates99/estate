#!/usr/bin/env bash
set -euo pipefail

project_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
build_dir="${project_dir}/build"

cmake -S "${project_dir}" -B "${build_dir}"
cmake --build "${build_dir}" --parallel

cp "${project_dir}/assets/ARIAL.TTF" "${build_dir}/"

cd "${build_dir}"
exec ./estate
