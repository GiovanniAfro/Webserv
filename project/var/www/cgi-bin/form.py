#!/usr/bin/python3

import sys
import os

content_length = int(os.environ.get('CONTENT_LENGTH', 0))
post_data = sys.stdin.read(content_length)

print("Content-type: text/plain\n")
print(f"post_data: {post_data}\ncontent_lenght: {content_length}")

data = {}
if post_data:
    pairs = post_data.split('&')
    print(f"pairs: {pairs}")
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
print(f"<p>Email: {email}</p>")
print("</body></html>")
