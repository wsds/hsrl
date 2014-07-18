var client;
var transport;
var protocol;
var shells = [];
var shells_index = 0;
var shells_current_index = 0;

var log_area;
var variables_area;
var shell_area;


$(document).ready(function(){
	shell_area = $("#shell_text");
	variables_area = $(".variables_area");
	log_area = $(".log_area");
	connect();
});

function shell(){
	var shell_text = shell_area.val();
	if(shell_text!=undefined && shell_text!=""){
		client.shell(shell_text);
		if(shell_text!=shells[shells_index-1]){
			shells_current_index = shells_index;
			shells[shells_index++] = shell_text;
		}
	}
}

function clear_rec(){
	log_area.html("");
}

function clear_variables(){
	variables_area.html("");
}


function processLog(log){
	var type = log.substring(0,2);
	var content = log.substring(2);
	if(type=="1."){
		log_area.html(log_area.html()+"<p>"+content+"</p>");
	}else if(type=="2."){
		variables_area.html(variables_area.html()+"<p>"+content+"</p>");
	}
}

function continue_run(){
	client.shell("CONTINUERUN");
}

function show_variables(){
	client.shell("GETALLVARABLES");
}

function connect(){
	if(transport==undefined ||transport.websocket==undefined || transport.websocket.readyState!=1){
		transport = new Thrift.WebsocketTransport("ws://localhost:9090/");	
		protocol  = new Thrift.Protocol(transport);
		client = new ShellClient(protocol);
		transport.setClient(client);
	}
}

function previous_execute(){

	var shell_text = shell_area.val();
	if(shell_text!=""&&shell_text!=shells[shells_current_index]){
		shells_current_index = shells_index;
		shells[shells_index++] = shell_text;
	}
	if(shells_current_index>0&&shells_current_index<shells_index){
		shell_area.val(shells[--shells_current_index]);
	}
}

function next_execute(){
	if(shells_current_index>=0&&shells_current_index<shells_index-1){	
		shell_area.val(shells[++shells_current_index]);
	}
}