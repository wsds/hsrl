var client;
$(document).ready(function(){
	var transport = new Thrift.WebsocketTransport("ws://localhost:9090/");	
    var protocol  = new Thrift.Protocol(transport);
    client = new ShellClient(protocol);
	transport.setClient(client);
});

function test(){
	client.shell("123");
}

function shell(){
	var shell_text = $("#shell_text").val();
	if(shell_text!=undefined && shell_text!=""){
		client.shell(shell_text);
	}
}

function clear_rec(){
	$(".div2").html("");
}

function processLog(log){
	$(".div2").html($(".div2").html()+"<p>"+log+"</p>");
}