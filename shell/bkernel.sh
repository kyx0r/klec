make -j 4
make INSTALL_MOD_STRIP=1 modules_install
make install

mv /boot/vmlinuz /boot/vmlinuz-5.8.13
mv /boot/System.map /boot/System.map-5.8.13
