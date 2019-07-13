#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Char{
	char val;
public:
	class Pos{
		class Rng{
			const std::string &filename;
			class LP{
				int line, pos;
			public:
				friend std::ostream &operator<<(std::ostream &os, const LP &lp){
					return os << lp.line << "," << lp.pos;
				}
				LP(int line, int pos): line(line), pos(pos) {}
			} begin, end;
		public:
			friend std::ostream &operator<<(std::ostream &os, const Rng &rng){
				return os << rng.filename << ":" << rng.begin << "-" << rng.end;
			}
			Rng(const Pos &begin, const Pos &end): filename(begin.filename), begin(begin.line, begin.pos), end(end.line, end.pos){}
		};
		const std::string &filename;
		int line, pos;
		std::vector<Rng> from;
	public:
		void add_from(const Pos &, const Pos &);
		Pos(const std::string &filename): filename(filename){}
		void set(int, int);
		const Pos &operator=(const Pos &);
		friend std::ostream &operator<<(std::ostream &, const Pos &);
	} pos;
	Char(const std::string &filename): pos(filename){}
	void set(char, int, int);
	const Char &operator=(const Char &);
	operator char() const { return val; }
};

class Source{
	std::string filename;
	std::ifstream fs;
	int line, pos;
	struct Buffer{
		Char buf;
		bool isbuf;
		Buffer(const std::string &filename): buf(filename), isbuf(false) {}
	} buf_comment, buf_peek;
	void next(Char &ch);
public:
	bool eof();
	Source(const char []);
	Char get(bool = 0);
};
