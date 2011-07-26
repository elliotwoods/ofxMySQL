/*
 *  ofxMySQL.cpp
 *	iOS oF extension
 *
 *  Created by Elliot Woods on 28/11/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

// instructions to build libmysql.a at
// http://blog.iosplace.com/?p=20

#include "ofxMySQL.h"

ofxMySQL::ofxMySQL()
{
	isConnected = false;
	
	nRows = 0;
	nFields = 0;
	
	_db = new MYSQL();
	mysql_init(_db);
	
	setTimeout(5);
}

ofxMySQL::~ofxMySQL()
{
	close();
	delete _db;
}

void ofxMySQL::connect(string hostname, string username, string password, string dbname="")
{
	if (isConnected)
		mysql_close(_db);
	
	isConnected = mysql_real_connect(_db,hostname.c_str(),username.c_str(),password.c_str(),dbname.c_str(),0,NULL,0);
	
	if (!isConnected)
	{
		ofLog(OF_LOG_ERROR, "ofxMySQL: Connection failed to database '" + dbname + "' on host " + hostname);
		reportError();
	} else
		ofLog(OF_LOG_VERBOSE, "ofxMySQL: Successfully connected to database '" + dbname + "' on host " + hostname);
}

void ofxMySQL::close()
{
    mysql_close(_db);
}

bool ofxMySQL::query(string querystring)
{
	if (!isConnected)
	{
		ofLog(OF_LOG_WARNING, "ofxMySQL: Attempting to query without connection");
		return false;
	}
	
	bool success = !mysql_query(_db, querystring.c_str());
	
	if (success)
	{
		_result = mysql_store_result(_db); 
		if (_result != 0)
		{
			nRows = mysql_num_rows(_result);
			nFields = mysql_num_fields(_result);
		}
	} else {
		ofLog(OF_LOG_VERBOSE, "ofxMySQL: Failed query: \"" + querystring + "\"");
		reportError();
	}

	return success;
}

bool ofxMySQL::getStrings(vector<string> &results, string tableName, string fieldName, string whereCondition)
{
	//get a field
	
	string querystring = "SELECT " + fieldName + " FROM " + tableName + (whereCondition.length()>0 ? "WHERE " + whereCondition : "");
	
	//if query failed, quit
	//query function will report errors
	if (!query(querystring))
		return false;
	
	//clear vector
	results.resize(0);
	
	
	//fill in vector
	MYSQL_ROW row;
	while (row = mysql_fetch_row(_result))
		results.push_back(row[0]);
	
	return true;
	
}

bool ofxMySQL::getStrings(vector<vector<string> > &results, string tableName, vector<string> fieldNames, string whereCondition)
{
	//get a vector of fields
	
	string querystring = "SELECT ";
	
	nFields = fieldNames.size();
	
	for (int iField=0; iField < nFields; iField++)
	{
		if (iField>0)
			querystring += ", ";
		
		querystring += fieldNames[iField];
	}
	
	querystring += " FROM " + tableName + (whereCondition.length()>0 ? "WHERE " + whereCondition : "");
	
	//if query failed, quit
	//query function will report errors
	if (!query(querystring))
		return false;
	
	//setup vectors
	results.resize(0);
	vector<string> resultsRow;
	resultsRow.resize(nFields);
	
	
	//fill in vector
	MYSQL_ROW row;
	while (row = mysql_fetch_row(_result))
	{
		
		//populate values
		for (int iField = 0; iField<nFields; iField++)
			resultsRow[iField] = row[iField];
		
		//insert row into results
		results.push_back(resultsRow);
	}
	
	return true;
	
}



int ofxMySQL::insert(string tableName, vector<ofxMySQLField> &fields)
{
	string querystring = "INSERT INTO " + tableName;

	querystring += buildValueString(fields);
	
	if (!query(querystring))
		return -1;
	
	return mysql_insert_id(_db);
	
}

bool ofxMySQL::update(string tableName, vector<ofxMySQLField> &fields, string whereCondition)
{
	//UPDATE table_name SET field1=new-value1, field2=new-value2
	//[WHERE Clause]
	
	string querystring = "UPDATE " + tableName + " SET ";
	
	for (int iField=0; iField<fields.size(); iField++) {
		
		if (iField != 0)
			querystring += ", ";
		
		querystring += fields[iField].fieldName + "=";
		querystring += "'" + fields[iField].value + "'";
	}
	
	querystring += " WHERE " + whereCondition;
	
	return (query(querystring));
}

bool ofxMySQL::deleteRow(string tableName, string whereCondition)
{
	string querystring = "DELETE FROM " + tableName + " WHERE " + whereCondition;
	return query(querystring);
}

void ofxMySQL::setTimeout(unsigned int timeout) {
	setOption(MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
}

void ofxMySQL::setOption(mysql_option setting, const void* value) {
	mysql_options(_db, setting, value);
}

//////////////////////////////////////////////////////////////////////
//privates

void ofxMySQL::reportError()
{
	const char *errorChar = mysql_error(_db);
	
	ofLog(OF_LOG_ERROR, "ofxMySQL: " + string(errorChar));
}

string ofxMySQL::buildValueString(vector<ofxMySQLField> &fields)
{
	string querystring = "(";
	
	//field names, e.g. (ID, name)
	for (int iField=0; iField < fields.size(); iField++)
	{
		if (iField != 0)
			querystring += ", ";
		
		querystring += fields[iField].fieldName;
	}
	querystring += ") VALUES (";
	
	//data values, e.g. "3", "Car"
	for (int iField=0; iField < fields.size(); iField++)
	{
		if (iField != 0)
			querystring += ", ";
		
		querystring += fields[iField].value;
	}
	querystring += ")";
	
	return querystring;
}