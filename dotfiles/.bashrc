#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

[ -z $ENVSET ] && if [ -f ~/.profile ]; then
	. ~/.profile
fi

set -o vi
bind -m vi-insert "\C-l":clear-screen
bind '"kj":vi-movement-mode'

# SET THE MODE STRING AND CURSOR TO INDICATE THE VIM MODE
#   FOR THE NUMBER AFTER `\e[`:
#     0: blinking block
#     1: blinking block (default)
#     2: steady block
#     3: blinking underline
#     4: steady underline
#     5: blinking bar (xterm)
#     6: steady bar (xterm)
bind 'set show-mode-in-prompt on'
bind 'set vi-ins-mode-string \1\e[3 q\2'
bind 'set vi-cmd-mode-string \1\e[4 q\2'
# Eternal bash history.
export HISTFILESIZE=
export HISTSIZE=
export HISTTIMEFORMAT="[%F %T] "
export HISTFILE=~/.bhist
export HISTCONTROL=ignoredups
PROMPT_COMMAND="history -a; $PROMPT_COMMAND"
stty -ixon
