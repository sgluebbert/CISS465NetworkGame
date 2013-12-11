#!/usr/bin/env python

import smtplib, imaplib
import email
from email.MIMEMultipart import MIMEMultipart
from email.MIMEBase import MIMEBase
from email.MIMEText import MIMEText
from email.MIMEImage import MIMEImage
from email import Encoders, encoders
import mimetypes

from gmail_config import *

def mail(to,
         subject,
         from_=USER,
         text=None, html=None,
         servers=SMTP_SERVERS,
         port=SMTP_PORT,
         user=USER,
         password=PASSWORD,
         ):

   msg = MIMEMultipart()
   msg['From'] = from_
   msg['To'] = to
   msg['Subject'] = subject

   alternative = MIMEMultipart('alternative')
   msg.attach(alternative)

   if text:
      alternative.attach(MIMEText(text))

   if html:
      alternative.attach(MIMEText(html, 'html'))

   AUTHREQUIRED = 1

   for server in servers:
      print "trying server", server
      try:
         mailServer = smtplib.SMTP(server, port)
         mailServer.set_debuglevel(1)
         mailServer.ehlo()
         mailServer.starttls()
         #mailServer.ehlo()
         mailServer.login(user, password)
         mailServer.sendmail(from_, to, msg.as_string())
         # Not mailServer.quit() or it will crash
         mailServer.close()
         break
      except Exception, e:
         print e
         print
         print

      
if __name__ == "__main__":

   print "test ... \n"
   FROM = 'contactstarclash@gmail.com'
   TO = 'contactstarclash@gmail.com'
   SUBJECT = "This is the subject"
   TEXT = "This is a test. hello world."
   HTML = r"""
<html>
<body>
<h1>This is a test</h1>
hello world
</body>
</html>
"""
   
   mail(to = TO,
        from_ = FROM,
        subject = SUBJECT,
        text = TEXT,
        html = HTML,
        )
