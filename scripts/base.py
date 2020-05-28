
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

_USERNAME = ""
_PASSWORD = ""


def main():
    api = PixivAPI()
    api.login(_USERNAME, _PASSWORD)

    json_result = api.latest_works(page=1, per_page=5000)

    directory = "/tmp/illusts"
    if not os.path.exists(directory):
        os.makedirs(directory)

    text_file = open("output.txt", "a")

    for illust in json_result.response:
       print("https://www.pixiv.net/en/artworks/%s" % illust.id)
       text_file.write("https://www.pixiv.net/en/artworks/%s\n" % illust.id)
       #print(">>> %s url: %s" % (illust.title, illust.image_urls.large))
       api.download(illust.image_urls.large, path=directory, fname='%s' % illust.id)

    text_file.close()

if __name__ == '__main__':
    main()

