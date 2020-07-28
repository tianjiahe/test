/*
 功能:负责媒体流的收发,使用浏览器拉流(ie浏览器不能用,chrom或360可用)
 . nodejs的服务端代码,加载流媒体模块 node-media-server,
 . 配置2个端口,1935端口用rtmp协议收流,
 . 85号端口用http协议发流
 */
 
const NodeMediaServer = require('node-media-server');

const config = {
  rtmp: {
    port:       1935,     
    chunk_size: 60000,
    gop_cache:  true,
    ping:       30,
    ping_timeout: 60
  },
  http: {
    port: 8088,
    allow_origin: '*'
  }
};

var nms = new NodeMediaServer(config)
nms.run();

