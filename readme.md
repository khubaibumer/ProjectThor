# How to use THOR Server

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
 > rpc-cmd,(process),(lat_long),[ (arguments to process) ]
For Example you want to run "lightning-cli newaddr" the rpc varient will be

 > rpc-cmd,cli-node,12.321_-91.123,[ newaddr ]
 
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

 > db,add,items,(name),(quantity),(price),(extra)
 
#### Delete Items

 > db,del,items,(name)
 
#### Update Items

 > db,update,items,(name),(key),(value)
 
 Where keys are:
 * item-name
 * item-quantity
 * item-price
 * item-extra
 
#### Get List of Items

 > db,get-list,items
