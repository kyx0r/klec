
"In insert or command mode, move normally by using Ctrl
syntax on
set encoding=utf-8
set number
set backupdir-=.
set backupdir^=/tmp

let g:ycm_global_ycm_extra_conf = '~/VEther/.ycm_extra_conf.py'
let g:ycm_enable_diagnostic_signs = 0
let g:ycm_enable_diagnostic_highlighting = 0
function! SwitchToNextBuffer(incr)
  let help_buffer = (&filetype == 'help')
  let current = bufnr("%")
  let last = bufnr("$")
  let new = current + a:incr
  while 1
    if new != 0 && bufexists(new) && ((getbufvar(new, "&filetype") == 'help') == help_buffer)
      execute ":buffer ".new
      break
    else
      let new = new + a:incr
      if new < 1
        let new = last
      elseif new > last
        let new = 1
      endif
      if new == current
        break
      endif
    endif
  endwhile
endfunction
nnoremap <silent> <C-n> :call SwitchToNextBuffer(1)<CR>
nnoremap <silent> <C-p> :call SwitchToNextBuffer(-1)<CR>

nnoremap <C-k> :w<CR>
nnoremap <C-l> :e .<CR>
nnoremap <C-j> :bdelete<CR>
nnoremap <C-h> :q<CR>
inoremap <C-h> <Left>
inoremap <C-j> <Down>
inoremap <C-k> <Up>
inoremap <C-l> <Right>
cnoremap <C-h> <Left>
cnoremap <C-j> <Down>
cnoremap <C-k> <Up>
cnoremap <C-l> <Right>
"colorscheme 256_noir
