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
	static std::string T_to_string( T const &val );

	template <typename T>
	static T string_to_T( std::string const &val );

	template <>
	static std::string string_to_T( std::string const &val );
};




class ConfigFile
{
private:
	std::map<std::string, std::string> contents;
	std::string fName;

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
	ValueType getValueOfKey( const std::string &key, ValueType const &defaultValue) const;
};