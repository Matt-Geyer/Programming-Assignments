from pymongo import Connection
from bson.code import Code


i = 1
strip_chars = ""
while(i<0x20):
    strip_chars += chr(i)
    i += 1
strip_chars += '.'

con = Connection()
db = con["ngdocs"]
docs = db.docs

def esFilter(item):
    if item == '':
        return False
    else:
        return True
    
def ngramDoc(filename,nlength):
    ''' Return a dictionary in the format of the MongoDB document '''
    doc = {}

    doc["ngrams"] = []
    
    reader = open(filename, "r")
    
    body = reader.read()
    
    ''' Strip unwanted chars'''
    body = body.replace('\n', ' ')
    body = body.replace('\r', ' ')
    body = body.replace('\t', ' ')                    
    body = body.translate(None,strip_chars).lower()
    body = body.split(' ')  # Turn to a list of words
    body = filter(esFilter,body)

    ''' Iterate and construct the n-grams'''
    for i in range(0, len(body) + 1 - nlength): # Make sure index stays in range
        key = ""   # Create a fresh key string at every i
        for n in range(0,nlength):  
            if i + n >= len(body):
                continue
            key += body[i + n] + " "
            
        key = key[:len(key)-1]  # remove the last space
        try:
            key = unicode(key,'utf-8')
        except UnicodeDecodeError:
            # This doesn't seem to happen often -- will try to fix it
            print "Unable to decode token: ", token_str
            continue
        
        ''' Increase the count for the n-gram
        try:
            doc[key] += 1
        except KeyError:
            doc[key] = 1
        '''
        doc["ngrams"].append(key)
           
    doc["title"] = filename
    
    return doc    
    
   
def insertDoc(doc):
    try:
        title = doc["title"]
    except:
        print "Invalid parameter. Returning"
        return

    result = docs.find_one({"title":title})

    ''' If the document is not already in the database, insert it '''
    if result == None:
        docs.save(doc)

def mongoMapReduce():
    return docs.map_reduce(m, r, "ngstats")
    


m = Code("function() {"
         "this.ngrams.forEach(function(ngram) {"
         "    emit(ngram,1);"
         "    });"
         "}")

r = Code("function (key, values) {"
         "  var total = 0;"
         "  for (var i = 0; i < values.length; i++) {"
         "      total += values[i];"
         "    }"
         "    return total;"
         "}")
def menu():
    print "N-gram builder menu"
    print "1 - Insert Mongo Ngram doc from file"
    print "2 - Run MapReduce routine on DB"
    print "3 - Exit"
    print "\n\n"
    choice = raw_input("Enter your choice: ")
    return int(choice)

def mainLoop():
    choice = None
    READ = 1
    MAPREDUCE = 2
    EXIT = 3
    while choice != EXIT:
        
        choice = menu()        
        if choice == READ:
            fn = raw_input("Enter filename: ")
            order = raw_input("Enter ngram order: ")
            try:
                doc = ngramDoc(fn, int(order))
                insertDoc(doc)
            except:
                print "Unable to insert document!"
        elif choice == MAPREDUCE:
            mongoMapReduce()
        elif choice == EXIT:
            break
        else:
            continue
            

    print "\nGoodbye!"

mainLoop()
    
    
    
