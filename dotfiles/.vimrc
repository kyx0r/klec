
syntax on
set encoding=utf-8
set number
set backupdir-=.
set backupdir^=/tmp

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
nnoremap <C-h> :q<CR>
nnoremap <C-j> :bdelete<CR>
"colorscheme 256_noir

