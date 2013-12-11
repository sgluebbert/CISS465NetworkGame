#!/usr/bin/env python


import cgitb; cgitb.enable()
import sendgmail

import cgi

form = cgi.FieldStorage()

body = form.getfirst('msg', '[NO MESSAGE]')
body = cgi.escape(body)

name = form.getfirst('name', '[NO NAME]')
name = cgi.escape(name)

email = form.getfirst('email', '[NO EMAIL]')
email = cgi.escape(email)

sendgmail.mail('contactstarclash@gmail.com', name + ' ' + email, text = body)
		
print r"""Content-Type: text/html;charset=utf-8

<html>
'<meta HTTP-EQUIV="REFRESH" content="0;url=/newtemplate/email_sent.html">'
</html
"""
		

