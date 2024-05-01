#!/usr/bin/python3

import sys
import os

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
post_data = sys.stdin.read(content_length)

print("Content-type: text/plain\n")
print("post_data: {}".format(post_data))

data = {}
if post_data:
    pairs = post_data.split('&')
    print("pairs: {}".format(pairs))
    for pair in pairs:
        key, value = pair.split('=')
        data[key] = value

name = data.get('name', '')
email = data.get('email', '')

print("Content-type: text/html\n")
print("<html><head><title>Form Submission</title></head>")
print("<body>")
print("<h1>Thank you for submitting the form!</h1>")
print(f"<p>Name: {name}</p>")
print(f"<p>CONTENT LENGHT: {content_length}</p>")
print(f"<p>Email: {email}</p>")
print("</body></html>")
