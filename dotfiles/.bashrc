#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

export PATH=/root/klec/shell:$PATH
export EDITOR=vim
export PS1="\[\e[35m\]\w\[\e[m\] "
export CFLAGS="-O3"                              
export KCFLAGS="$CFLAGS"
export CXXFLAGS="$CFLAGS"                                            
export KCPPFLAGS="$CFLAGS"                                            
export MAKEFLAGS="-j8"                                               
    
set -o vi
bind -m vi-insert "\C-l":clear-screen

if [ -f ~/ten_years__time_by_bythmark_d4dg6dz ]; then
if [[ ! -z "${DISPLAY}" ]]; then
feh --bg-scale ~/ten_years__time_by_bythmark_d4dg6dz &
disown
fi
fi

cdls() { cd "$@" && find . -maxdepth 1 -type d; }

alias grep='grep --color=auto'
alias ip='ip'
alias ls='ls --color=auto'
alias 'cd'='cdls'
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
alias 'gcls'='git clean -fx'
alias 'p'='sudo pacman'
alias 'r'='sudo'
alias 'm'='makepkg -si --skippgpcheck --skipinteg --skipchecksums'
alias 'mi'='sudo make install'
alias 'xq'='xbps-query -R -s'
alias 'xi'='xbps-install'
alias 'xr'='xbps-remove'
alias ','='sudo vim /etc/X11/xinit/xinitrc'
alias 'd'='gdb --args'
alias 'c'='cd ..'
alias '..'='cd ../..'
alias '...'='cd ../../..'
alias '....'='cd ../../../..'
alias '.....'='cd ../../../../..'

complete -cf r

mkfile() { mkdir -p "$(dirname "$1")" && touch "$1" ;  }

#disable ^s lock
stty -ixon 
shopt -s autocd
