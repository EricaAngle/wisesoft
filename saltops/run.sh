#!/bin/sh
if [ -d /home/fluent ]
then
cd /home/fluent
fi

if [ -f /home/fluent/master ]
then
   #cat /home/fluent/master
   echo "file 'master' existed"
else
   echo "file not exists"
fi

if [ -f settings.py ]
then 
   cp settings.py settings.py.orig
   if [ -n $salt_url ] 
   then
      sed -i "s/^SALT_REST_URL \+= \+'http:\/\/[0-9.]*:[0-9]*\/'/SALT_REST_URL = '$salt_url'/g" settings.py
   else
      echo $salt_url
   fi
  
   if [ -n $salt_user ]
   then
      sed -i "s/^SALT_USER \+= \+'[a-zA-Z0-9]*'/SALT_USER = '$salt_user'/g" settings.py
   fi

   if [ -n $salt_password ]
   then
      sed -i "s/^SALT_PASSWORD \+= \+'[a-zA-Z0-9]*'/SALT_PASSWORD = '$salt_password'/g" settings.py
   fi
fi
