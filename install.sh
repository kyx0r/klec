cp ten_years__time_by_bythmark_d4dg6dz ~/
cp -r GoldenShimmer /usr/share/icons
cp dotfiles/.twmrc ~/
cp dotfiles/.bashrc ~/
cp dotfiles/.gdbinit ~/
cp dotfiles/.vimrc ~/
cp dotfiles/.gitconfig ~/
cp dotfiles/keynavrc /etc
cp dotfiles/lynx.cfg /etc
cp dotfiles/index.theme /usr/share/icons/default/
cp dotfiles/local.conf /etc/fonts
cp dotfiles/grub /etc/default/
cd st
make install
cd ../
cd dwm
make install
cd ../
cd slstatus
make install
