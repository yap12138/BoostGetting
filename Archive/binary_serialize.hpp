
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

		/** д���ļ� */
		void TEST_write()
		{
			//д���ļ�  
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

		/** ��ȡ�ļ� */
		void TEST_read()
		{
			//��ȡ�ļ�  
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

		/** �Ƚ�text����binary���������ݵ����ܲ��� */
		void TEST_compare_text_with_binary()
		{
			message_t test{ 77, "it's testing file." };

			std::stringstream binary_sstream;
			std::stringstream text_sstream;

			long long begin, end;
			int size;

			//ʹ�ö����Ƶķ�ʽ���л�
			boost::archive::text_oarchive text_oa(text_sstream);
			boost::archive::binary_oarchive binary_oa(binary_sstream);

			begin = time(NULL);
			for (int i = 0; i < 1000000; ++i)
			{
				text_oa << test;
			}
			end = time(NULL);	//����ʱ�������������ִ��ʱ��

			size = text_sstream.tellp() / (1024 * 1024);

			std::cout << "text serialization seconds: " << end - begin << ", space: " << size << std::endl;

			begin = time(NULL);
			for (int i = 0; i < 1000000; ++i)
			{
				binary_oa << test;
			}
			end = time(NULL);
  
			//��MBΪ��λ
			size = binary_sstream.tellp() / (1024 * 1024);
  
			std::cout << "binary serialization seconds: " << end - begin << ", space: " << size << std::endl;
		}

		/** ֱ��ʹ��binary_oarchive��<<������д������������(eg: int)/std::string���� */
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

		/** ֱ��ʹ��binary_iarchive��>>����������������������(eg: int)/std::string���� */
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
