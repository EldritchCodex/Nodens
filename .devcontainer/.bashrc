# ~/.bashrc — interactive Bash configuration for the development container.

# Exit early when Bash is running non-interactively, such as from scripts.
[[ $- != *i* ]] && return

# Enable colored output for common directory and text-search commands.
alias ls='ls --color=auto'
alias grep='grep --color=auto'

# Load Git's prompt helper, which provides the __git_ps1 function.
. ~/.git-prompt.sh

# Display the current Git branch or repository state in the shell prompt.
# The prompt also includes the current user, hostname, working directory,
# and a colorized prompt character.
# Resulting prompt string: `user@hostname cwd (git) $`
PROMPT_COMMAND='PS1_CMD1=$(__git_ps1 "(%s) ")'; PS1='\[\e[38;5;82;3m\]\u\[\e[39;2m\]@\h\[\e[0m\] \w \[\e[38;5;87m\]${PS1_CMD1}\[\e[0m\]\\$ '
