#
# ~/.bashrc
#

# If not running interactively, don't do anything
case "$SHELL" in *bash*) ;; *) return ;; esac
[[ $- != *i* ]] && return

[ -z $ENVSET ] && if [ -f ~/.rc ]; then
	. ~/.rc
fi

set -o vi
bind -m vi-insert "\C-l":clear-screen
bind '"\C-k":"gg\n"'
bind '"\C-b":"bb\n"'
bind -x '"\C-f":"f"'
bind -x '"\C-n":"l"'

# Eternal bash history.
export HISTTIMEFORMAT="[%F %T] "
export HISTFILE=~/.bhist
export HISTCONTROL=ignoredups
PROMPT_COMMAND="history -a; $PROMPT_COMMAND"
stty -ixon &> /dev/null

# Case-insensitive globbing (used in pathname expansion)
shopt -s nocaseglob;
# Correct spelling errors in arguments supplied to cd
shopt -s cdspell;
# Autocorrect on directory names to match a glob.
shopt -s dirspell 2> /dev/null
# Turn on recursive globbing (enables ** to recurse all directories)
shopt -s globstar 2> /dev/null
