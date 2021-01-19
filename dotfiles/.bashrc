#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

export KISS_SU=su
export KISS_PATH=$HOME/repo/core:$HOME/repo/extra:$HOME/repo/xorg:$HOME/community/community
export PATH=/root/klec/shell:$PATH
export EDITOR=vi
export PS1="\[\e[35m\]\w\[\e[m\] "
export CFLAGS="-O3"                              
export KCFLAGS="$CFLAGS"
export CXXFLAGS="$CFLAGS"                                            
export KCPPFLAGS="$CFLAGS"                                            
export MAKEFLAGS="-j$(nproc)"
    
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
export HISTSIZE=50000
shopt -s histappend

if [ -f ~/ten_years__time_by_bythmark_d4dg6dz ]; then
if [[ ! -z "${DISPLAY}" ]]; then
feh --bg-scale ~/ten_years__time_by_bythmark_d4dg6dz &
disown
fi
fi

if type setxkbmap >/dev/null 2>&1; then
        setxkbmap -layout us -option ctrl:nocaps 2>/dev/null
fi


alias grep='grep --color=auto'
alias ip='ip'
alias ls='ls --color=auto'
alias 'l'='ls -glhaS'
alias 'df'='df -h'
alias 'rm'='rm -rf'
alias 'v'='vi'
alias 'i'='vi'
alias 's'='su root'
alias 'f'='feh --title %f___[%wX%h]_[%u/%l] --min-dimension 1920x1080 -p'
alias 'fu'='feh --title %f___[%wX%h]_[%u/%l] --min-dimension 1920x1080 --max-dimension 2560x1440 -p'
alias 'g'='git'
alias 'ga'='git add .'
alias 'gc'='git commit -a -m'
alias 'gcr'='git clone --recursive'
alias 'gp'='git push'
alias 'gl'='git pull'
alias 'gs'='git status'
alias 'gcls'='git clean -fx'
alias 'gr'='git restore'
alias 'grh'='git reset --hard'
alias 'gch'='git checkout'
alias 'p'='pacman'
alias 'pc'='rm /var/lib/pacman/db.lck'
alias 'm'='makepkg -si --skippgpcheck --skipinteg --skipchecksums'
alias 'mi'='make install'
alias 'xq'='xbps-query -R -s'
alias 'xi'='xbps-install'
alias 'xr'='xbps-remove'
alias 'x'='. fastcd.sh'
alias ','='vi /etc/X11/xinit/xinitrc'
alias ',,'='vi ~/.bashrc'
alias ',,,'='vim ~/.vimrc'
alias 'd'='gdb --args'
alias 'c'='cd ..'
alias '..'='cd ../..'
alias '...'='cd ../../..'
alias '....'='cd ../../../..'
alias '.....'='cd ../../../../..'
alias 'fnd'='find . -name'
alias 'chromium'='chromium --no-sandbox'

complete -cf r

ki() { kiss b "$1" && kiss i "$1"; }
loc() { find . -name "$@" | xargs wc -l; }
r() { grep -r "$@" . ; }
cdls() { cd "$@" && l; }
alias 'cd'='cdls'
mkfile() { mkdir -p "$(dirname "$1")" && touch "$1" ;  }

#disable ^s lock
stty -ixon 
shopt -s autocd

compr() {
    FILE=$1
    shift
    case $FILE in
        *.tar.bz2) tar cjf $FILE $*  ;;
        *.tar.gz)  tar czf $FILE $*  ;;
        *.tgz)     tar czf $FILE $*  ;;
        *.zip)     zip $FILE $*      ;;
        *.rar)     rar $FILE $*      ;;
        *)         echo "Filetype not recognized" ;;
   esac
}

## EXTRACT FUNCTION ##
extr () {
    if [ -f $1 ] ; then
        case $1 in
            *.tar.bz2)   tar xjf $1     ;;
            *.tar.gz)    tar xzf $1     ;;
            *.bz2)       bunzip2 $1     ;;
            *.rar)       unrar e $1     ;;
            *.gz)        gunzip $1      ;;
            *.tar)       tar xf $1      ;;
            *.tbz2)      tar xjf $1     ;;
            *.tgz)       tar xzf $1     ;;
            *.zip)       unzip $1       ;;
            *.Z)         uncompress $1  ;;
            *.7z)        7z x $1        ;;
            *)     echo "'$1' cannot be extracted via extract()" ;;
        esac
    else
        echo "'$1' is not a valid file"
    fi
}
