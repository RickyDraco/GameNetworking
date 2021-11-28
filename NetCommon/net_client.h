#pragma once
#include"net_common.h"
#include"net_message.h"
#include"net_tsqueue.h"
#include"net_connection.h"


namespace net
{
	template<typename T>
	class client_interface
	{
	public:
		client_interface()
			: m_socket(m_context)
		{

		}

		virtual ~client_interface() { Disconnect(); }

	private:
		tsqueue<owned_message<T>> m_qMessagesIn;

	protected:
		asio::io_context m_context;
		std::thread thrContext;
		asio::ip::tcp::socket m_socket;
		std::unique_ptr<connection<T>> m_connection;

	public:
		bool Connect(const std::string& host, const uint16_t port)
		{
			try
			{
				m_connection = std::make_unique<connection<T>>();

				asio::ip::tcp::resolver resolver(m_context);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Client Exception: " << e.what() << std::endl;
				return false;
			}

			return true;
		}
		
		void Disconnect()
		{

		}

		bool IsConnected()
		{
			if (m_connection)
				return m_connection->IsConnected();
			else
				return false;
		}

		tsqueue<owned_message<T>>& Incoming()
		{
			return m_qMessagesIn;
		}
	};
}