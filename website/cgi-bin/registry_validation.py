#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# File: write-file.py

import cgitb; cgitb.enable()

import MySQLdb
conn = MySQLdb.connect(user='starclash', passwd='starclash', db='Starclash')
cursor = conn.cursor(MySQLdb.cursors.DictCursor)
cursor.execute("use Starclash")
import cgi

form = cgi.FieldStorage()
name = form.getfirst('name', "")
name = cgi.escape(name)

username = form.getfirst('username', "")
username = cgi.escape(username)

password = form.getfirst('password', "")
password = cgi.escape(password)

email = form.getfirst('email', "")
email = cgi.escape(email)

redirect = ''
user_error = '<font color=''green''>OK</font>'
name_error = '<font color=''green''>OK</font>'
psw_error = '<font color=''green''>OK</font>'
email_error = '<font color=''green''>OK</font>'

if username and password and name and '@' in email:
	cursor.execute("select count(player_id) from Player")
	count = cursor.fetchone()
	cursor.execute("insert Player set player_id='%s', username='%s', password='%s', email='%s', fname='%s'" % (count['count(player_id)'] + 1, username, password, email, name))
	conn.commit()
	redirect = '<meta HTTP-EQUIV="REFRESH" content="0;url=/newtemplate/home.html">'
else:
	redirect = '<meta HTTP-EQUIV="REFRESH" content="2;url=/newtemplate/download.html">'

if username == '':
	user_error = '<font color=''red''>Error</font>'
if name == '':
	name_error = '<font color=''red''>Error</font>'
if password == '':
	psw_error = '<font color=''red''>Error</font>'
if email == '':
	email_error = '<font color=''red''>Error</font>'

err = 'Proccessing'
if not username or not name or not password or '@' not in email:
	err = 'There was an error in processing your information'

print r"""Content-Type: text/html;charset=utf-8

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
				<td>Name</td>
				<td>%s</td>
			</tr>
			<tr>
				<td>Password</td>
				<td>%s</td>
			</tr>
			<tr>
				<td>Email</td>
				<td>%s</td>
			</tr>
		</table>
	</body>
</html>
""" % (redirect, err, user_error, name_error, psw_error, email_error)
