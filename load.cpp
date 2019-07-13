#include <map>
#include "io.h"
#include "load.h"
#include "errmsg.h"

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

class Sentence{
public:
	String command;
	std::vector<String> arguments;
	void run();
};

std::map<const String, const String> macros;

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
