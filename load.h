#include <vector>
#include <iostream>

void load(Source &);

class String{
	std::vector<Char> text;
	struct Pos{
		Char::Pos begin, end;
		Pos(Char::Pos begin, Char::Pos end): begin(begin), end(end) {}
		friend std::ostream &operator<<(std::ostream &os, const Pos &pos){
			return os << pos.begin << "-" << pos.end;
		}
	};
public:
	Pos pos() const;
	void add(const Char &);
	void clear();
	bool empty() const;
	bool match(const std::string &str) const;
	void append(const String &, const String &);
	friend std::ostream &operator<<(std::ostream &os, const String &str){
		for(Char i : str.text) os << i;
		return os;
	}
	friend bool operator<(const String &l, const String &r){
		std::vector<Char>::const_iterator il = l.text.begin(), ir = r.text.begin();
		for(;; ++il, ++ir){
			if(ir == r.text.end()) return false;
			else if(il == l.text.end()) return true;
			else if(*il > *ir) return false;
			else if(*il < *ir) return true;
		}
	}
};
