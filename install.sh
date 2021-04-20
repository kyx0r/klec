mkdir -p /usr/share/icons/default/
mkdir -p /etc/fonts
cp cgi/ten_years__time_by_bythmark_d4dg6dz ~/
cp -r cgi/GoldenShimmer /usr/share/icons
cp -r cgi/terminus-ttf-4.47.0/ /usr/share/fonts

cp dotfiles/.twmrc ~/
cp dotfiles/.bashrc ~/
cp dotfiles/.profile ~/
cp dotfiles/.rc ~/
cp dotfiles/.gdbinit ~/
cp dotfiles/.vimrc ~/
cp dotfiles/.gitconfig ~/
cp dotfiles/.mailrc ~/
cp dotfiles/keynavrc /etc
cp dotfiles/lynx.cfg /etc
cp dotfiles/index.theme /usr/share/icons/default/
cp dotfiles/local.conf /etc/fonts
cp dotfiles/grub /etc/default/

cd cprogs
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
cd ../grabc
make install-bin
cd ../dmenu
make install
