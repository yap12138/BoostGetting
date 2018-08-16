
#ifndef HOGASON_ARCHIVE_BINARY_SERIALIZE_HPP_
#define HOGASON_ARCHIVE_BINARY_SERIALIZE_HPP_
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/archive/text_oarchive.hpp>

#include <fstream>
#include <iostream>
#include <sstream>

namespace hogason
{
	namespace archive
	{
		struct message_t
		{
			int num;
			std::string str;

			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive &ar, const unsigned int /*version*/)
			{
				ar & num;
				ar & str;
			}
		};

		/** 写入文件 */
		void TEST_write()
		{
			//写入文件  
			std::string fileName = "test.dat";
			std::ofstream os(fileName.c_str(), std::ios::binary);
			if (!os.fail())
			{
				try
				{
					message_t buf_t {77, "it's testing file."};
					boost::archive::binary_oarchive oa(os);
					oa << buf_t;
				}
				catch (std::exception& e)
				{
					std::cerr << "Exception: " << e.what() << "\n";
				}
			}
			if (os.is_open())
				os.close();
		}

		/** 读取文件 */
		void TEST_read()
		{
			//读取文件  
			std::string fileName = "test.dat";
			std::ifstream is(fileName.c_str(), std::ios::binary);
			if (!is.fail())
			{
				try
				{
					message_t buf_t;
					boost::archive::binary_iarchive ia(is);
					ia >> buf_t;
					std::cout << buf_t.num << "\n" << buf_t.str << "\n";
				}
				catch (boost::archive::archive_exception& e)//boost::archive::archive_exception  
				{
					std::cerr << "Exception: " << e.what() << "\n";
				}
			}
			if (is.is_open())
				is.close();
		}

		/** 比较text流和binary流处理数据的性能差异 */
		void TEST_compare_text_with_binary()
		{
			message_t test{ 77, "it's testing file." };

			std::stringstream binary_sstream;
			std::stringstream text_sstream;

			long long begin, end;
			int size;

			//使用二进制的方式序列化
			boost::archive::text_oarchive text_oa(text_sstream);
			boost::archive::binary_oarchive binary_oa(binary_sstream);

			begin = time(NULL);
			for (int i = 0; i < 1000000; ++i)
			{
				text_oa << test;
			}
			end = time(NULL);	//这里时间用来计算程序执行时间

			size = text_sstream.tellp() / (1024 * 1024);

			std::cout << "text serialization seconds: " << end - begin << ", space: " << size << std::endl;

			begin = time(NULL);
			for (int i = 0; i < 1000000; ++i)
			{
				binary_oa << test;
			}
			end = time(NULL);
  
			//以MB为单位
			size = binary_sstream.tellp() / (1024 * 1024);
  
			std::cout << "binary serialization seconds: " << end - begin << ", space: " << size << std::endl;
		}

		/** 直接使用binary_oarchive的<<操作符写内置数据类型(eg: int)/std::string类型 */
		void TEST_single_write()
		{
			std::ofstream _os("test.dat", std::ios::binary);
			if (!_os.fail())
			{
				try
				{
					boost::archive::binary_oarchive oa(_os);
					oa << 88;
					oa << "my single serialize test";
				}
				catch (...)
				{
				}
			}
			_os.close();
		}

		/** 直接使用binary_iarchive的>>操作符读入内置数据类型(eg: int)/std::string类型 */
		void TEST_single_read()
		{
			int rInt;
			std::string rMsg;
			std::ifstream _is("test.dat", std::ios::binary);
			if (!_is.fail())
			{
				try
				{
					boost::archive::binary_iarchive ia(_is);
					ia >> rInt;
					ia >> rMsg;
					std::cout << rInt << "\n" << rMsg << "\n";
				}
				catch (...)
				{
				}
			}
			_is.close();
		}
	}
}

#endif // HOGASON_ARCHIVE_BINARY_SERIALIZE_HPP_
