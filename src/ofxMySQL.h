#pragma once

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
	class Row : public map<string, string> {
	public:
		template<typename FieldType>
		bool get(string fieldName, FieldType & value) const {
			auto findFieldName = this->find(fieldName);
			if (findFieldName != this->end()) {
				auto findFieldValue = findFieldName->second;
				if (typeid(FieldType) == typeid(int)) {
					value = ofToInt(findFieldValue);
					return true;
				} else if (typeid(FieldType) == typeid(float)) {
					value = ofToFloat(findFieldValue);
					return true;
				} else if (typeid(FieldType) == typeid(string)) {
					value = * (FieldType *) & findFieldValue;
					return true;
				}
			}
			return false;
		}
		
		template<typename FieldType>
		void set(string fieldName, FieldType & value) {
			stringstream ss;
			ss << value;
			(*this)[fieldName] = ss.str();
		}
	};
	typedef vector<Row> MultipleRow;
	
	ofxMySQL();
	~ofxMySQL();
	
	void	connect(string hostname, string username, string password, string dbname);
    void    close();
    
	bool	query(string querystring);
	
	///returns a vector of rows, for each row we store the values in a map of <filedname, value>
	MultipleRow select(string tableName, string fields = "*", string options="");
	
	//old select syntax
	bool	getStrings(vector<string> &results, string tableName, string fieldName, string whereCondition="");
	bool	getStrings(vector<vector<string> > &results, string tableName, vector<string> fieldNames, string whereCondition="");
	
	int		insert(string tableName, vector<ofxMySQLField> &fields);
	int		insert(string tableName, const Row &);
	bool	update(string tableName, vector<ofxMySQLField> &fields, string whereCondition);
	bool	update(string tableName, const Row &, string whereCondition);
	bool	deleteRow(string tableName, string whereCondition);

	bool	isConnected() const;
	
	int		nRows;
	int		nFields;
	
	//settings
	void	setTimeout(unsigned int timeout);
	void	setOption(mysql_option setting, const void* value);
	
	MYSQL	* getDatabase();
protected:
	void	reportError();
	string	buildValueString(vector<ofxMySQLField> &fields);
	MYSQL		*_db;
	MYSQL_RES	*_result;
	bool	connected;
};

