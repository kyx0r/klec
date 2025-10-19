
syntax on
set encoding=utf-8
set backupdir-=.
set backupdir^=/run
set directory^=/run
set backspace=indent,eol,start
set number relativenumber
set nu rnu
"set cursorline lags
"set cursorcolumn
set wildmenu
set wildmode=longest:full,full
set path+=**
set mouse=i
set noshowmode
set nofoldenable
set foldmethod=syntax
set listchars=eol:¬,tab:>-,trail:~,extends:>,precedes:<,space:_
set colorcolumn=80
set hidden
set noswapfile
highlight ColorColumn term=bold cterm=bold ctermfg=Cyan ctermbg=none
highlight Visual term=bold cterm=reverse guibg=Grey
highlight CursorLine term=bold cterm=bold ctermfg=Yellow ctermbg=none
highlight CursorColumn term=bold cterm=bold ctermfg=Yellow ctermbg=none
highlight Folded term=bold cterm=none ctermfg=Magenta ctermbg=none
highlight FoldColumn term=bold cterm=none ctermfg=Magenta ctermbg=none
hi Search cterm=NONE ctermfg=Red ctermbg=none
let g:netrw_banner=0 "disable the file browser banner
let g:netrw_liststyle=3 "tree view
let mapleader = "\<Space>"
let $RTP=split(&runtimepath, ',')[0]
let g:netrw_bufsettings = 'noma nomod nu nobl nowrap ro'

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
nnoremap <C-j> :tabnew .<CR>
nnoremap TT :set list!<CR>
nnoremap <leader>t :tn<CR>
nnoremap <leader>g xxi.<ESC>
nnoremap <leader>h xi-><ESC>
nnoremap <leader>r :source ~/.vimrc<CR>
nnoremap <leader>; :<C-f>i
nnoremap S :<C-f>i%s///gI<ESC>hhhi
nnoremap X :tabn<CR>
nnoremap <leader>s :%s/\<<C-r><C-w>\>/<C-r><C-w>/gI<Left><Left><Left>
nnoremap <leader>o o<ESC>
nnoremap <leader>1 :colorscheme 256_noir<CR>
inoremap kj <Esc>l
vnoremap kj <Esc>
inoremap <expr> h pumvisible() ? "\<C-n>" : "h"
inoremap <expr> l pumvisible() ? "\<C-p>" : "l"

" Highlight all instances of word under cursor, when idle.
" Useful when studying strange source code.
" Type z/ to toggle highlighting on/off.
nnoremap z/ :if AutoHighlightToggle()<Bar>set hls<Bar>endif<CR>
function! AutoHighlightToggle()
   let @/ = ''
   if exists('#auto_highlight')
     au! auto_highlight
     augroup! auto_highlight
     setl updatetime=4000
     echo 'Highlight current word: off'
     return 0
  else
    augroup auto_highlight
    au!
    au CursorHold * let @/ = '\V\<'.escape(expand('<cword>'), '\').'\>'
    augroup end
    setl updatetime=500
    echo 'Highlight current word: ON'
  return 1
 endif
endfunction

function! MyFoldText()
    let nblines = v:foldend - v:foldstart + 1
    let w = winwidth(0) - &foldcolumn - (&number ? 8 : 0)
    let expansionString = repeat(" ", w+1)
    let txt = '+ ' . nblines . expansionString
    return txt
endfunction
set foldtext=MyFoldText()
