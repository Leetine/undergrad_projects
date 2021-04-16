#!/usr/bin/env python3
# -*- coding: utf-8 -*-
from socket import *

buffer_size = 1024
ip_port = ("192.168.0.105", 5000)

server = socket(family=AF_INET, type=SOCK_DGRAM)
server.bind(ip_port)
print("Starting to receive packet…")
while True:
    try:
        msg_get, addr = server.recvfrom(buffer_size)
        print("收到来自%s客户端的消息为：%s" % (addr, msg_get.decode("UTF8")))

        msg = input(">>:").strip().encode("UTF8")
        if not msg: continue
        server.sendto(msg, addr)
        print("UDP服务器端消息发送成功！")

    except Exception as e:
        print(e)
        break
server.close()

