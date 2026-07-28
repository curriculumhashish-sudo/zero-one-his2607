#pragma once
/*
 Copyright Zero One Star. All rights reserved.

 @Author: awei
 @Date: 2025/11/28 15:19:56

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#ifndef _PDFREPORTBO_H_
#define _PDFREPORTBO_H_
#include <list>
#include "oatpp/core/Types.hpp"

/**
 * Generic list data accessor interface
 */
class IDataAccessor
{
public:
	virtual ~IDataAccessor() = default;
	// Get the specified field value of the current element
	virtual std::string getField(const std::string& fieldName) const = 0;
	// Move to the next element
	virtual void moveNext() = 0;
	// Whether there is a next element
	virtual bool hasNext() = 0;
};

/**
 * Adapter for oatpp List data
 */
template<typename T>
class OatppAccessor : public IDataAccessor
{
private:
	oatpp::List<T> wrapper_;
	typename std::list<T>::iterator iterator_;
public:
	explicit OatppAccessor(const oatpp::List<T>& wrapper) : wrapper_(wrapper)
	{
		iterator_ = wrapper_->begin();
	}

	std::string getField(const std::string& fieldName) const override
	{
		auto f = (*iterator_)[fieldName.c_str()];
		auto type = f.getValueType();
		if (type == oatpp::String::Class::getType())
			return f.template cast<oatpp::String>();
		else if (type == oatpp::Int32::Class::getType())
			return std::to_string(f.template cast<oatpp::Int32>());
		else if (type == oatpp::UInt32::Class::getType())
			return std::to_string(f.template cast<oatpp::UInt32>());
		else if (type == oatpp::Int64::Class::getType())
			return std::to_string(f.template cast<oatpp::Int64>());
		else if (type == oatpp::UInt64::Class::getType())
			return std::to_string(f.template cast<oatpp::UInt64>());
		else if (type == oatpp::Float32::Class::getType())
			return std::to_string(f.template cast<oatpp::Float32>());
		else if (type == oatpp::Float64::Class::getType())
			return std::to_string(f.template cast<oatpp::Float64>());
		else if (type == oatpp::Boolean::Class::getType())
			return std::to_string(f.template cast<oatpp::Boolean>());
		return "";
	}

	void moveNext() override
	{
		iterator_++;
	}

	bool hasNext() override
	{
		return iterator_ != wrapper_->end();
	}
};

/**
 * Table column field configuration
 */
class FieldConfig
{
public:
	// Title, e.g. "id"
	std::string title;
	// Property name, e.g. "id"
	std::string prop;
	// Column width; 0 means use default
	float colwidth = 0;
};

#endif // !_PDFREPORTBO_H_
