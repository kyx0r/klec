#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

export PATH=/home/ether/Hihon:$PATH
export EDITOR=vim
export PS1="\[\e[36m\][\[\e[m\]\[\e[31m\]\u\[\e[m\]\[\e[34m\]@\[\e[m\]\[\e[32m\]\h\[\e[m\]:\[\e[35m\]\w\[\e[m\]\[\e[36m\]]\[\e[m\]$ "
export CFLAGS="-Os -pipe -march=native"                              
export CXXFLAGS="$CFLAGS"                                            
export MAKEFLAGS="-j8"                                               
    

set -o vi
bind -m vi-insert "\C-l":clear-screen

if [ -f ~/ten_years__time_by_bythmark_d4dg6dz ]; then
if xset q &>/dev/null; then
feh --bg-scale ~/ten_years__time_by_bythmark_d4dg6dz &
disown
fi
fi

alias grep='grep --color=auto'
alias ip='ip'
alias ls='ls --color=auto'
alias 'l'='ls -glhaS'
alias 'df'='df -h'
alias 'rm'='rm -rf'
alias 'v'='vim'
alias 's'='su root'
alias 'f'='feh --title %f___[%wX%h]_[%u/%l] --min-dimension 1920x1080 -p'
alias 'fu'='feh --title %f___[%wX%h]_[%u/%l] --min-dimension 1920x1080 --max-dimension 2560x1440 -p'
alias 'g'='git'
alias 'ga'='git add .'
alias 'gc'='git commit -a -m'
alias 'gp'='git push'
alias 'gs'='git status'
alias 'p'='sudo pacman'
alias 'c'='cd ..'
alias 'r'='sudo'
alias 'm'='makepkg -si --skippgpcheck --skipinteg --skipchecksums'
alias 'mi'='sudo make install'
alias ','='sudo vim /etc/X11/xinit/xinitrc'

complete -cf r

mkfile() { mkdir -p "$(dirname "$1")" && touch "$1" ;  }

#disable ^s lock
stty -ixon 
shopt -s autocd
