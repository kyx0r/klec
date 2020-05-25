#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

export PATH=/home/ether/Hihon:$PATH
export EDITOR=vim
export PS1="\[\e[36m\][\[\e[m\]\[\e[31m\]\u\[\e[m\]\[\e[34m\]@\[\e[m\]\[\e[32m\]\h\[\e[m\]:\[\e[35m\]\w\[\e[m\]\[\e[36m\]]\[\e[m\]\[$\[\e[m\] "

set -o vi
bind -m vi-insert "\C-l":clear-screen

if [ -f ~/ten_years__time_by_bythmark_d4dg6dz ]; then
if xset q &>/dev/null; then
feh --bg-scale ~/ten_years__time_by_bythmark_d4dg6dz &
disown
fi
fi

alias grep='grep --color=auto'
alias ip='ip -color=auto'
alias ls='ls --color=auto'
alias 'l'='ls -golhaS --group-directories-first'
alias 'rm'='rm -rf'
alias 'v'='vim'
alias 's'='su root'
alias 'f'='feh --title %f___[%wX%h]_[%u/%l] --min-dimension 1920x1080 -p'
alias 'fu'='feh --title %f___[%wX%h]_[%u/%l] --min-dimension 1920x1080 --max-dimension 2560x1440 -p'
alias 'ga'='git add .'
alias 'gc'='git commit -a -m'
alias 'gp'='git push'

mkfile() { mkdir -p "$(dirname "$1")" && touch "$1" ;  }
