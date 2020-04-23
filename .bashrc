#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

set -o vi
bind -m vi-insert "\C-l":clear-screen

feh --bg-scale ~/ten_years__time_by_bythmark_d4dg6dz
export PATH=/home/ether/temp/st:$PATH
export PATH=/home/ether/tiny_vulkan/src:$PATH

export PS1="\[\e[36m\][\[\e[m\]\[\e[31m\]\u\[\e[m\]\[\e[34m\]@\[\e[m\]\[\e[32m\]\h\[\e[m\]:\[\e[35m\]\w\[\e[m\]\[\e[36m\]]\[\e[m\]\[$\[\e[m\] "

#xv /home/ether/f.jpg -root -geometry 1920x1080 -quit

alias ls='ls --color=auto'
mkfile() { mkdir -p "$(dirname "$1")" && touch "$1" ;  }
