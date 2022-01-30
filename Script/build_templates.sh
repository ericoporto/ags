#!/bin/bash

SCRIPT_DIR="$(cd -P "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATES_DIR="${SCRIPT_DIR}/templates"
TEMPLATES_SOURCE_DIR="${TEMPLATES_DIR}/ags-template-source"
AGS_PAK_SOURCE_DIR="${SCRIPT_DIR}/../Tools/agspak/"
AGS_PAK_BIN="${SCRIPT_DIR}/../Tools/agspak/agspak"
CURRENT_BRANCH=`git branch | sed -n 's/^\* //p'`


mkdir -p "${TEMPLATES_DIR}"
cd "${TEMPLATES_DIR}"
git clone --recurse-submodules https://github.com/adventuregamestudio/ags-template-source.git
cd "${TEMPLATES_SOURCE_DIR}"
git checkout CURRENT_BRANCH
cd "${AGS_PAK_SOURCE_DIR}"
echo "Switching to AGS PAK DIR... ${AGS_PAK_SOURCE_DIR}"
make
cd "${TEMPLATES_SOURCE_DIR}"

ls
shopt -s dotglob
shopt -s nullglob
TEMPLATES_ARRAY=(*/)
for dir in "${TEMPLATES_ARRAY[@]}"; do
	if [[ "${dir}" != ".git/" ]]; then
		echo "Packing ${dir}...";
		pushd ${dir}
		${AGS_PAK_BIN} *
		popd
	fi
	done