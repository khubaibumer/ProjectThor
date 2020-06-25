# libThor
Interfacing Library for c-lightning

## :: LIST OF COMMANDS ::

> db,add,user,<name>,<pass>,<mode>,<extra>
  
> db,del,user,<name>{,<pass>,<mode>,<extra>}
  
> db,update,user,<name>,<key>,<value>
  
> db,get-list,user

> db,get-list,items

> db,update,items,<name>,<key>,<value>
  
> db,add,items,<name>,<quantity>,<price>,<extra>
  
> db,del,items,<name>,<quantity>,<price>{,<extra>}

> rpc-cmd,cli-node,[command to execute]


## :: LIST OF RESPONSE ::

> resp,ok,0,response,<response>
  
> resp,fail,reason,<reason>
  
> resp,status,<status>
  

## :: RPC STRUCTURE ::

> rpc-type,rpc-action,rpc-identifier,Args...

