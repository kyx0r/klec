
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

    barg = 0
    for opt, arg in opts:
        if opt == '-x':
           json_result = api.ranking(mode="daily", page=1, per_page=1500, date=arg)
           barg = 1

    if not barg:
       json_result = api.ranking(mode="daily", page=1, per_page=1500, date="2020-03-06")

    directory = "/tmp/illusts"
    if not os.path.exists(directory):
        os.makedirs(directory)

    text_file = open("output.txt", "a")

    for illust in json_result.response[0].works:
       print("https://www.pixiv.net/en/artworks/%s" % illust.work.id)
       #print(">>> %s origin url: %s" % (illust.work.title, illust.work.image_urls['large']))
       text_file.write("https://www.pixiv.net/en/artworks/%s\n" % illust.work.id)
       api.download(illust.work.image_urls['large'], path=directory, fname='%s' % illust.work.id)

    text_file.close()

if __name__ == '__main__':
    main(sys.argv[1:])

