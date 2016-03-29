'use strict';

let http=require('http');
let URL=require('url');
let redis=require('redis');
let log4js=require('log4js');

let addon = require('./build/Release/addon.node');
let CONF=require('./config');

//配置log4js
log4js.configure({
  appenders:[
    {
      "type":"file",
      "filename":"./logs/server.log",
      "maxLogSize":CONF.maxLogSize,
      "backups":2,
      "category":"sys"
    }
  ]
},{});

let logSys=log4js.getLogger('sys'); 

/**
 *redis客户端
 */
let client=undefined;
function conn(){
  client=redis.createClient(CONF.REDIS_PORT,CONF.REDIS_IP,CONF.REDIS_CONF);

  client.on('error',function(err){
    client=undefined;
    logSys.error('redis conn error.');
    setTimeout(conn,5000);
  });
}
conn();

/**
 *建立http服务器
 */
let server=http.createServer(function(req,res){
  let url=req.url;
  //访问记录
  logSys.info(url);

  let cid=URL.parse(req.url).query;
  let len=undefined;
  if (!cid||(len=cid.length)<1){
    res.writeHead(500,{});
    res.end('cid is null');
    return;
  }

  let keys=addon.hashkeys(cid,cid.length);  
  
  if (!client){
    res.writeHead(500,{});
    res.end('redis is null');
    return;
  }
	  
  let multi=client.multi();

  keys.forEach(function(item){
     multi.setbit('bloomfilter',item,1); 
  });

  multi.exec(function(err,reply){
    if (err){
      print("{'success':false,'cid':"+cid+"}");
    }else{
      print("{'success':true,'cid':''}")
    }

    //打印结果
    function print(result){
      res.writeHead(200,{'Content-Type':'application/json'});
      res.end(result);
    }
  });
});

//监听端口
server.listen(CONF.HTTP_PORT);

/**
 *异常处理
 */
process.on('uncaughtException',function(err){
  logSys.error('uncaughtException:'+err);
});

