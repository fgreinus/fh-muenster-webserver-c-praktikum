#!/usr/bin/python
# -*- coding: utf-8 -*-
#
# This script makes a HTTP request to a specific server and compares
# responses with user-defined expected responses.
from __future__ import unicode_literals

import sys, socket

from pewpewlaz0rt4nk import (
    Beam,
    Laz0rCannon,
)

__version__ = '2.0.0'

# Set host and port.
# Read the target IP address and port from the command line arguments.
# If there are no command line arguments, use the following default values.
host = 'localhost'
port = 31337

# Overwrite host and port (if specified)
if len(sys.argv) == 3:
    _, host, port = sys.argv
    port = int(port)

# Initialise Laz0rCannon
cannon = Laz0rCannon(host=host, port=port)

# Laz0r Beams (Tests to fire against the server).
cannon += Beam(
    description='empty header',
    request='GET /../../htpasswd HTTP/1.1\r\nHost: intern\r\nAuthorization:\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='space header',
    request='GET / HTTP/1.1\r\nConnection: \r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Invalid method "SHRED"',
    request='SHRED /index.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 501']
)
cannon += Beam(
    description='Invalid Path"',
    request='GET ../index.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400 Bad Request']
)
cannon += Beam(
    description='long Path"',
    request='GET /index.html/index.html/index.html/index.html/index.htmlv/index.htmlv/index.html/index.html/index.html/index.html/index.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 404 Not Found']
)
cannon += Beam(
    description='Space before Method:Get"',
    request=' GET / HTTP/1.1\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Only whitespace after GET',
    request='GET ',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='New Line after GET',
    request='GET /\r\n',
    response=['HTTP/1.1 400']
)
#bad Uri Tests
cannon += Beam(
    description='Bad URI "/in%xydex.html"',
    request='GET /in%xydex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Bad URI "/in\\r\\ndex.html"',
    request='GET /in\r\ndex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Bad URI "/in dex.html"',
    request='GET /in dex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Bad URI "/in\\0dex.html"',
    request='GET /in\0dex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='GET /index.html only LF no CR',
    request='GET /index.html HTTP/1.1\nHost: {host}\nConnection: close\n\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='GET /index.html only CR no LF',
    request='GET /index.html HTTP/1.1\rHost: {host}\rConnection: close\r\r',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Bad URI "/in\\xffdex.html"',
    request='GET /in\xffdex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Bad URI "/in\\tdex.html"',
    request='GET /in\tdex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Bad URI "/in\\n\\rdex.html"',
    request='GET /in\n\rdex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Bad URI chars "/inäÖÜdex.html"',
    request='GET /inäÖÜdex.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='No URI',
    request='GET HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Try Access /index.html',
    request='GET /index.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Header ends wthout CRLF"',
    request='GET / HTTP/1.1\r\nHost: {host}\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Try Access intern',
    request='GET / HTTP/1.1\r\nHost: intern\r\n\r\n',
    response=['HTTP/1.1 401']
)
cannon += Beam(
    description='NULL before Header',
    request='GET /index.html HTTP/1.1\r\0\n\0Host: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='NULL before GET',
    request='\0GET /index.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='CRLF before GET',
    request='\r\nGET /index.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Request is double CRLF',
    request='\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Request is NULL',
    request='\0',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Access /images/tux. with . ',
    request='GET /images/tux. HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access /images/tux.png',
    request='GET /images/tux.png HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access /images/tux.xxx.jPG',
    request='GET /images/tux.xxx.jPG HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access /images/TUX1.PNG',
    request='GET /images/TUX1.PNG HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access /images/tux',
    request='GET /images/tux HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access pic with long filename',
    request='GET /images/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.png HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access with %20',
    request='GET /images/ein%20leerzeichen.png HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access /images/tux..pdf.JPg double ..',
    request='GET /images/tux..pdf.JPg HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access /images/tux.jpg',
    request='GET /images/tux.jpg HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access /js/javascript.js',
    request='GET /js/javascript.js HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access /test',
    request='GET /test HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Access not existing file',
    request='GET /foo.html HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 404']
)
cannon += Beam(
    description='Access /index.html without host header',
    request='GET /index.html HTTP/1.1\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description=': in header name',
    request='GET /index.html HTTP/1.1\r\nTest-He:ader: Foobar\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='GET with Entity-Body and without Content-Length',
    request='GET /index.html HTTP/1.1\r\nHost: {host}\r\n\r\nTESTTESTTEST',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='GET with Entity-Body and Content-Length: 10',
    request='GET /index.html HTTP/1.1\r\nContent-Length: 10\r\nHost: {host}\r\n\r\nTESTTESTTE',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Request with chars as content-length',
    request='GET /index.html HTTP/1.1\r\nContent-Length: test\r\nHost: {host}\r\n\r\nTEST',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Invalid request',
    request='content-length: 183\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400 Bad Request']
)
cannon += Beam(
    description='Cyrillic uri',
    request='GET /Кирилица HTTP/1.1\r\nHost: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Cyrillic in header name',
    request='GET / HTTP/1.1\r\nКирилица: {host}\r\n\r\n',
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='Cyrillic in header value',
    request='GET / HTTP/1.1\r\nHost: Кирилица\r\n\r\n',
    response=['HTTP/1.1 200']
)
cannon += Beam(
    description='Closing socket',
    request='GET / HTTP/1.1\r\nHost: whatever\r\n\r\n',
    response=['HTTP/1.1 200'],
    shutdown=socket.SHUT_RDWR
)
cannon += Beam(
    description='extrem lange request',
    request='\r\n'.join("""GET /index.html HTTP/1.1
Host: intern:31337
User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:52.0) Gecko/20100101 Firefox/52.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Connection: close
Upgrade-Insecure-Requests: 1
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Connection: close
Upgrade-Insecure-Requests: 1
Content-Length: 788023
""".split()) + '\r\n'.join("""
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Connection: close
Upgrade-Insecure-Requests: 1""".split()) * 10,
    response=['HTTP/1.1 400']
)
cannon += Beam(
    description='host header is several whitespaces',
    request='GET /index.html HTTP/1.1\r\nHost:      \r\n\r\n',
    response=['HTTP/1.1 400 Bad Request']
)
cannon += Beam(
    description='another header is several whitespaces',
    request='GET /index.html HTTP/1.1\r\nHost: {host}\r\nConnection:     \r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
##Unauthorized
cannon += Beam(
    description='test1:test1test1:',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGVzdDE6dGVzdDF0ZXN0MTo=\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 401 Unauthorized']
)
cannon += Beam(
    description='test:testtest',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGVzdDp0ZXN0dGVzdA==\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='test:testtest spaces after Basic',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic   dGVzdDp0ZXN0dGVzdA==\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='test:testtest spaces before Basic',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization:   Basic dGVzdDp0ZXN0dGVzdA==\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='test:testtest spaces before and after Basic',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization:   Basic   dGVzdDp0ZXN0dGVzdA==\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='test:testtest space after Base64',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGVzdDp0ZXN0dGVzdA== \r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='te  st1:test1  test1',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGUgIHN0MTp0ZXN0MSAgdGVzdDE=\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 401 Unauthorized']
)
cannon += Beam(
    description=':test1:test1test1',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic OnRlc3QxOnRlc3QxdGVzdDE=\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 401 Unauthorized']
)
cannon += Beam(
    description='test1:test1test1',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGVzdDE6dGVzdDF0ZXN0MQ==\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 200 OK']
)
cannon += Beam(
    description='test1:testtest',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGVzdDE6dGVzdHRlc3Q=\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 401 Unauthorized']
)
cannon += Beam(
    description='test1:test1test1:',
    request='GET /index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGVzdDE6dGVzdDF0ZXN0MTo=\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 401 Unauthorized']
)
cannon += Beam(
    description='extern obwohl intern',
    request='GET /../../extern/index.html HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGVzdDp0ZXN0dGVzdA==\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 404']
)
cannon += Beam(
    description='htpasswd',
    request='GET /../../htpasswd HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic dGVzdDp0ZXN0dGVzdA==\r\nConnection: close\r\n\r\n',
    response=['HTTP/1.1 404']
)


# Pew pew!
cannon.pewpew()
