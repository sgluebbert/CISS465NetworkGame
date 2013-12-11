#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# File: Contact_Us.py

import cgitb; cgitb.enable()
import navi_bar

print r"""Content-Type: text/html;charset=utf-8

<html>
	%s
	<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/twitter-bootstrap/2.3.1/css/bootstrap-combined.min.css" />
	<div class="container">
	<body>
	<p>This is the contact us page</p>

	<form action="/cgi-bin/email/send_email.py" method="post">
		<label for="bug">Bug Report:</label>
		<input type="radio" id="buzg" name="type" value="bug" />
		
		<label for="feature">Feature Request:</label>
		<input type="radio" id="feature" name="type" value="feature" />
		
	

		<label for="body">Enter your message here: </label>
		<TEXTAREA Name="body" ROWS=4 COLS=30></TEXTAREA>

		<br /><input type ="submit" />
	</form>

	</body>
	</div>
</html>
""" % navi_bar.navi_bar()
