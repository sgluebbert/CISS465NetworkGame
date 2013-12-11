#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# File: input-text-3.py
import cgitb; cgitb.enable()    
import navi_bar

print r"""Content-Type: text/html;charset=utf-8

<html>
	%s
	<link rel="stylesheet" href="http://netdna.bootstrapcdn.com/twitter-bootstrap/2.3.1/css/bootstrap-combined.min.css" />
	<style>
	.container { }
	</style>
		
		<title> User Registration </title>
<body>

<div class="container">
<form method='get' action='registry_validation.py'>

	<label for="username">Username:</label>
	<input type = 'text' value='' name='username'/><br />

	<label for="username">Password:</label>
	<input type = 'text' value='' name='password'/><br />

	<label for="username">Email:</label>
	<input type = 'text' value='' name='email'/><br />
	<input type = 'submit' />
</form>
</div>

</body>
</html>
""" % navi_bar.navi_bar()


