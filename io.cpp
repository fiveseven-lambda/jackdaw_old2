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
bool Source::eof(){
	return fs.eof();
}
Source::Source(const char filename_cstr[]):
	fs(filename_cstr),
	filename(filename_cstr),
	line(0),
	pos(0),
	buf_comment(filename),
	buf_peek(filename)
{
}

void Source::next(Char &ch){
	ch.set(fs.get(), line, pos);
	if(ch == '\n'){
		++line;
		pos = 0;
	}else{
		++pos;
	}
}

Char Source::get(bool peek){
	if(!buf_peek.isbuf){
		if(buf_comment.isbuf){
			buf_peek.buf = buf_comment.buf;
			buf_comment.isbuf = false;
		}else{
			next(buf_peek.buf);
		}
		if(buf_peek.buf == '/'){
			next(buf_comment.buf);
			if(buf_comment.buf == '+'){
				bool plus = false;
				for(;;){
					Char tmp = get();
					if(eof()) error_unterminated_comment(tmp);
					else if(tmp == '+') plus = true;
					else if(plus && tmp == '/') break;
					else plus = false;
				}
				return get(peek);
			}else{
				buf_comment.isbuf = true;
			}
		}
	}
	buf_peek.isbuf = peek;
	return buf_peek.buf;
}
