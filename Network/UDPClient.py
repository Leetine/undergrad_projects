#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from socket import *

buffer_size = 1024
ip_port = ("192.168.0.105", 5000)

client = socket(family=AF_INET, type=SOCK_DGRAM)

while True:
    try:
        msg = input(">>:").strip().encode("UTF8")
        if not msg: continue
        client.sendto(msg, ip_port)
        print("客户端消息发送成功！")

        msg_get, addr = client.recvfrom(buffer_size)
        print("收到来自%s服务器的消息为：%s" % (addr, msg_get.decode("UTF8")))
    except Exception as e:
        print(e)
client.close()

