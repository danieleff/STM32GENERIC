import json
import os.path
import urllib2
import time
import urllib
import zipfile

urllib.urlretrieve ("http://downloads.arduino.cc/libraries/library_index.json", "library_index.json")

with open('library_index.json') as file:
    data = json.load(file)

libraries = {}

for library in data["libraries"]:
    libraries[library["name"]] = library

for name, library in libraries.iteritems():
    
    zip_file = os.path.basename(library["url"])
    download_file = "cache/" + zip_file
    
    print zip_file
    
    if not os.path.exists(download_file):
        try:
            urllib.urlretrieve (library["url"], download_file)
        except Exception, e:
            print e
        time.sleep(1)
    try:
        zip_ref = zipfile.ZipFile(download_file, 'r')
        zip_ref.extractall('libraries')
        zip_ref.close()
    except Exception, e:
            print e