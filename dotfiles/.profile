export PATH=/home/ether/Hihon:$PATH
export EDITOR=vim
export PS1="\[\e[36m\][\[\e[m\]\[\e[31m\]\u\[\e[m\]\[\e[34m\]@\[\e[m\]\[\e[32m\]\h\[\e[m\]:\[\e[35m\]\w\[\e[m\]\[\e[36m\]]\[\e[m\]$ "
set -o vi

if [ -f ~/ten_years__time_by_bythmark_d4dg6dz ]; then
if xset q &>/dev/null; then
feh --bg-scale ~/ten_years__time_by_bythmark_d4dg6dz &
fi
fi

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

#disable ^s lock
stty -ixon 
