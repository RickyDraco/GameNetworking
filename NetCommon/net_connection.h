#pragma once

#include"net_common.h"
#include"net_tsqueue.h"
#include"net_message.h"

namespace net
{
	template<typename T>
	class connection : public std::enable_shared_from_this<connection<T>>				// equvelent to "this" but provides a shared pointer to the current instance
	{
	public:
		connection()
		{}
		virtual ~connection()
		{}

	public:
		bool ConnectToServer();
		bool Disconnect();
		bool IsConnected() const;

	public:
		bool Send(const message<T>& msg);

	protected:

		asio::ip::tcp::socket m_socket;
		asio::io_context& m_asioContext;
		tsqueue<message<T>> m_qMessagesOut;
		tsqueue<owned_message>& m_qMessagesIn;
	};
}