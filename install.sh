mkdir -p /usr/share/icons/default/
mkdir -p /etc/fonts
cp -r cgi/GoldenShimmer /usr/share/icons
cp -r cgi/terminus-ttf-4.47.0/ /usr/share/fonts

ln -sf $PWD/dotfiles/.bashrc ~/
ln -sf $PWD/dotfiles/.profile ~/
ln -sf $PWD/dotfiles/.rc ~/
ln -sf $PWD/dotfiles/.gdbinit ~/
ln -sf $PWD/dotfiles/.vimrc ~/
ln -sf $PWD/dotfiles/.gitconfig ~/
ln -sf $PWD/dotfiles/.mailrc ~/
ln -sf $PWD/dotfiles/lynx_bookmarks.html ~/
ln -sf $PWD/dotfiles/keynavrc /etc
ln -sf $PWD/dotfiles/lynx.cfg /etc
ln -sf $PWD/dotfiles/index.theme /usr/share/icons/default/
ln -sf $PWD/dotfiles/local.conf /etc/fonts
ln -sf $PWD/dotfiles/grub /etc/default/

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
cd ../nextvi
./build.sh
./build.sh install
cd ../dmenu
make install
