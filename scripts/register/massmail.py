#!/usr/bin/env python

import MySQLdb
import re
import sys
import smtplib


dbname=sys.argv[1]
template=sys.argv[2]
sql=sys.argv[3]

server=smtplib.SMTP('localhost')
db=MySQLdb.connect(db=dbname)
From="accounts@vinyambar.de"

cursor=db.cursor()
query=("select distinct users.id, users.email, users.firstname "+
  "from users, games, subscriptions "+
  "where users.id=subscriptions.user and subscriptions.game=games.id and "+
  sql)

users=cursor.execute(query)
print "Sending confirmation to "+str(int(users))+" users"
while users!=0:
    users=users-1
    entry=cursor.fetchone()
    custid=str(int(entry[0]))
    email=entry[1]
    firstname=entry[2]

    infile=open(template,"r")
    line = infile.read()

    line = re.sub('<CUSTID>', custid, line)
    line = re.sub('<FIRSTNAME>', firstname, line)
#    line = re.sub("<GAMENAME>", game, line)

    Msg = ("From: "+From+"\nTo: "+email+"\n"+
      line)

    try:
        server.sendmail(From, email, Msg)
        update=db.cursor()
        update.execute("UPDATE users set status='PENDING' WHERE id="+custid)
	print "Sent billing information to "+email

    except:
        print "Could not inform "+To
        print "Reason was: '"+Reason+"'"
        print "Exception is:", sys.exc_type, ":", sys.exc_value

    infile.close()
