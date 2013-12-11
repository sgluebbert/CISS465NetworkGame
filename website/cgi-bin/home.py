#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# File: Home.py

import cgitb; cgitb.enable()
import navi_bar
import strings
import os

sign_in = strings.home_sign_in_form

cookie = os.environ.get('HTTP_COOKIE', 'no cookie')
#log_in.login()

print r"""Content-Type: text/html;charset=utf-8

<html>
	<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/twitter-bootstrap/2.3.1/css/bootstrap-combined.min.css" />
	%s
	<div class="container">
	<body>
		<p>This is the Home Page</p>
	%s
	</body>
	</div>
	
</html>
""" % (navi_bar.navi_bar(), sign_in)
