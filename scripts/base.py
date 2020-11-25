
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
    opts, args = getopt.getopt(argv, 'x:')
    print(f'Options Tuple is {opts}')

    api = PixivAPI()
    api.login(_USERNAME, _PASSWORD)

    barg = 1
    for opt, arg in opts:
        if opt == '-x':
           barg = arg

    json_result = api.latest_works(page=barg, per_page=500)

    directory = "/tmp/illusts"
    if not os.path.exists(directory):
        os.makedirs(directory)


    for illust in json_result.response:
       print("https://www.pixiv.net/en/artworks/%s" % illust.id)
       #print(">>> %s url: %s" % (illust.title, illust.image_urls.large))
       api.download(illust.image_urls.large, path=directory, fname='%s' % illust.id)

if __name__ == '__main__':
    main(sys.argv[1:])
