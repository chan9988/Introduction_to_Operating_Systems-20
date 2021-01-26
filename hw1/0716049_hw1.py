import hashlib
import requests
import random
import time
import threading
import multiprocessing
import asyncio

def hash(s):
    ch=0
    while ch==0:
        for j in range (0,5):
            a=random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()"')
            s=a+s
        m=hashlib.sha256()
        m.update(s.encode('utf8'))    
        #print(m.hexdigest())
        ha=int(m.hexdigest()[0:5],16)
        if ha==0:
            ch=1
        else:
            s=s[5:10]
    print(s)

async def a_hash(s):
    ch=0
    while ch==0:
        for j in range (0,5):
            a=random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()"')
            s=a+s
        m=hashlib.sha256()
        m.update(s.encode('utf8'))    
        #print(m.hexdigest())
        ha=int(m.hexdigest()[0:5],16)
        if ha==0:
            ch=1
        else:
            s=s[5:10]
    print(s)



def http(url):
    r=requests.get(url)
    front=r.text.find("<title>")
    end=r.text.find("</title>")
    title=r.text[front+7:end]
    print(title)

async def a_http(url):
    r=requests.get(url)
    front=r.text.find("<title>")
    end=r.text.find("</title>")
    title=r.text[front+7:end]
    print(title)



if __name__ == '__main__':
    task=int(input())
    m=input()
    if m[0]=='1' or m[0]=='2':
        (m,k)=m.split(" ")
        k=int(k)
    m=int(m)
    n=int(input())
    start_time=time.time()
    if task==1:
        if  m==2:
            process=[]
            for i in range (0,n):
                s=input()
                process.append(multiprocessing.Process(target=hash,args=(s,))) 
            cnt=0
            i=0
            while i<n:
                if  len(multiprocessing.active_children())<k:
                    process[i].start()
                    i+=1
            for i in range (0,n):
                process[i].join() 
        if  m==1:
            thread=[]
            for i in range (0,n):
                s=input()
                thread.append(threading.Thread(target=hash,args=(s,)))
            cnt=0
            i=0
            while i<n:
                if  threading.active_count()<k+1:
                    thread[i].start()
                    i+=1
            for i in range (0,n):
                thread[i].join() 
        if m==3:
            for i in range(0,n):
                s=input()
                asyncio.run(a_hash(s))
    else:
        if  m==2:
            process=[]
            for i in range (0,n):
                s=input()
                process.append(multiprocessing.Process(target=http,args=(s,))) 
            cnt=0
            i=0
            while i<n:
                if  len(multiprocessing.active_children())<k:
                    process[i].start()
                    i+=1
            for i in range (0,n):
                process[i].join() 
        if  m==1:
            thread=[]
            for i in range (0,n):
                s=input()
                thread.append(threading.Thread(target=http,args=(s,)))
            cnt=0
            i=0
            while i<n:
                if  threading.active_count()<k+1:
                    thread[i].start()
                    i+=1
            for i in range (0,n):
                thread[i].join() 
        if m==3:
            for i in range(0,n):
                s=input()
                asyncio.run(a_http(s))

    end_time=time.time()
    print("This program spends ",end_time-start_time, "seconds")