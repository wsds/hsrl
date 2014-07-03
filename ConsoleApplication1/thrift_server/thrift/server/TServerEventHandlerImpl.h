/*
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements. See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership. The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License. You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied. See the License for the
* specific language governing permissions and limitations
* under the License.
*/

#ifndef _TSERVEREVENTHANDLERIMPL_H_
#define _TSERVEREVENTHANDLERIMPL_H_ 1

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <thrift/server/TServer.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TWebSocketTransport.h>

namespace apache { namespace thrift { namespace server {

	/**
	* Virtual interface class that can handle events from the server core. To
	* use this you should subclass it and implement the methods that you care
	* about. Your subclass can also store local data that you may care about,
	* such as additional "arguments" to these methods (stored in the object
	* instance's state).
	*/
	template<class ServiceClient>
	class TServerEventHandlerImpl : public TServerEventHandler {
	public:
		TServerEventHandlerImpl() 
			: stop_(false)
			, lock_send_(send_mutex_, boost::defer_lock_t())
		{ }
		virtual ~TServerEventHandlerImpl() {}

		/**
		* Called before the server begins.
		*/
		virtual void preServe() {}

		/**
		* Called when a new client has connected and is about to being processing.
		*/
		virtual void* createContext(boost::shared_ptr<TProtocol> input,
			boost::shared_ptr<TProtocol> output)
		{	
			input->getInputTransport()->handshake();

			TWebsocketTransport* in = dynamic_cast<TWebsocketTransport*>(input->getInputTransport().get());
			TWebsocketTransport* out = dynamic_cast<TWebsocketTransport*>(output->getInputTransport().get());
			if (in != NULL && out != NULL) {
				out->setOldVersion(in->isOldVersion());
			}

			boost::mutex::scoped_lock lock(client_mutex_);

			/// for concurrency, only use the original out socket.
			boost::shared_ptr<TProtocol> jsonOutput(new TJSONProtocol(output->getTransport()));
			client_ = boost::shared_ptr<ServiceClient>(new ServiceClient(jsonOutput));

			stop_ = false;
			printf("createContext\n");
			return NULL;
		}

		/**
		* Called when a client has finished request-handling to delete server
		* context.
		*/
		virtual void deleteContext(void* serverContext,
			boost::shared_ptr<TProtocol>input,
			boost::shared_ptr<TProtocol>output) {
				(void)serverContext;
				(void)input;
				(void)output;

				boost::mutex::scoped_lock lock(client_mutex_);

//				output->getTransport()->flush();

				printf("deleteContext\n");
				//stop_ = true;
				client_.reset();
		}

		/**
		* Called when a client is about to call the processor.
		*/
		virtual void processContext(void* serverContext,
			boost::shared_ptr<TTransport> transport) {
				(void)serverContext;
				(void)transport;
		}

		bool isStop() const { return stop_; }
		void stopIt() { stop_ = true; }
		/** 
		*
		* for example:
		UserProfile up, up2;
		up.uid = 2;
		up.name = "gnet";
		client.store(up);
		*/
		boost::shared_ptr<typename ServiceClient> getServiceClient() const { return client_; }

		boost::mutex& getClientMutex() { return client_mutex_; }
		void lockSend() { lock_send_.lock(); }
		void unlockSend() { lock_send_.unlock(); }

	protected:	    
		/// only for single thread model. only support one client.
		boost::shared_ptr<ServiceClient>	client_;
		bool								stop_;		/// only for test

		/// this mutex used for send notice(which use the output protocol and out transport)	 
		boost::mutex				send_mutex_;
		boost::mutex::scoped_lock	lock_send_;

		boost::mutex				client_mutex_;
	};



	/**
	* Virtual interface class that can handle events from the processor. To
	* use this you should subclass it and implement the methods that you care
	* about. Your subclass can also store local data that you may care about,
	* such as additional "arguments" to these methods (stored in the object
	* instance's state).
	*/
	template<class ServiceClient>
	class TProcessorEventHandlerImpl: public TProcessorEventHandler {
	public:
		TProcessorEventHandlerImpl(boost::shared_ptr<TServerEventHandler>	serverEventHandler)
			: serverEventHandler_(serverEventHandler){}

		/**
		* Called between calling the handler and writing the response.
		*/
		virtual void preWrite(void* ctx, const char* fn_name) {
			(void) ctx;
			(void) fn_name;

			((TServerEventHandlerImpl<ServiceClient>*)serverEventHandler_.get())->lockSend();	
		}

		/**
		* Called after writing the response.
		*/
		virtual void postWrite(void* ctx, const char* fn_name, uint32_t bytes) {
			(void) ctx;
			(void) fn_name;
			(void) bytes;

			((TServerEventHandlerImpl<ServiceClient>*)serverEventHandler_.get())->unlockSend();
		}

	private:
		/// 由于加锁、解锁是分开的，中间不能有异常，否则，可能就不会解锁了。
		boost::shared_ptr<TServerEventHandler>	serverEventHandler_;	
	};

}}} // apache::thrift::server

#endif // #ifndef _TSERVEREVENTHANDLERIMPL_H_
