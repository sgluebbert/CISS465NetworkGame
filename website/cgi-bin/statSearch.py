#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# File: statSearch.py

import cgitb; cgitb.enable()
import cgi
import mysql.connector
from mysql.connector import errorcode
import os
import datetime

ipKey = 'REMOTE_ADDR'

now = datetime.datetime.now()

form = cgi.FieldStorage()
searchUser = form.getfirst('searchUser', '[NotFound]')
searchUser = cgi.escape(searchUser)

cnx = mysql.connector.connect(user='starclash', password='starclash', database='Starclash')
cursor = cnx.cursor()

userLocation = -1

cursor.execute("SELECT username, total_kills, shots_fired, total_deaths, accuracy, wins, losses, experience_points, captures, rank FROM Player")#, (searchUser))

data = cursor.fetchall()

# [username[0] for username in data].index(searchUser)
try:
	userLocation = [username[0] for username in data].index(searchUser)

	if userLocation >= 0:
		result = [data[userLocation][0], data[userLocation][1], data[userLocation][3], data[userLocation][2], data[userLocation][4], data[userLocation][5], 
		data[userLocation][6], data[userLocation][7], data[userLocation][8], data[userLocation][9]]
		kda = float(data[userLocation][1])/float(data[userLocation][2])
		"{:.3f}".format(kda)	

except (ValueError):
	result = ['No Such User', '-', '-', '-', '-', '-', '-', '-', '-', '-'] 
	kda = '-'




cursor.close()
cnx.close()

print r"""Content-Type: text/html;charset:utf-8


<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="stylesheet" type="text/css" href="../newtemplate/style.css" />
<title>Starclash</title>
</head>

<body>
    <div id="page">
		
        <div id="header">
        	<h1>Star Clash</h1>
            <h2>A game about.... </h2>
            
      </div>
  <div id="bar">
        	<div class="link"><a href="../newtemplate/home.html">home</a></div>
            <div class="link"><a href="../newtemplate/about.html">about</a></div>
            <div class="link"><a href="../newtemplate/registration.html">registration</a></div>
            <div class="link"><a href="../newtemplate/download.html">download</a></div>
            <div class="link"><a href="../newtemplate/gameplay.html">game play</a></div>
            <div class="link"><a href="../newtemplate/stats.html">stats</a></div>
            <div class="link"><a href="../newtemplate/contact.html">contact</a></div>
      </div>
        <div class="contentTitle"><h1>User Stats</h1></div>
        <div class="contentText">

          <p> 	
          <table style="color:#000000; border:1; align:center; bgcolor:#000000">

			<tr>
			<td>Username: %s</td> 
			<td>Kills: %s</td> 
			<td>Deaths: %s</td>
			<td>KDA: %s</td>
			<td>Shots Fired: %s</td> 
			<td>Accuracy: %s</td> 
			<td>Wins: %s</td>
			<td>Losses: %s</td>
			<td>XP: %s</td>	
			<td>Captures: %s</td>
			<td>Rank: %s</td>		
			</tr>
			</table>	

			<br>	

			<form method='get' action='statSearch.py'>
			What user would you like to view statistics for?
			<input type='text' value='' name='searchUser' />
			<input type='submit' />
			</form>	
			</p>
         
            </div>
		</div>
        
        
        <div class="cleaner h40"></div>

    
    	<div class="cleaner"></div>
    
    <div class="cleaner"></div>
</div> 
          
  
</div>
   		<div id="footer" style="backgroud-color:"#404040";clear:both;text-align:center;">
		This page was generated at %s for %s.</div>
</body>
</html>



"""%(result[0], result[1], result[2], kda, result[3], result[4], result[5], result[6], result[7], result[8], result[9], now, os.environ[ipKey])






