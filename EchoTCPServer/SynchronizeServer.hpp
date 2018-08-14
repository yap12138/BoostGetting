

#ifndef HOGASON_ECHOTCPSERVER_SYNCHRONIZESERVER_HPP_
#define HOGASON_ECHOTCPSERVER_SYNCHRONIZESERVER_HPP_

#if defined(LINUX)

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>

namespace hogason
{
	using namespace std;
	using boost::asio::ip::tcp;

	const char serviceList[] =
		"        Services          \n"
		"**************************\n"
		"[1] Get current time.     \n"
		"[2] Who's online.         \n"
		"[3] Get system info.      \n"
		"**************************\n"
		"Please pick a service[1-3]: ";

	void getResult(const string& cmdPrefix, const char* outputFile, string& res)
	{
		// cmd == "w > who"
		string cmd(cmdPrefix + outputFile);
		system(cmd.c_str());

		ifstream fin;
		fin.open(outputFile);
		if (fin) {
			ostringstream os;
			os << fin.rdbuf();
			res = os.str();
		}
		if (fin.is_open()) {
			fin.close();
		}
	}

	string getServiceContent(const int& select) {
		string res;
		switch (select) {
		case 1: {
			time_t t = time(0);
			res = ctime(&t);
			break;
		}
		case 2:
			getResult("w > ", "who", res);
			break;
		case 3:
			getResult("uname -a > ", "uname", res);
			break;
		default:
			res = "Sorry, no such service.\n";
			break;
		}
		return res;
	}

	inline void TEST_SynchronizeServer() {
		try {
			boost::asio::io_service io_service; // #1
			tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 8868)); // #2
			while (1)
			{
				tcp::socket socket(io_service);
				acceptor.accept(socket);

				boost::system::error_code ignored_error;
				boost::asio::write(socket, boost::asio::buffer(serviceList), ignored_error);

				char selection[20];
				size_t n = socket.read_some(boost::asio::buffer(selection), ignored_error);

				string response = getServiceContent(atoi(selection));
				boost::asio::write(socket, boost::asio::buffer(response), boost::asio::transfer_all(), ignored_error);
			}
		}
		catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

#endif // LINUX

#endif // HOGASON_ECHOTCPSERVER_SYNCHRONIZESERVER_HPP_
