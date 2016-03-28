'use strict';

let http=require('http');
let url=require('url');
let redis=require('redis');
let log4js=require('log4js');

let addon = require('./build/Release/addon.node');

/**
 *建立http服务器
 */
let server=http.createServer(function(req,res){
  let cid=url.parse(req.url).query;

  let keys=addon.hashkeys(cid,cid.length);  

});

//监听端口
server.listen(PORT);
