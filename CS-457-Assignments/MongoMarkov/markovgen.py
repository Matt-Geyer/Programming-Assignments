# Generate Text based on the collections in Mongo
# Use ngramDriver to populate database with ngrams and their probabilites

from pymongo import Connection
import random
from collections import OrderedDict
import operator
import sys

if (len(sys.argv) > 1):
    numWords = int(sys.argv[1])
else:
    numWords = 15

con = Connection()
db = con["ngdocs"]
docs = db.docs
ngstats = db.ngstats

ngp = {}

results = ngstats.find()
total = 0

for result in results:
    total += result["value"]

results = ngstats.find()
for result in results:
    ngp[result["_id"]] = float(result["value"])

ngprob = OrderedDict(sorted(ngp.items(), key = lambda t: t[1]))

ngdict = {}
nglist = []

for key in ngprob.keys():
    t = []
    t += key.split(' ')
    t.append(ngprob[key])
    nglist.append(t)
    

randDict = {}   # Holds the cumulative total of probabilities
c_total = 0

for key in ngprob.keys():
    c_total += ngprob[key] / total
    randDict[key] = c_total

r = random.random()
prev = 0
for key in ngprob.keys():
    if r >= prev and r < randDict[key]:
        string = key
        break      
    prev = randDict[key]

#print "Seed is: " + seed
seed = string.split(' ')
''' So many for loops.. feels wrong '''
for i in range(0,numWords):
    plist = []
    for ngram in nglist:
        if (seed[1] == ngram[0]):
            plist.append(ngram)

    ''' select a random element from the list based on probability '''
    plist_total = 0
    for ngram in plist:
        key = ngram[0] + " " + ngram[1]
        plist_total += ngprob[key]

    pd = {}
    for ngram in plist:
        key = ngram[0] + " " + ngram[1]
        pd[key] = ngprob[key] / plist_total

    pdict = OrderedDict(sorted(pd.items(), key = lambda t: t[1]))
    randDict = {}
    c_total = 0
    for key in pdict.keys():
        c_total += pdict[key]
        randDict[key] = c_total
    r = random.random()
    prev = 0
    for key in randDict.keys():
        if r >= prev and r < randDict[key]:
            string += " " + key.split(' ')[1]
            break
        prev = randDict[key]
        
    strList = string.split(' ')
    seed = strList[len(strList)-2:]   
    

print string
        
