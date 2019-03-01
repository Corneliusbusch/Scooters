
import hmac
import hashlib
import numpy
import requests
import json
from keys import api_key, secret_key


#api_key     = 'mpagOBWTPi2WfIx-'
#secret_key  = 'a7Zc8vMpPnIZtb12mN54eOf3Ho3wVlEw'.encode('utf-8')



year = "%2A"
currentPage = 1
size = 1

def createQuery(festival, page, year, size):
    if page == 1:
        return "festival=" + festival + "&size=" +str(size) + "&year=" + year
    else:
        return "festival=" + festival + "&size=" +str(size) + "&from="+ str(page) + "&year=" + year

#for n in range(0, 10):
#    print(createQuery("demofringe", n*size, year, size))

query       = '/events?' + createQuery("demofringe", currentPage, year, size)  + '&pretty=1' + '&key=' + api_key
signature   = hmac.new(secret_key, query.encode('utf-8'), hashlib.sha1).hexdigest()
url         = 'https://api.edinburghfestivalcity.com' + query + '&signature=' + signature

def runQuery(url):
    print(url)
    r = requests.get(url)
    print(r.status_code)
    text_file = open("Output.txt", "w")
    #d = json.loads(r.text)
    text_file.write(r.text)
    text_file.close()
    print(r.text)


runQuery(url)
