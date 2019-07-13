#include <map>
#include "io.h"
#include "load.h"
#include "errmsg.h"

class Sentence{
public:
	String command;
	std::vector<String> arguments;
	void run();
};

std::map<const String, const String> macros;

String::Pos String::pos() const {
	return Pos(text.front().pos, text.back().pos);
}
void String::add(const Char &ch){
	text.push_back(ch);
}
void String::clear(){
	text.clear();
}
bool String::empty() const{
	return text.empty();
}
bool String::match(const std::string &str) const{
	if(text.size() != str.size()) return false;
	for(int i = 0; i < text.size(); ++i) if(text[i] != str[i]) return false;
	return true;
}
void String::append(const String &str, const String &from){
	for(auto i : str.text){
		text.push_back(i);
		text.back().pos.add_from(from.text.front().pos, from.text.back().pos);
	}
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


void load(Source &source){
	Sentence sentence;

	for(;;){
		while(isspace(source.get(1))) source.get();

		Char tmp = source.get();

		if(source.eof()){
			sentence.run();
			return;
		}else if(tmp == '{'){
			String argument;
			for(;;){
				Char tmp = source.get();
				if(source.eof()){
					error_unexpected_end_of_file(tmp);
				}else if(tmp == '}'){
					break;
				}else if(tmp == '{'){
					String buf;
					for(;;){
						Char tmp = source.get();
						if(tmp == '}') break;
						else buf.add(tmp);
					}
					auto it = macros.find(buf);
					if(it == macros.end()){
						error_undefined_macro(buf);
					}else{
						argument.append(it->second, buf);
					}
				}else{
					argument.add(tmp);
				}
			}
			sentence.arguments.push_back(argument);
		}else if(isalpha(tmp)){
			if(!sentence.command.empty()){
				sentence.run();
				sentence.command.clear();
				sentence.arguments.clear();
			}
			sentence.command.add(tmp);
			while(isalpha(source.get(1))) sentence.command.add(source.get());
		}else{
			error_unexpected_character(tmp);
		}
	}
}

void Sentence::run(){
	if(command.match(std::string("message"))){
		if(arguments.size() == 1){
			std::cout << arguments[0];
		}
	}else if(command.match(std::string("define"))){
		if(arguments.size() == 2){
			auto it = macros.find(arguments[0]);
			if(it == macros.end()){
				macros.insert(std::make_pair(arguments[0], arguments[1]));
			}else{
				error_duplicate_macro(it->first, arguments[0]);
			}
		}
	}else if(command.match(std::string("score"))){
		std::cout << "score" << std::endl;
	}else{
		error_unknown_command(command);
	}
}
