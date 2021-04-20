HOST=''
USER=''
PASS=''
TARGETFOLDER='/www/kyryl.tk'
SOURCEFOLDER='/root/My-Website/kyryl.tk'

lftp -f "
set ftp:ssl-allow no
open $HOST
user $USER $PASS
lcd $SOURCEFOLDER
mirror --reverse --delete --verbose $SOURCEFOLDER $TARGETFOLDER
bye
"
