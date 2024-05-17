#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

if [ -f ~/.rc ]; then
	. ~/.rc
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
# ignore duplicate commands, ignore commands starting with a space
export HISTCONTROL=erasedups:ignorespace
export HISTSIZE=-1
export HISTFILESIZE=-1
export PROMPT_COMMAND="history -n; history -a"
shopt -s histappend

complete -cf r
