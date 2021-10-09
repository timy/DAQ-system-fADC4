#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class CFG_TYPE_ABSTRACT {
public:
	CFG_TYPE_ABSTRACT() {}
	CFG_TYPE_ABSTRACT(std::string key) : k(key) {}
	virtual ~CFG_TYPE_ABSTRACT() {}

	virtual void write(std::ostringstream& ss) = 0;
	virtual void read(std::istringstream& ss) = 0;

protected:
	std::string k;
};

template <typename T>
class CFG_TYPE_VAR : public CFG_TYPE_ABSTRACT {
public:
	CFG_TYPE_VAR(std::string key, T& value) : CFG_TYPE_ABSTRACT(key), v(value) {}

	virtual void write(std::ostringstream& ss) final {
		ss << k << " = " << v;
	}
	virtual void read(std::istringstream& ss) final {
		std::string strKey, strEqu;
		ss >> strKey >> strEqu >> v;
	}
protected:
	T& v;
};

class CFG_TYPE_COMMENT : public CFG_TYPE_ABSTRACT {
public:
	CFG_TYPE_COMMENT(std::string comment) : CFG_TYPE_ABSTRACT(comment) {}

	virtual void write(std::ostringstream& ss) final { ss << k; }
	virtual void read(std::istringstream& ss) final {}
};

class CFG_TYPE_SKIP : public CFG_TYPE_ABSTRACT {
public:
	virtual void write(std::ostringstream& ss) final {}
	virtual void read(std::istringstream& ss) final {}
};


class CfgFileItems {
public:
	CfgFileItems() {}
	virtual ~CfgFileItems() {
		while (!lines.empty()) {
			delete lines.back();
			lines.pop_back();
		}
	}

	// add item
	template <typename T>
	void add(std::string key, T& value) {
		lines.push_back(new CFG_TYPE_VAR<T>(key, value));
	}
	void add(std::string key) {
		lines.push_back(new CFG_TYPE_COMMENT(key));
	}
	void add() {
		lines.push_back(new CFG_TYPE_SKIP());
	}

	// read and write
	void write(std::ofstream& file) {
		std::ostringstream ss;
		for (auto line : lines) {
			ss.str(std::string());
			ss.clear();
			line->write(ss);
			file << ss.str() << std::endl;
		}
	}
	void read(std::ifstream& file) {
		std::string strLine;
		std::istringstream ss;
		for (auto line : lines) {
			strLine.clear();
			std::getline(file, strLine);
			ss.str(strLine);
			line->read(ss);
			ss.clear();
		}
	}

private:
	std::vector<CFG_TYPE_ABSTRACT*> lines;
};
