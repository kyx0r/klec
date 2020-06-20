
syntax on
set encoding=utf-8
set backupdir-=.
set backupdir^=/tmp
set backspace=indent,eol,start
set number relativenumber
set nu rnu
set cursorline
set cursorcolumn
set wildmenu
set wildmode=longest:full,full
set path+=**
highlight CursorLine term=reverse cterm=reverse ctermfg=Yellow guibg=Grey
highlight CursorColumn term=reverse cterm=reverse ctermfg=Yellow guibg=Grey
highlight Visual term=reverse cterm=reverse guibg=Grey
let g:netrw_banner=0 "disable the file browser banner
let g:netrw_liststyle=3 "tree view

autocmd InsertEnter * norm zz
command! Tags !ctags -R .

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
nnoremap S :%s///gI<Left><Left><Left>
"colorscheme 256_noir

