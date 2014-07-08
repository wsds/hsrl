Thrift.WebsocketTransport = function(url) {
	///	"ws://localhost:9090/test";
    this.host = url;
	/// add for websocket
	this.websocket = null;
	this.client = null;
	this.receivedResponse = false;
};

Thrift.WebsocketTransport.prototype = new Thrift.Transport(this.host);

Thrift.WebsocketTransport.prototype.send = function(msg){
	//assert(this.websocket != null);
	this.websocket.send(msg);
};

Thrift.WebsocketTransport.prototype.connect = function(){	
	try{
		var This = this;
		
		this.websocket = new WebSocket(this.host);		
		console.log('Socket Status: '+this.websocket.readyState);
		this.websocket.onopen = function(){
			console.log('Socket Status: '+this.readyState+' (open)');
		}
		
		this.websocket.onmessage = function(msg){
			console.log('received message: '+msg.data);
			This.setRecvBuffer(msg.data);
			
			try{
				var msgobj;
				if (typeof JSON !== 'undefined' && typeof JSON.parse === 'function') {
					msgobj = JSON.parse(msg.data);
				} else if (typeof jQuery !== 'undefined') {
					msgobj = jQuery.parseJSON(msg.data);
				} else {
					msgobj = eval(msg.data);
				}
				if(msgobj[0] = 1){
					var ret = This.client.input.readMessageBegin();
					var fname = ret.fname;
					var mtype = ret.mtype;
					var rseqid = ret.rseqid;
					This.client.input.readMessageEnd();
					
					/// 如果请求的响应也使用回调方式； 不过，现在请求会使用ajax的方式同步进行
					var method = (mtype == Thrift.MessageType.CALL) ? "notice_":"recv_";
					method += fname;
					console.log('mothod: '+method);

					This.client[method]();	
				}	
			}catch(e){
				processLog(msg.data);
			}
		};		
		
		this.websocket.onclose = function(){
			console.log('Socket Status: '+this.readyState+' (Closed)');
		}				
	} catch(exception){
		console.log('Error'+exception);
	}
};
/// if use http for client, not overwrite flush function.
Thrift.WebsocketTransport.prototype.flush = function(async) {
	console.log('sending:'+this.send_buf);	
	this.websocket.send(this.send_buf);	
};

Thrift.WebsocketTransport.prototype.setClient = function(c){
	this.client = c;
	this.connect();
};