#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# File: Home.py

import cgitb; cgitb.enable()
import navi_bar

print r"""Content-Type: text/html;charset=utf-8

<html>
	%s
	<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/twitter-bootstrap/2.3.1/css/bootstrap-combined.min.css" />
	<div class="container">
	<body>
	<p>This is the about page</p>


	</body>
	</div>
</html>
""" % navi_bar.navi_bar()
