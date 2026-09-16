#!/usr/bin/env bash
#
# Creates the non-root development user the container runs as, and grants
# it passwordless sudo *inside the container only*.
#
# Passwordless sudo is safe here because:
#   - This account only exists inside an ephemeral, disposable container.
#   - The container has no other users or secrets to protect from itself.
#   - It avoids being prompted for a password for routine tasks (installing
#     a missing package, editing a system file) while developing.
# This is not a pattern you should reuse for a shared host or production
# image.
#
# Intended to run as root during the container image build (see Dockerfile).
#
# Usage: create-user.sh <username>

set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <username>" >&2
    exit 1
fi

username="$1"

useradd --create-home --shell /bin/bash "${username}"

sudoers_file="/etc/sudoers.d/${username}"
printf '%s\n' "${username} ALL=(ALL) NOPASSWD:ALL" > "${sudoers_file}"
chmod 0440 "${sudoers_file}"

# Validate only the file we just wrote. If it's malformed, fail the build
# loudly instead of leaving behind a broken/ignored sudoers drop-in.
visudo --check --file "${sudoers_file}"
