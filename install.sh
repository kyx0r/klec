cp ten_years__time_by_bythmark_d4dg6dz ~/
mkdir -p /usr/share/icons/default/
mkdir -p /etc/fonts
cp -r GoldenShimmer /usr/share/icons
cp dotfiles/.twmrc ~/
cp dotfiles/.bashrc ~/
cp dotfiles/.bash_profile ~/
cp dotfiles/.gdbinit ~/
cp dotfiles/.vimrc ~/
mkdir -p ~/.vim/colors
cp dotfiles/256_noir.vim ~/.vim/colors
cp dotfiles/.gitconfig ~/
cp dotfiles/keynavrc /etc
cp dotfiles/lynx.cfg /etc
cp dotfiles/index.theme /usr/share/icons/default/
cp dotfiles/local.conf /etc/fonts
cp dotfiles/grub /etc/default/
cp -r terminus-ttf-4.47.0/ /usr/share/fonts
cd st
make install
cd ../dwm
make install
cd ../slstatus
make install
cd ../xkeyboard
make install
cd ../utils 
./build.sh
cp -f ./cembed /bin
cd ../neatvi
make install
cd ../hund
make install
cd ../fastcd
make install
cd ../grabc
make install-bin
