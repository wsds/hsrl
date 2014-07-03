//
// Autogenerated by Thrift Compiler (0.9.1)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//


//HELPER FUNCTIONS AND STRUCTURES

Shell_shell_args = function(args) {
  this.query = null;
  if (args) {
    if (args.query !== undefined) {
      this.query = args.query;
    }
  }
};
Shell_shell_args.prototype = {};
Shell_shell_args.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
      if (ftype == Thrift.Type.STRING) {
        this.query = input.readString().value;
      } else {
        input.skip(ftype);
      }
      break;
      case 0:
        input.skip(ftype);
        break;
      default:
        input.skip(ftype);
    }
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

Shell_shell_args.prototype.write = function(output) {
  output.writeStructBegin('Shell_shell_args');
  if (this.query !== null && this.query !== undefined) {
    output.writeFieldBegin('query', Thrift.Type.STRING, 1);
    output.writeString(this.query);
    output.writeFieldEnd();
  }
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

Shell_shell_result = function(args) {
};
Shell_shell_result.prototype = {};
Shell_shell_result.prototype.read = function(input) {
  input.readStructBegin();
  while (true)
  {
    var ret = input.readFieldBegin();
    var fname = ret.fname;
    var ftype = ret.ftype;
    var fid = ret.fid;
    if (ftype == Thrift.Type.STOP) {
      break;
    }
    input.skip(ftype);
    input.readFieldEnd();
  }
  input.readStructEnd();
  return;
};

Shell_shell_result.prototype.write = function(output) {
  output.writeStructBegin('Shell_shell_result');
  output.writeFieldStop();
  output.writeStructEnd();
  return;
};

ShellClient = function(input, output) {
    this.input = input;
    this.output = (!output) ? input : output;
    this.seqid = 0;
};
ShellClient.prototype = {};
ShellClient.prototype.shell = function(query) {
  this.send_shell(query);
  this.recv_shell();
};

ShellClient.prototype.send_shell = function(query) {
  this.output.writeMessageBegin('shell', Thrift.MessageType.CALL, this.seqid);
  var args = new Shell_shell_args();
  args.query = query;
  args.write(this.output);
  this.output.writeMessageEnd();
  return this.output.getTransport().flush();
};

ShellClient.prototype.recv_shell = function() {
  var ret = this.input.readMessageBegin();
  var fname = ret.fname;
  var mtype = ret.mtype;
  var rseqid = ret.rseqid;
  if (mtype == Thrift.MessageType.EXCEPTION) {
    var x = new Thrift.TApplicationException();
    x.read(this.input);
    this.input.readMessageEnd();
    throw x;
  }
  var result = new Shell_shell_result();
  result.read(this.input);
  this.input.readMessageEnd();

  return;
};