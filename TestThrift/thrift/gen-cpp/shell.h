/**
 * Autogenerated by Thrift Compiler (0.9.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef shell_H
#define shell_H

#include <thrift/TDispatchProcessor.h>
#include "test_types.h"

namespace open {

	class shellIf {
	public:
		virtual ~shellIf() {}
		virtual void shell(const std::string& query) = 0;
	};

	class shellIfFactory {
	public:
		typedef shellIf Handler;

		virtual ~shellIfFactory() {}

		virtual shellIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
		virtual void releaseHandler(shellIf* /* handler */) = 0;
	};

	class shellIfSingletonFactory : virtual public shellIfFactory {
	public:
		shellIfSingletonFactory(const std::shared_ptr<shellIf>& iface) : iface_(iface) {}
		virtual ~shellIfSingletonFactory() {}

		virtual shellIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
			return iface_.get();
		}
		virtual void releaseHandler(shellIf* /* handler */) {}

	protected:
		std::shared_ptr<shellIf> iface_;
	};

	class shellNull : virtual public shellIf {
	public:
		virtual ~shellNull() {}
		void shell(const std::string& /* query */) {
			return;
		}
	};

	typedef struct _shell_shell_args__isset {
		_shell_shell_args__isset() : query(false) {}
		bool query;
	} _shell_shell_args__isset;

	class shell_shell_args {
	public:

		shell_shell_args() : query() {
		}

		virtual ~shell_shell_args() throw() {}

		std::string query;

		_shell_shell_args__isset __isset;

		void __set_query(const std::string& val) {
			query = val;
		}

		bool operator == (const shell_shell_args & rhs) const
		{
			if (!(query == rhs.query))
				return false;
			return true;
		}
		bool operator != (const shell_shell_args &rhs) const {
			return !(*this == rhs);
		}

		bool operator < (const shell_shell_args &) const;

		uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
		uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

	};


	class shell_shell_pargs {
	public:


		virtual ~shell_shell_pargs() throw() {}

		const std::string* query;

		uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

	};


	class shell_shell_result {
	public:

		shell_shell_result() {
		}

		virtual ~shell_shell_result() throw() {}


		bool operator == (const shell_shell_result & /* rhs */) const
		{
			return true;
		}
		bool operator != (const shell_shell_result &rhs) const {
			return !(*this == rhs);
		}

		bool operator < (const shell_shell_result &) const;

		uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
		uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

	};


	class shell_shell_presult {
	public:


		virtual ~shell_shell_presult() throw() {}


		uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

	};

	class shellClient : virtual public shellIf {
	public:
		shellClient(std::tr1::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) :
			piprot_(prot),
			poprot_(prot) {
			iprot_ = prot.get();
			oprot_ = prot.get();
		}
		shellClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) :
			piprot_(iprot),
			poprot_(oprot) {
			iprot_ = iprot.get();
			oprot_ = oprot.get();
		}
		std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
			return piprot_;
		}
		std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
			return poprot_;
		}
		void shell(const std::string& query);
		void send_shell(const std::string& query);
		void recv_shell();
	protected:
		std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
		std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
		::apache::thrift::protocol::TProtocol* iprot_;
		::apache::thrift::protocol::TProtocol* oprot_;
	};

	class shellProcessor : public ::apache::thrift::TDispatchProcessor {
	protected:
		std::shared_ptr<shellIf> iface_;
		virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
	private:
		typedef  void (shellProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
		typedef std::map<std::string, ProcessFunction> ProcessMap;
		ProcessMap processMap_;
		void process_shell(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
	public:
		shellProcessor(std::shared_ptr<shellIf> iface) :
			iface_(iface) {
			processMap_["shell"] = &shellProcessor::process_shell;
		}

		virtual ~shellProcessor() {}
	};

	class shellProcessorFactory : public ::apache::thrift::TProcessorFactory {
	public:
		shellProcessorFactory(const ::std::shared_ptr< shellIfFactory >& handlerFactory) :
			handlerFactory_(handlerFactory) {}

		::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

	protected:
		::std::shared_ptr< shellIfFactory > handlerFactory_;
	};

	class shellMultiface : virtual public shellIf {
	public:
		shellMultiface(std::vector<std::shared_ptr<shellIf> >& ifaces) : ifaces_(ifaces) {
		}
		virtual ~shellMultiface() {}
	protected:
		std::vector<std::shared_ptr<shellIf> > ifaces_;
		shellMultiface() {}
		void add(std::shared_ptr<shellIf> iface) {
			ifaces_.push_back(iface);
		}
	public:
		void shell(const std::string& query) {
			size_t sz = ifaces_.size();
			size_t i = 0;
			for (; i < (sz - 1); ++i) {
				ifaces_[i]->shell(query);
			}
			ifaces_[i]->shell(query);
		}

	};

} // namespace

#endif
