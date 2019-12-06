#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

feh --bg-scale ~/f.jpg
export PATH=/home/ether/st:$PATH

export PS1="\[\e[36m\][\[\e[m\]\[\e[31m\]\u\[\e[m\]\[\e[34m\]@\[\e[m\]\[\e[32m\]\h\[\e[m\]:\[\e[35m\]\w\[\e[m\]\[\e[36m\]]\[\e[m\]\[$\[\e[m\] "

#xv /home/ether/f.jpg -root -geometry 1920x1080 -quit

alias ls='ls --color=auto'
mkfile() { mkdir -p "$(dirname "$1")" && touch "$1" ;  }
