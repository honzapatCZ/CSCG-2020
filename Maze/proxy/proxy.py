import socket
import os
from threading import Thread
import mparser
import random
import struct
from importlib import *

# this code gets worse and worse!!!

class Proxy2Server(Thread):

    def __init__(self, host, port):
        super(Proxy2Server, self).__init__()
        self.game = None # game client socket not known yet
        self.port = port
        self.host = host
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.connect((host, port))

    # run in thread
    def run(self):
        while True:
            data = self.server.recv(4096)
            if data:
                #print "[{}] <- {}".format(self.port, data[:100].encode('hex'))
                try:         
                    mparser.parse(data, self.port, 'server')
                    if len(mparser.CLIENT_QUEUE)>0:
                        pkt = mparser.CLIENT_QUEUE.pop()
                        #print "got queue client: {}".format(pkt.encode('hex'))
                        self.game.sendall(pkt)
                except Exception as e:
                    print ('server[{}]'.format(self.port), e)
                # forward to client
                self.game.sendall(data)

class Game2Proxy(Thread):

    def __init__(self, host, port):
        super(Game2Proxy, self).__init__()
        self.server = None # real server socket not known yet
        self.port = port
        self.host = host
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((host, port))
        sock.listen(1)
        # waiting for a connection
        self.game, addr = sock.accept()
    def run(self):
        while True:
            data = self.game.recv(4096)
            if data:
                #print "[{}] -> {}".format(self.port, data[:100].encode('hex'))
                try:
                    mparser.parse(data, self.port, 'client')
                    if len(mparser.SERVER_QUEUE)>0:
                        pkt = mparser.SERVER_QUEUE.pop()
                        #print "got queue server: {}".format(pkt.encode('hex'))
                        self.server.sendall(pkt)
                except Exception as e:
                    print ('client[{}]'.format(self.port), e)
                # forward to server
                self.server.sendall(data)

class Proxy(Thread):

    def __init__(self, from_host, to_host, port):
        super(Proxy, self).__init__()
        self.from_host = from_host
        self.to_host = to_host
        self.port = port
        self.running = False

    def run(self):
        while True:
            print ("[proxy({})] setting up".format(self.port))
            self.g2p = Game2Proxy(self.from_host, self.port) # waiting for a client
            self.p2s = Proxy2Server(self.to_host, self.port)
            print ("[proxy({})] connection established".format(self.port))
            self.g2p.server = self.p2s.server
            self.p2s.game = self.g2p.game
            self.running = True
            self.g2p.start()
            self.p2s.start()

class UDPProxy2Server(Thread):

    def __init__(self, host, port):
        super(UDPProxy2Server, self).__init__()
        self.game = None # game client socket not known yet
        self.gSock = None
        self.gAddr = None
        self.port = port
        self.host = host
        self.address = None
        self.server = self
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.connect((self.host, self.port))

    # run in thread
    def run(self):
        while True:
            data, addr = self.sock.recvfrom(8192)
            self.game.sAddr = self.address;
            if data:
                #print ("[{}] <- {}".format(self.port, data[:100].hex()))
                try:         
                    mparser.parse(data, self.port, 'server')
                    if len(mparser.CLIENT_QUEUE)>0:
                        pkt = mparser.CLIENT_QUEUE.pop()
                        #print "got queue client: {}".format(pkt.encode('hex'))
                        self.game.sock.sendto(pkt, self.game.address)
                except Exception as e:
                    print ('server[{}]'.format(self.port), e)
                # forward to client
                self.game.sock.sendto(data,self.game.address)

class UDPGame2Proxy(Thread):

    def __init__(self, host, port):
        super(UDPGame2Proxy, self).__init__()
        self.server = None # real server socket not known yet
        self.sSock = None
        self.port = port
        self.host = host
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((host, port))
        # waiting for a connection
        data, self.address = self.sock.recvfrom(4096)
        self.game = self
    def run(self):
        while True:
            data, addr = self.sock.recvfrom(4096)
            
            if data:
                #print ("[{}] -> {}".format(self.port, data[:100].hex()))
                try:
                    mparser.parse(data, self.port, 'client')
                    if len(mparser.SERVER_QUEUE)>0:
                        pkt = mparser.SERVER_QUEUE.pop()
                        #print "got queue server: {}".format(pkt.encode('hex'))
                        self.server.sock.sendto(pkt,(self.host, self.port))
                except Exception as e:
                    print ('client[{}]'.format(self.port), e)
                # forward to server
                self.server.sock.send(data)

class UDPProxy(Thread):

    def __init__(self, from_host, to_host, port):
        super(UDPProxy, self).__init__()
        self.from_host = from_host
        self.to_host = to_host
        self.port = port
        self.running = False

    def run(self):
        while True:
            print ("[proxy({})] setting up".format(self.port))
            self.g2p = UDPGame2Proxy(self.from_host, self.port) # waiting for a client
            self.p2s = UDPProxy2Server(self.to_host, self.port)
            print ("[proxy({})] connection established".format(self.port))
            self.g2p.server = self.p2s.server
            self.g2p.sSock = self.p2s.sock
            self.p2s.game = self.g2p.game
            self.p2s.gSock = self.g2p.sock
            self.p2s.sAddr = self.g2p.address
            self.running = True
            self.g2p.start()
            self.p2s.start()


master_server = Proxy('192.168.1.77', '147.75.85.99', 80)
master_server.start()

game_servers = []
for port in range(1337, 1357):
    _game_server = UDPProxy('192.168.1.77', '147.75.85.99', port)
    _game_server.start()
    game_servers.append(_game_server)


while True:
    try:
        
        cmd = input('$ ')
        if cmd[:4] == 'quit':
            os._exit(0)
        elif cmd[0:2] == 'S ':
            # send to server
            for server in game_servers:
                if server.running:
                    mparser.SERVER_QUEUE.append(bytearray.fromhex(cmd[2:]))
        elif cmd[0:2] == 'C ':
            # send to client
            for server in game_servers:
                if server.running:
                    mparser.CLIENT_QUEUE.append(bytearray.fromhex(cmd[2:]))
        
        else:
            reload(mparser)
    except Exception as e:
        print (e)



