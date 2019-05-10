import os, sys, threading, time, socket
from pyftpdlib.authorizers import DummyAuthorizer
from pyftpdlib.handlers import FTPHandler
from pyftpdlib.servers import FTPServer

def getIP():
    """Gets the system local IP address"""
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('10.255.255.255', 1))
        IP = s.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP

def recieveFile():
    """Hosts an ftp server at port 1026 which is used by client app to send files to server"""
    password = input('Enter password for user. This password will only be valid for this session\n')
    print(f'Username: user \nPassword: {password}')
    ip = getIP()
    print(f'recieving server running at {ip}:1026')
    authorizer = DummyAuthorizer()
    authorizer.add_user('user', password, '.', perm='elradfmw')
    authorizer.add_anonymous('.', perm='elr')

    handler = FTPHandler
    handler.authorizer = authorizer
    address = (ip, 1026)
    server = FTPServer(address, handler)

    server.max_cons = 256
    server.max_cons_per_ip = 5

    server.serve_forever()

def sendFile(filePath, clientIP):
    """
    Sends a file to the client over tcp sockets
    Keyword arguments:

    filePath --  path of the file to send
    """
    print(f'sending file {filePath} at port: 12345')
    try:
        s = socket.socket()
        host = clientIP
        port = 12345
        fileName = filePath
        s.connect((host, port))
        f = open(fileName, mode='rb')
        print ('Sending...')
        byte = f.read(1024)
        while (byte):
            print ('Sending...')
            s.send(byte)
            byte = f.read(1024)

        f.close()
        print ("Done Sending")
        s.shutdown(socket.SHUT_WR)
        s.close()
    except Exception as identifier:
        print('Something happened.')
        print('Make sure both devices are on the same network, the file path is correct and and client app is listening')
        print(f'Error: {identifier}')

def main():
    time.sleep(2)
    wantToSendFile = False
    while not wantToSendFile:
        print('\nDo you want to send file (Y/N)')
        choice = input()
        if choice == 'Y':
            file = input('Enter file path (without quotes):\n')
            clientIP = input('Enter client IP address:\n')
            sendFile(str(file), str(clientIP))
        
        
if __name__ == "__main__":
    print("Note: Your input will be accepted even there's another prompt on screen")
    sendThread = threading.Thread(target=main)
    recieveThread = threading.Thread(target=recieveFile)
    recieveThread.start()
    sendThread.start()
