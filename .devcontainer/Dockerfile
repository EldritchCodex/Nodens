# EldritchCodex/Nodens development container.
#
# This image is intentionally split into small, individually reviewable
# shell scripts under scripts/ instead of one long RUN block, so anyone can
# inspect exactly what runs during the build without parsing Dockerfile
# syntax. Each script documents *why* it does what it does.
FROM archlinux:latest

# --- Install build/debug toolchain -----------------------------------------
COPY scripts/install-packages.sh /tmp/install-packages.sh
RUN chmod +x /tmp/install-packages.sh && \
    /tmp/install-packages.sh && \
    rm /tmp/install-packages.sh

# --- Create the non-root development user -----------------------------------
COPY scripts/create-user.sh /tmp/create-user.sh
RUN chmod +x /tmp/create-user.sh && \
    /tmp/create-user.sh eldritchcodex && \
    rm /tmp/create-user.sh

# --- Set up an informative bash prompt (git branch/status) ------------------
COPY scripts/install-git-prompt.sh /tmp/install-git-prompt.sh
RUN chmod +x /tmp/install-git-prompt.sh && \
    /tmp/install-git-prompt.sh eldritchcodex && \
    rm /tmp/install-git-prompt.sh

COPY --chown=eldritchcodex:eldritchcodex .bashrc /home/eldritchcodex/.bashrc

# Switch to the eldritchcodex user
USER eldritchcodex
