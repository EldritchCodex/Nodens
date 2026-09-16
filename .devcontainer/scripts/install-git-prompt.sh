#!/usr/bin/env bash
#
# Installs git's official bash prompt helper (__git_ps1) into a user's home
# directory, so .bashrc can show the current branch/status in the prompt.
#
# The download is pinned to a specific, signed git release tag rather than
# the "master" branch. This keeps the build reproducible and reviewable:
# the exact file contents fetched here won't silently change if upstream
# pushes new commits, and anyone auditing this devcontainer can diff
# against a known git-prompt.sh revision.
# See: https://github.com/git/git/blob/master/contrib/completion/git-prompt.sh
#
# Intended to run as root during the container image build (see Dockerfile),
# after the target user has already been created.
#
# Usage: install-git-prompt.sh <username>

set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <username>" >&2
    exit 1
fi

username="$1"
git_ref="v2.47.1"
url="https://raw.githubusercontent.com/git/git/${git_ref}/contrib/completion/git-prompt.sh"

user_home="$(getent passwd "${username}" | cut -d: -f6)"
if [[ -z "${user_home}" ]]; then
    echo "error: no such user '${username}'" >&2
    exit 1
fi

dest="${user_home}/.git-prompt.sh"

curl --fail --location --retry 3 --silent --show-error \
    "${url}" \
    --output "${dest}"

# Sanity-check the download before wiring it into anyone's shell: it must
# be non-empty and actually contain the function we expect to source.
test -s "${dest}"
grep --quiet '__git_ps1' "${dest}"

chown "${username}:${username}" "${dest}"
