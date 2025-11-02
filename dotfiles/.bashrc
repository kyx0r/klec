#
# ~/.bashrc
#

case "$SHELL" in *bash*) ;; *) unset HISTFILE; return ;; esac
case "$(echo $0)" in *bash*) ;; *) unset HISTFILE; return ;; esac
# If not running interactively, don't do anything
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
export HISTCONTROL=ignoredups:erasedups
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

colors() {
	paste <(i=0; while [ $i -le 255 ]; do \
		printf "\e[48;5;%sm%3d\e[0m " "$i" "$i"; \
		if [ "$i" -eq 15 ] || { [ "$i" -gt 15 ] && [ $(( (i - 15) % 6 )) -eq 0 ]; }; then \
			echo; \
		fi; \
		i=$((i + 1)); \
	done) <(i=0; while [ $i -le 255 ]; do \
		printf "\e[30m\e[48;5;%sm%3d\e[0m " "$i" "$i"; \
		if [ "$i" -eq 15 ] || { [ "$i" -gt 15 ] && [ $(( (i - 15) % 6 )) -eq 0 ]; }; then \
			echo; \
		fi; \
		i=$((i + 1)); \
	done)
}
