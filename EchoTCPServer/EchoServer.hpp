#ifndef HOGASON_ECHOTCPSERVER_ECHOSERVER_HPP_
#define HOGASON_ECHOTCPSERVER_ECHOSERVER_HPP_

// һ���򵥵Ļ��Է�����

#include <iostream>
#include <memory>
#include <array>
#include <boost/asio.hpp>

namespace hogason
{
	using boost::asio::ip::tcp;

	// ��������ĳ���ͻ���֮��ġ��Ự��
	// �������д�¼�
	class session : public std::enable_shared_from_this<session>
	{
	public:
		session(tcp::socket&& s) : socket_(std::move(s)) {}
		~session() { std::cout << "deconstruct session\n"; }

		void start()
		{
			async_read();
		}
	private:
		void async_read()
		{
			auto self(shared_from_this());
			socket_.async_read_some(boost::asio::buffer(data_),  // �첽��
				[this, self](const boost::system::error_code &ec, size_t bytes_transferred)  // ���������ʱ�ص��ú���
			{  // ����`self`ʹshared_ptr<session>�����ü�������1���ڸ����б�����async_read()�˳�ʱ�����ü�����Ϊ0
				if (!ec)
					async_write(bytes_transferred);  // ���꼴д
			}
			);
		}

		void async_write(std::size_t length)
		{
			auto self(shared_from_this());
			boost::asio::async_write(socket_, boost::asio::buffer(data_, length),  // �첽д
				[this, self](const boost::system::error_code &ec, size_t)
			{
				if (!ec)
					async_read();
			}
			);
		}

		tcp::socket socket_;  // ���Ự�������Ѿ�������socket����
		std::array<char, 1024> data_;
	};

	// ��������
	// �����ͻ�����������async_accept������ĳ���ͻ��˽���socket���Ӻ�Ϊ������һ��session
	class server
	{
	public:
		server(boost::asio::io_service &io_service, short port)
			: acceptor_(io_service, tcp::endpoint(tcp::v4(), port)), socket_(io_service)
		{
			async_accept();
		}

	private:
		void async_accept()
		{
			acceptor_.async_accept(socket_, std::bind(&server::handle_accept, this, std::placeholders::_1));  // �첽accept��socket���ӽ����󣬵���handle_accept()
		}

		void handle_accept(const boost::system::error_code &ec)
		{
			if (!ec)
			{
				std::shared_ptr<session> session_ptr(new session(std::move(socket_)));
				session_ptr->start();
			}

			async_accept();  // ���������ͻ�����������
		}

		tcp::acceptor acceptor_;
		tcp::socket socket_;
	};

	inline void TEST_EchoServer()
	{
		boost::asio::io_service io_service;
		server s(io_service, 52014);
		io_service.run();
	}
}

#endif // HOGASON_ECHOTCPSERVER_ECHOSERVER_HPP_
