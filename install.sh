cp ten_years__time_by_bythmark_d4dg6dz ~/
cp -r GoldenShimmer /usr/share/icons
cp dotfiles/.twmrc ~/
cp dotfiles/.bashrc ~/
cp dotfiles/.gdbinit ~/
cp dotfiles/.vimrc ~/
cp dotfiles/keynavrc /etc
cp dotfiles/local.conf /etc/fonts
cd st
make install
cd ../
cd dwm
make install
