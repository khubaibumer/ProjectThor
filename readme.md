# How to use THOR Server

Interfacing Server for c-lightning

## [![CodeFactor](https://www.codefactor.io/repository/github/khubaibumer/libthor/badge?s=431fd03279c42ac1ec2eaa4238b0b139d9388921)](https://www.codefactor.io/repository/github/khubaibumer/libthor)

## [![Build Status](https://travis-ci.com/khubaibumer/libThor.svg?token=1CSkg4TqiLRRZPUNDexe&branch=master)](https://travis-ci.com/khubaibumer/libThor)

## [![Codacy Badge](https://app.codacy.com/project/badge/Grade/519d1d8ac4b94317b3f344d12e39eeb5)](https://www.codacy.com/gh/khubaibumer/ProjectThor/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=khubaibumer/ProjectThor&amp;utm_campaign=Badge_Grade)


## Installation

1. Run "sudo chmod a+x ./INSTALL.sh"
2. Run "./INSTALL.sh"
3. Follow the proceedure
4. INSTALL.sh should exit with "Installation Done!"

## Execute THOR

1. Open a Terminal
2. Execute "/thor/THOR"
3. As soon as it shows a QR Code you should be able to connect to it
4. If QR Code is not shown please refer to the logfile in /thor directory

## Using openssl as client

To use openssl are a Client

1. Open a terminal
2. Execute "openssl s_client -connect IP:PORT"
3. You will get "auth,who" in the terminal
4. Enter Credentials ( for now just send "auth,admin,admin")
5. You will recieve "auth,ok,0" in the case of success or "auth,fail,-1" in case of failure
6. Failure can only be due to invalid credentials

## Using RPC Command Line

After you have "auth,ok,0" on login you will be able to run all allowed commands including rpc

To use RPC Command Line Interface use the following

### Basic RPC Command Structure

RPC Command Compromises of
 > rpc-cmd,(process),(lat_long),(txId),[ (arguments to process) ]
For Example you want to run "lightning-cli newaddr" the rpc varient will be

 > rpc-cmd,cli-node,12.321_-91.123,1,[ newaddr ]
 
* Please Note that there are no Spaces in Between ',' and ONE Space each between '['<space> args <space>']'

## DataBase Interface

### User Information

#### Add User

 > db,add,user,(userId),(userPw),(userMode)
 
#### Delete User
 
 > db,del,user,(userId)
 
#### Update User Information

 > db,update,user,(userId),(key),(value)
 
 Where keys are:
 * userid
 * userpass
 * usermode
 * userlogged
 
#### Get List of Users

 > db,get-list,user
 
### Items Information

#### Add Item

 > db,add,items,(upc),(quantity),(price),(name)
 
#### Delete Items

 > db,del,items,(upc)
 
#### Update Items

 > db,update,items,(upc),(key),(value)
 
 Where keys are:
 * item-name
 * item-quantity
 * item-price
 * item-upc
 
#### Get List of Items

 > db,get-list,items

### Log Data

 > log-cmd,(process),(lat_long),(txId),(data)
 
### Image Database

#### Add Image

 > db,add,images,(upc),(image)
 
#### Delete Image

 > db,del,images,(upc)
 
#### Update Images

 > db,update,images,(upc),(key),(value)
 
 Where keys are:
 * item-upc
 * images
 
#### Get List of UPCs in Image DB

 > db,get-list,images
 
#### Get Image for a UPC

 > db,get-image,images,(upc)
 
### Control Interface

#### Get Applicable Tax

 > control,get-tax
 
#### List Peers (active users)

 > control,list-peers
 
#### Set Logfile Logging Level

 > control,set-logging,(logging level)
 
#### Get Logfile Logging Level

 > control,get_logging
 
#### Get List of Available Logging Levels

 > control,get-log-levels

 **IMPORTANT NOTE**

 * All Commands to the server must end with footer ",<$#EOT#$>"
 * Any command not ending on this footer will cause the server to hang
