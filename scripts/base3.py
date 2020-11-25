import os
import sys
if sys.version_info >= (3, 0):
    import imp
    imp.reload(sys)
else:
    reload(sys)
    sys.setdefaultencoding('utf8')
sys.dont_write_bytecode = True

from pixivpy3 import *
import getopt

_USERNAME = ""
_PASSWORD = ""


def main(argv):
    opts, args = getopt.getopt(argv, 'n:s:')
    print(f'Options Tuple is {opts}')

    directory = "/tmp/illusts"
    if not os.path.exists(directory):
        os.makedirs(directory)

    api = PixivAPI()
    api.login(_USERNAME, _PASSWORD)

    barg = 1
    beginid = 82939879
    for opt, arg in opts:
        if opt == '-n':
           barg = int(arg)
        if opt == '-s':
           beginid = int(arg)


    for x in range(0, barg):
       json_result = api.works(beginid)
       if json_result.response:
           illust = json_result.response[0]
           print("https://www.pixiv.net/en/artworks/%s" % illust.id)
           api.download(illust.image_urls.large, path=directory, fname='%s' % illust.id)
       beginid = beginid - 1
       #print(">>> %s url: %s" % (illust.title, illust.image_urls.large))

if __name__ == '__main__':
    main(sys.argv[1:])
