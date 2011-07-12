#pragma once
/*
 *  ofxMySQL.h
 *	iOS oF extension
 *
 *  Created by Elliot Woods on 28/11/2010.
 *  Copyright 2010 Kimchi and Chips. All rights reserved.
 *
 */

// instructions to build libmysql.a at
// http://blog.iosplace.com/?p=20


#include "mysql.h"
#include "ofMain.h"

class ofxMySQLField {
public:
	ofxMySQLField(string _fieldName, string _value)
	{
		fieldName = _fieldName;
		value = _value;
	}
	
	string fieldName;
	string value;
};

class ofxMySQL
{
public:
	ofxMySQL();
	~ofxMySQL();
	
	void	connect(string hostname, string username, string password, string dbname);
    void    close();
    
	bool	query(string querystring);
	
	bool	getStrings(vector<string> &results, string tableName, string fieldName, string whereCondition="");
	bool	getStrings(vector<vector<string> > &results, string tableName, vector<string> fieldNames, string whereCondition="");
	int		insert(string tableName, vector<ofxMySQLField> &fields);
	bool	update(string tableName, vector<ofxMySQLField> &fields, string whereCondition);
	bool	deleteRow(string tableName, string whereCondition);

	bool	isConnected;
	
	int		nRows;
	int		nFields;
	
protected:
	void	reportError();
	string	buildValueString(vector<ofxMySQLField> &fields);
	MYSQL		*_db;
	MYSQL_RES	*_result;
};

