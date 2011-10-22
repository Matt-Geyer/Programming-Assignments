#!/usr/bin/python

import SimpleXMLRPCServer
import sys

# Port number
port = 6789

class MyRPC(object):
    def __init__(self):
        self.log = []
    def send(self,user,message,time):
        self.log.append((user,message,time))
        return self.log
    def get(self):
        return self.log

if __name__== "__main__":
    server = SimpleXMLRPCServer.SimpleXMLRPCServer(("localhost",port))
    server.register_instance(MyRPC())
    server.serve_forever()


