#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# File: write-file.py

import cgitb; cgitb.enable()

import MySQLdb
conn = MySQLdb.connect(user='root', passwd='root', db='starclash')
cursor = conn.cursor(MySQLdb.cursors.DictCursor)
cursor.execute("use starclash")
import cgi

form = cgi.FieldStorage()
username = form.getfirst('user', "")
username = cgi.escape(username)
password = form.getfirst('pass', "")
password = cgi.escape(password)
user = None
redirect = ''
user_error = '<font color=''green''>OK</font>'
psw_error = '<font color=''green''>OK</font>'
email_error = '<font color=''green''>OK</font>'
make_cookie = 1;
cookie = ''

if username and password:
	cursor.execute("select * from user where user_name = '%s'" % username)
	user = cursor.fetchone()
	redirect = '<meta HTTP-EQUIV="REFRESH" content="0;url=home.py">'
else:
	redirect = '<meta HTTP-EQUIV="REFRESH" content="2;url=home.py">'

err = 'Proccessing'
if not username or not password:
	err = 'There was an error in processing your information'
if username == '' or username != user['user_name'] or user == None:
	user_error = '<font color=''red''>Error</font>'
	make_cookie = 0
if password == '' or password != user['password'] or user == None:
	psw_error = '<font color=''red''>Error</font>'
	make_cookie = 0

if make_cookie:
	cookie = 'Set-Cookie: username=%s' % user['user_name']

print r"""%s 
Content-Type: text/html;charset=utf-8
<html>
	%s
	<center><h2>%s</h2></center>
	<body>
		<table cellspacing='10'>
			<tr>
				<td>Username</td>
				<td>%s</td>
			</tr>
			<tr>
				<td>Password</td>
				<td>%s</td>
			</tr>
		</table>
	</body>
</html>
""" % (cookie, redirect, err, user_error, psw_error)

