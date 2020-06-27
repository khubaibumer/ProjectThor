#! /bin/bash

# Author: Muhammad Khubaib Umer
# Dated: 27-June-2020
# Description: Install Script for Project THOR

if [ $UID -eq 0 ]
then
	printf "Please Do Not Run Installer as ROOT\n"
	exit 1
fi

USR=$USER

printf "||=============================================================||\n"
printf "||              Welcome to Project THOR                        ||\n"
printf "||=============================================================||\n"

printf "This Script will help you install Project THOR . . .\n\n"

printf "Project THOR Depends on the following Packages\n"
printf "  - libssl-dev\n"
#printf "  - libcrypto++-dev\n"
printf "  - libsqlite3-dev\n\n"

# Assuming that we need to install these dependencies lets dowload and install them

if ! sudo apt install libssl-dev libsqlite3-dev --install-recommends --upgrade
then
	printf "\nPlease Correct the Issues mentioned above and Retry\n"
	exit 1
fi

printf "\n\nPlease Enter Installation Directory: "
read -r DIR

printf "\n\nProject THOR will be installed to $DIR \n"

CFG=$DIR/thor.cfg

sudo mkdir -p $DIR
sudo touch $CFG
sudo chown $USR $DIR -R

printf "\n\nGenerating Configuration File . . .\n"

printf "Enter IP Address to Bind Server: "
read -r IP

printf "Enter PORT to Bind Server: "
read -r PORT

printf "Do you want to Create a new Certificate file? [y/n]: "
read -r NC

if [ "$NC" = "y" ]
then
	printf "Enter Certificate File Name: "
	read -r CERT
	CERTP=$DIR/$CERT
	openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout $CERTP -out $CERTP
else
        printf "Enter Existing Certificate File Path: "
        read -r CERTP
fi

printf "Enter Logfile Name: "
read -r LOGFILE

printf "Do you Want to Encrypt Logfile? [y/n]: "
read -r ENC

printf "Enter DataBase Name: "
read -r DB

printf "\n\nConfiguration File is being Generated\n"

sleep 2

echo "IP=$IP" > $CFG
echo "PORT=$PORT" >> $CFG
echo "CERT=$CERTP" >> $CFG
echo "LOG=$DIR/$LOGFILE" >> $CFG

if [ "$ENC" = "y" ]
then
	echo "SEC=1" >> $CFG
else
	echo "SEC=0" >> $CFG
fi

echo "DB=$DIR/$DB" >> $CFG

printf "\n\n## EOF\n" >> $CFG

printf "\n\nConfiguration File Generated Successfuly with parameters\n\n"
cat $CFG

printf "\n\nInstalling Server to Path Specified . . . \n\n"
cp Debug/THOR $DIR

#if [ "$ENC" = "y" ]
#then
#
#	printf "\n\nPlease Enter Username and Password to Secure DataBase\n"
#	printf "Username: "
#	read -s UsrName
#	echo ""
#
#	printf "Password: "
#	read -s UsrPass
#	echo ""
#
#	printf "ID=$UsrName\n"
#	printf "Pass=$UsrPass\n"
#
#fi

