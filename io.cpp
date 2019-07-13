#include "errmsg.h"
#include "io.h"

void Char::Pos::add_from(const Char::Pos &begin, const Char::Pos &end){
	from.push_back(Char::Pos::Rng(begin, end));
}
void Char::Pos::set(int line, int pos){
	this->line = line;
	this->pos = pos;
}
const Char::Pos &Char::Pos::operator=(const Char::Pos &p){
	line = p.line;
	pos = p.pos;
	return *this;
}
std::ostream &operator<<(std::ostream &os, const Char::Pos &pos){
	os << "[" << pos.filename << ":" << pos.line << "," << pos.pos;
	for(auto i : pos.from) os << "<" << i;
	return os << "]";
}
void Char::set(char val, int line, int pos){
	this->val = val;
	this->pos.set(line, pos);
}
const Char &Char::operator=(const Char &ch){
	val = ch.val;
	pos = ch.pos;
	return *this;
}
