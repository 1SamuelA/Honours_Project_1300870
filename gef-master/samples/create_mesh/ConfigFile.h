#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>

class Convert
{
public:
	template <typename T>
	static std::string T_to_string( T const &val )
	{
		std::ostringstream ostr;
		ostr << val;

		return ostr.str();
	}

	void exitWithError( const std::string &error )
	{
		std::cout << error;
		std::cin.ignore();
		std::cin.get();

		exit( EXIT_FAILURE );
	}

	template <typename T>
	static T string_to_T( std::string const &val )
	{
		std::istringstream istr( val );
		T returnVal;
		if( !(istr >> returnVal) )
			exit(0);
			//exitWithError( "CFG: Not a valid " + (std::string)typeid(T).name() + " received!\n" );

		return returnVal;
	}

	

	template <>
	static std::string string_to_T( std::string const &val )
	{
		return val;
	}
};




class ConfigFile
{
private:
	std::map<std::string, std::string> contents;
	std::string fName;
	void exitWithError( const std::string &error );
	void removeComment( std::string &line ) const;

	bool onlyWhitespace( const std::string &line ) const;
	bool validLine( const std::string &line ) const;

	void extractKey( std::string &key, size_t const &sepPos, const std::string &line ) const;
	void extractValue( std::string &value, size_t const &sepPos, const std::string &line ) const;

	void extractContents( const std::string &line );

	void parseLine( const std::string &line, size_t const lineNo );

	void ExtractKeys();

	
public:
	ConfigFile( const std::string &fName );

	bool keyExists( const std::string &key ) const;

	template <typename ValueType>
	ValueType getValueOfKey( const std::string &key, ValueType const &defaultValue = ValueType() ) const
	{
		if( !keyExists( key ) )
			return defaultValue;

		return Convert::string_to_T<ValueType>( contents.find( key )->second );
	}

	bool OutputFile()
	{
		//std::ofstream file;
		//file.open( fName.c_str() );
		//if( !file )
		//	exit( -1 );
		std::string file;

		for( int x = 0; x < contents.size(); x++ )
		{
			std::string line = (std::string)contents.
			file = "a";
		}
	}
};




//
//#include <iostream>
//#include <string>
//#include <sstream>
//#include <map>
//#include <fstream>
//
//class Convert
//{
//public:
//	template <typename T>
//	static std::string T_to_string( T const &val );
//
//	template <typename T>
//	static T string_to_T( std::string const &val );
//
//	template <>
//	static std::string string_to_T( std::string const &val );
//};
//
//
//
//
//class ConfigFile
//{
//private:
//	std::map<std::string, std::string> contents;
//	std::string fName;
//
//	void removeComment( std::string &line ) const;
//
//	bool onlyWhitespace( const std::string &line ) const;
//	bool validLine( const std::string &line ) const;
//
//	void extractKey( std::string &key, size_t const &sepPos, const std::string &line ) const;
//	void extractValue( std::string &value, size_t const &sepPos, const std::string &line ) const;
//
//	void extractContents( const std::string &line );
//
//	void parseLine( const std::string &line, size_t const lineNo );
//
//	void ExtractKeys();
//public:
//	ConfigFile( const std::string &fName );
//
//	bool keyExists( const std::string &key ) const;
//
//	template <typename ValueType>
//	ValueType getValueOfKey( const std::string &key, ValueType const &defaultValue = ValueType() ) const;
//};