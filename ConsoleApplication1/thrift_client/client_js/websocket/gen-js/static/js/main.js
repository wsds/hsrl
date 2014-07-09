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

function clear_variables(){
	$(".div3").html("");
}


function processLog(log){
	var type = log.substring(0,2);
	var content = log.substring(2);
	if(type=="1."){
		$(".div2").html($(".div2").html()+"<p>"+content+"</p>");
	}else if(type=="2."){
		$(".div3").html($(".div3").html()+"<p>"+content+"</p>");
	}
}

function continue_run(){
	client.shell("CONTINUERUN");
}

function show_variables(){
	client.shell("GETALLVARABLES");
}