#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <experimental/optional>
#include <cctype>

#undef NULL

namespace ascii {
    constexpr int NULL = 0;
    constexpr int SOH = 1;
    constexpr int STX = 2;
    constexpr int ETX = 3;
    constexpr int EOT = 4;
    constexpr int ENQ = 5;
    constexpr int ACK = 6;
    constexpr int BEL = 7;
    constexpr int BS = 8;
    constexpr int HT = 9;
    constexpr int LF = 10;
    constexpr int VT = 11;
    constexpr int FF = 12;
    constexpr int CR = 13;
    constexpr int SO = 14;
    constexpr int SI = 15;
    constexpr int DLE = 16;
    constexpr int DC1 = 17;
    constexpr int DC2 = 18;
    constexpr int DC3 = 19;
    constexpr int DC4 = 20;
    constexpr int NAK = 21;
    constexpr int SYN = 22;
    constexpr int ETB = 23;
    constexpr int CAN = 24;
    constexpr int EM = 25;
    constexpr int SUB = 26;
    constexpr int ESC = 27;
    constexpr int FS = 28;
    constexpr int GS = 29;
    constexpr int RS = 30;
    constexpr int US = 31;
    constexpr int SPACE = 32;
    constexpr int EXCLAMATION_MARK = 33;
    constexpr int QUOTATION_MARK = 34;
    constexpr int NUMBER_SIGN = 35;
    constexpr int DOLLAR_SIGN = 36;
    constexpr int PERCENT_SIGN = 37;
    constexpr int AMPERSAND = 38;
    constexpr int APOSTROPHE = 39;
    constexpr int ROUND_BRACKET_OPEN = 40;
    constexpr int ROUND_BRACKET_CLOSE = 41;
    constexpr int ASTERISK = 42;
    constexpr int PLUS_SIGN = 43;
    constexpr int COMMA = 44;
    constexpr int HYPHEN = 45;
    constexpr int DOT = 46;
    constexpr int SLASH = 47;
    constexpr int ZERO = 48;
    constexpr int ONE = 49;
    constexpr int TWO = 50;
    constexpr int THREE = 51;
    constexpr int FOUR = 52;
    constexpr int FIVE = 53;
    constexpr int SIX = 54;
    constexpr int SEVEN = 55;
    constexpr int EIGHT = 56;
    constexpr int NINE = 57;
    constexpr int COLON = 58;
    constexpr int SEMICOLON = 59;
    constexpr int LESS_THAN = 60;
    constexpr int EQUALS_SIGN = 61;
    constexpr int GREATER_SIGN = 62;
    constexpr int QUESTION_MARK = 63;
    constexpr int AT_SIGN = 64;
    constexpr int A = 65;
    constexpr int B = 66;
    constexpr int C = 67;
    constexpr int D = 68;
    constexpr int E = 69;
    constexpr int F = 70;
    constexpr int G = 71;
    constexpr int H = 72;
    constexpr int I = 73;
    constexpr int J = 74;
    constexpr int K = 75;
    constexpr int L = 76;
    constexpr int M = 77;
    constexpr int N = 78;
    constexpr int O = 79;
    constexpr int P = 80;
    constexpr int Q = 81;
    constexpr int R = 82;
    constexpr int S = 83;
    constexpr int T = 84;
    constexpr int U = 85;
    constexpr int V = 86;
    constexpr int W = 87;
    constexpr int X = 88;
    constexpr int Y = 89;
    constexpr int Z = 90;
    constexpr int SQUARE_BRACKET_OPEN = 91;
    constexpr int BACKSLASH = 92;
    constexpr int SQUARE_BRACKET_CLOSE = 93;
    constexpr int CARET = 94;
    constexpr int UNDERSCORE = 95;
    constexpr int GRAVE = 96;
    constexpr int a = 97;
    constexpr int b = 98;
    constexpr int c = 99;
    constexpr int d = 100;
    constexpr int e = 101;
    constexpr int f = 102;
    constexpr int g = 103;
    constexpr int h = 104;
    constexpr int i = 105;
    constexpr int j = 106;
    constexpr int k = 107;
    constexpr int l = 108;
    constexpr int m = 109;
    constexpr int n = 110;
    constexpr int o = 111;
    constexpr int p = 112;
    constexpr int q = 113;
    constexpr int r = 114;
    constexpr int s = 115;
    constexpr int t = 116;
    constexpr int u = 117;
    constexpr int v = 118;
    constexpr int w = 119;
    constexpr int x = 120;
    constexpr int y = 121;
    constexpr int z = 122;
    constexpr int CURLY_BRACKET_OPEN = 123;
    constexpr int VERTICAL_SLASH = 124;
    constexpr int CURLY_BRACKET_CLOSE = 125;
    constexpr int TILDE = 126;
    constexpr int DEL = 127;
}

namespace csi_sequence {
    // NOTE(robin): this is a shitty hash :), but works
	constexpr short encode_escape_sequence(const char c) noexcept {
		return c + 1000;
	};

	constexpr char decode_escape_sequence(const short c) noexcept {
		return c - 1000;
	}

	constexpr short unhandled = encode_escape_sequence(0);
	constexpr short arrow_up = encode_escape_sequence('A');
	constexpr short arrow_down = encode_escape_sequence('B');
	constexpr short arrow_right = encode_escape_sequence('C');
	constexpr short arrow_left = encode_escape_sequence('D');
	constexpr int MAX_ESC_SEQ_ARGS = 4;
	
	short sequence_table[128][2] = {
		{0,unhandled}, {1,unhandled}, {2,unhandled}, {3,unhandled},
		{4,unhandled}, {5,unhandled}, {6,unhandled}, {7,unhandled},
		{8,unhandled}, {9,unhandled}, {10,unhandled}, {11,unhandled},
		{12,unhandled}, {13,unhandled}, {14,unhandled}, {15,unhandled},
		{16,unhandled}, {17,unhandled}, {18,unhandled}, {19,unhandled},
		{20,unhandled}, {21,unhandled}, {22,unhandled}, {23,unhandled},
		{24,unhandled}, {25,unhandled}, {26,unhandled}, {27,unhandled},
		{28,unhandled}, {29,unhandled}, {30,unhandled}, {31,unhandled},
		{32,unhandled}, {33,unhandled}, {34,unhandled}, {35,unhandled},
		{36,unhandled}, {37,unhandled}, {38,unhandled}, {39,unhandled},
		{40,unhandled}, {41,unhandled}, {42,unhandled}, {43,unhandled},
		{44,unhandled}, {45,unhandled}, {46,unhandled}, {47,unhandled},
		{48,unhandled}, {49,unhandled}, {50,unhandled}, {51,unhandled},
		{52,unhandled}, {53,unhandled}, {54,unhandled}, {55,unhandled},
		{56,unhandled}, {57,unhandled}, {58,unhandled}, {59,unhandled},
		{60,unhandled}, {61,unhandled}, {62,unhandled}, {63,unhandled},
		{64,unhandled}, {65,arrow_up}, {66,arrow_down}, {67,arrow_right},
		{68,arrow_left}, {69,unhandled}, {70,unhandled}, {71,unhandled},
		{72,unhandled}, {73,unhandled}, {74,unhandled}, {75,unhandled},
		{76,unhandled}, {77,unhandled}, {78,unhandled}, {79,unhandled},
		{80,unhandled}, {81,unhandled}, {82,unhandled}, {83,unhandled},
		{84,unhandled}, {85,unhandled}, {86,unhandled}, {87,unhandled},
		{88,unhandled}, {89,unhandled}, {90,unhandled}, {91,unhandled},
		{92,unhandled}, {93,unhandled}, {94,unhandled}, {95,unhandled},
		{96,unhandled}, {97,unhandled}, {98,unhandled}, {99,unhandled},
		{100,unhandled}, {101,unhandled}, {102,unhandled}, {103,unhandled},
		{104,unhandled}, {105,unhandled}, {106,unhandled}, {107,unhandled},
		{108,unhandled}, {109,unhandled}, {110,unhandled}, {111,unhandled},
		{112,unhandled}, {113,unhandled}, {114,unhandled}, {115,unhandled},
		{116,unhandled}, {117,unhandled}, {118,unhandled}, {119,unhandled},
		{120,unhandled}, {121,unhandled}, {122,unhandled}, {123,unhandled},
		{124,unhandled}, {125,unhandled}, {126,unhandled}, {127,unhandled},
	}; 


    #define declare_sequence(x,...) const char x[] =  {__VA_ARGS__}

    declare_sequence(clear_line, ascii::ESC, ascii::SQUARE_BRACKET_OPEN, ascii::TWO, ascii::K);
    declare_sequence(beginning_of_line, ascii::CR);

	short char_to_type(const char c) noexcept {
		return sequence_table[(int) c][1];
	}

    char type_to_char(const short s) noexcept {
        return decode_escape_sequence(s);
    }
	
	struct esc_seq {
		short type;
		int arguments[MAX_ESC_SEQ_ARGS];	
	};
}

struct stdio {
	stdio(const int in = 0, const int out = 1) noexcept : in(in), out(out) {
	    termios settings;
    	tcgetattr(in, &settings);
    	old_settings = settings;
    	settings.c_iflag |= IGNBRK;
    	settings.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
    	settings.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
    	settings.c_cflag &= ~CSIZE;
        settings.c_cflag |=  CS8;
     	settings.c_cc[VMIN] = 1;
    	settings.c_cc[VTIME] = 0;
    	tcsetattr(in, TCSANOW, &settings);
    }
    
    ~stdio() noexcept {
    	tcsetattr(in, TCSANOW, &old_settings);
    }

	int write(const char * buffer, const int size) const noexcept {
		return ::write(out, buffer, size);
	}

	int read(char * buffer, const int size) const noexcept {
		return ::read(in, buffer, size);
	}

	int read_nonblocking(char * buffer, const int size) const noexcept {
		int flags = fcntl(in, F_GETFL, 0);	
		fcntl(in, F_SETFL, flags | O_NONBLOCK);
	
		int read = this->read(buffer, size); 	

		fcntl(in, F_SETFL, flags);

		return read;
	}

	int in, out;

private:
    termios old_settings;    
};

template <typename T>
class io_stream {
public:
	template<typename ...Args>
	io_stream(Args&&... args) noexcept : io_impl(std::forward<Args>(args)...) {}

	int write(const short * buffer, const int size) const noexcept {
		return convert_and_write(buffer, size);
	}

	int write(const char * buffer, const int size) const noexcept {
		return io_impl.write(buffer, size);
	}

    int read(char * buffer, const int size) const noexcept {
        return io_impl.read(buffer, size);
    }

	int read(short * buffer, const int size) const noexcept {
		return read_converted(buffer, size);
	}

private:
	T io_impl;

	bool is_start_of_escape_sequence(const char to_test) const noexcept {
		return to_test == ascii::ESC;
	}

	void parse_args(const char * arg_buffer, const int size, int * arguments, const short type) const noexcept {
		if(!arg_buffer[0]) {
			switch(type) {
			case csi_sequence::arrow_up:
			case csi_sequence::arrow_down:
			case csi_sequence::arrow_left:
			case csi_sequence::arrow_right:
				arguments[0] = 1;
				arguments[1] = -1;
				arguments[2] = -1;
				arguments[3] = -1;
			break;
			default:
				arguments[0] = 0;
				arguments[1] = -1;
				arguments[2] = -1;
				arguments[3] = -1;
			}
		} else {
			// TODO(robin): handle numeric arguments
		}
	}
 
	csi_sequence::esc_seq parse_csi() const noexcept {
		char in_buffer[1] = { 0 };
		char arg_buffer[1024] = { 0 };
		int read = 0;
		csi_sequence::esc_seq seq;

		io_impl.read(in_buffer, sizeof(in_buffer));

		// numeric argument
		while((in_buffer[0] >= ascii::ZERO) && (in_buffer[0] <= ascii::SEMICOLON)) {
			arg_buffer[read++] = in_buffer[0];
			io_impl.read(in_buffer, sizeof(in_buffer));
		} 

		seq.type = csi_sequence::char_to_type(in_buffer[0]);
		parse_args(arg_buffer, sizeof(arg_buffer), seq.arguments, seq.type);

		return seq;
	}

	int read_maybe_escape_squence(short * buffer, const int size) const noexcept {
		char in_buffer[1];

		// don't wait -> only driver generated escape sequences are valid
		if(io_impl.read_nonblocking(in_buffer, 1) >= 1) {
			// check if this is really a escape sequence
			if(in_buffer[0] >= ascii::A && in_buffer[0] <= ascii::UNDERSCORE) {
				switch(in_buffer[0]) {
                case ascii::SQUARE_BRACKET_OPEN:
				{
					csi_sequence::esc_seq seq = parse_csi();
					switch(seq.type) {
					case csi_sequence::arrow_up:
					case csi_sequence::arrow_down:
					case csi_sequence::arrow_left:
					case csi_sequence::arrow_right:
					{
						// NOTE(robin): loses sequences, when arguments[0] > size
						int i = 0;
						for(i = 0; (i < seq.arguments[0]) && (i < size); i++) {
							buffer[i] = seq.type;	
						}
						return i;
					}
					break;

					default:
						buffer[0] = csi_sequence::unhandled;
						return 1;
					}
				}
				break;

				default:
					buffer[0] = csi_sequence::unhandled;
					return 1;
				break;
				}
			} else {
				// find better solution?
				buffer[0] = ascii::ESC;
				return 1;		
			}
		} else {
			buffer[0] = ascii::ESC;
			return 1;
		}
	}

	// NOTE(robin): implement bigger buffersize, if this is a performance bottleneck
	int read_converted(short * buffer, const int size) const noexcept {
		int num_read = 0;
		char in_buffer[1];

		while(num_read < size) {
			int read = io_impl.read(in_buffer, sizeof(in_buffer));

			if(read > 0) {
				if(is_start_of_escape_sequence(in_buffer[0])) {
					num_read += read_maybe_escape_squence(&buffer[num_read], size - num_read);
				} else {
					buffer[num_read] = in_buffer[0];
					num_read++;
				}
			} else {
				return num_read;
			}
		}

        return num_read;
	}

	int write_escape_sequence(const short seq) const noexcept {
        char buffer [3] = {ascii::ESC, ascii::SQUARE_BRACKET_OPEN}; 
        buffer[2] = csi_sequence::type_to_char(seq);
        return io_impl.write(buffer, 3);
	}

	int convert_and_write(const short * buffer, const int size) const noexcept {
		int ret;
        int written = 0;

		for(int i = 0; i < size; i++) {
			if(buffer[0] > 255) {
				written += write_escape_sequence(buffer[i]);
			} else {
				if((ret = io_impl.write((char *) &buffer[i], 1)) != 1) {
					return ret;
				} else {
                    written += ret;
                }
			}
		}

        return written;
	}
};

struct utf8char {
    char chars[4];
    
    utf8char() noexcept {
        chars[0] = 0;
        chars[1] = 0;
        chars[2] = 0;
        chars[3] = 0;
    }

    bool operator==(const utf8char & other) const noexcept {
        return (chars[0] == other.chars[0]) &&
               (chars[1] == other.chars[1]) &&
               (chars[2] == other.chars[2]) &&
               (chars[3] == other.chars[3]);
    }

    int length() const noexcept {
        if((chars[0] & 0b1111'0000) == 0b1111'0000) {
            return 4;
        } else if((chars[0] & 0b1110'0000) == 0b1110'0000) {
            return 3;
        } else if((chars[0] & 0b1100'0000) == 0b1100'0000) {
            return 2;
        } else {
            return 1;
        }
    }
};

struct utf8string {
public:
    utf8string() noexcept {}
    utf8string(const char * string) noexcept {
        while(*string) {
            utf8char c;

            c.chars[0] = string[0];
            string++;

            for(int i = 1; i < c.length(); i++) {
                c.chars[i] = string[0];
                string++;
            }

           this->string.push_back(c); 
        }
    }

    int length() const noexcept {
        return string.size();
    }

    int raw_length() const noexcept {
        return string.size() * 4;
    }

    char * raw_data() noexcept {
        if(string.size() > 0) {
            return &string[0].chars[0];
        }

        return nullptr;
    }

    void add(utf8char c) noexcept {
        string.push_back(c);
    }

    void insert(int index, utf8char c) noexcept {
        if(index >= (int) string.size()) {
            add(c);
        } else {
            string.insert(string.begin() + index, c);
        }
    }

    void erase(int index) noexcept {
        string.erase(string.begin() + index);
    }

    std::string unpack() const noexcept {
        std::string s;

        for(const auto & c : string) {
            for(int i = 0; i < 4; i++) {
                if(c.chars[i]) {
                    s.push_back(c.chars[i]); 
                }
            }
        }

        return s;
    }

    auto begin() {
        return string.begin();
    }

    auto end() {
        return string.end();
    }

    bool contains(utf8string s) {
        return std::search(string.begin(), string.end(), s.begin(), s.end()) != string.end();
    }

    void clear() noexcept {
        string.clear();
    }

private:
    std::vector<utf8char> string;
};

struct history {
    history(const int size) noexcept : index(0), size_(size) {
        hist.reserve(size);
    }
    
    void add(utf8string s) noexcept {
        hist.emplace(hist.begin() + (index++ % size_), s);
    }

    const int size() const noexcept {
        return hist.size();
    }

    int find_first(utf8string s) {
        for(int i = hist.size() - 1; i >= 0; i--) {
            if(hist[i % size_].contains(s)) {
                return -(i % size_ - hist.size());
            }
        }

        return -1;
    }

    utf8string get(const int index) const noexcept {
        return hist[(hist.size() - index) % size_];
    }
    
private:
    int index;
    int size_;
    std::vector<utf8string> hist;
};

template<typename T>
struct readline {
	template<typename ...Args>
    readline(int hist_size, Args&&... args) noexcept : io(std::forward<Args>(args)...), h(hist_size) {} 

    void searching_off() {
        if(searching) {
            io.write((short *) &csi_sequence::arrow_down, 1);
            io.write(csi_sequence::clear_line, sizeof(csi_sequence::clear_line));
            io.write((short *) &csi_sequence::arrow_up, 1);

            searching = false;
        }
    }

    void searching_on() {
        if(!searching) {
            before_search_pos = cursor_pos;

            io.write(crlf, 2);
            io.write((short *) &csi_sequence::arrow_up, 1);

            search_input.clear();

            searching = true;

            before_search = current_read;
            current_read.clear();
            cursor_pos = 0;
            search_hint = "history search: ";
        }
    }

    std::experimental::optional<utf8string> getline(const std::experimental::optional<utf8string> prompt = nullptr) noexcept {
        current_read.clear();
        search_input.clear();
        cursor_pos = 0;
        hist_index = 0;
        searching = false;

        if(prompt) {
            this->prompt = *prompt;    
        }

        do {
	        short buffer[1]; 

            update_line();

    		io.read(buffer, 1);

    		if((buffer[0] == ascii::ETX) | (buffer[0] == ascii::EOT)) {
                if(!searching) {
        			break;
                } else {
                    searching_off();
                    current_read = before_search;
                    cursor_pos = before_search_pos;
                    continue;
                }
    		}

    		switch (buffer[0]) {
                case csi_sequence::arrow_up: {
                    searching_off();

                    hist_index += (hist_index < h.size());

                    if(hist_index > 0) {
                        if(hist_index == 1) {
                            old_cursor_pos = cursor_pos;
                            old_read = current_read;
                        }

                        current_read = h.get(hist_index);
                        cursor_pos = current_read.length();
                    }

                    update_line();

                    break;
                }
                case csi_sequence::arrow_down: {
                    searching_off();

                    bool was_bigger = hist_index > 0;
                    hist_index -= was_bigger;

                    if(hist_index == 0 && was_bigger) {
                        current_read = old_read;
                        cursor_pos = old_cursor_pos;
                    } else if(was_bigger) {
                        current_read = h.get(hist_index);
                        cursor_pos = current_read.length();
                    }

                    update_line();

                    break;
                }
                case csi_sequence::arrow_left: {
                    searching_off();
                    cursor_pos -= (cursor_pos > 0);
                }
                break;
                case csi_sequence::arrow_right: {
                    searching_off();
                    cursor_pos += (cursor_pos < current_read.length());
                }
                break;
                case ascii::DEL:
                    if(searching) {
                        if(search_input.length() > 0) {
                            search_input.erase(search_input.length() - 1);
                        }

                        int index = h.find_first(search_input); 
                        
                        if(index != -1) {
                            search_hint = "history search: ";
                            current_read = h.get(index);
                            hist_index = index;
                        } else {
                            search_hint = "history search (not found): ";
                        }
                    } else {
                        if((cursor_pos - 1) < current_read.length() && cursor_pos > 0) {
                            current_read.erase(cursor_pos - 1);
                            cursor_pos -= (cursor_pos > 0);
                        }
                    }
                break;
                case ascii::CR: {
                    utf8string ret = current_read;

                    current_read.clear();
                    
                    searching_off();
                    io.write(crlf, 2);

                    return ret;
                    break;
                }
                case ascii::DC2: {
//                    searching = !searching;

                    if(!searching) {
                        searching_on();
                    } else {
                        searching_off();
                    }
                    break;
                }
                default:
                    utf8char to_read;
                    to_read.chars[0] = buffer[0];

                    // NOTE(robin): minus one, because we read already one char
                    if((to_read.length() - 1) > 0) {
                        io.read(&to_read.chars[1], to_read.length() - 1);
                    }

                    if(searching) {
                        search_input.add(to_read);
                        
                        int index = h.find_first(search_input); 
                        
                        if(index != -1) {
                            search_hint = "history search: ";
                            current_read = h.get(index);
                            hist_index = index;
                        } else {
                            search_hint = "history search (not found): ";
                        }
                    } else {
                        current_read.insert(cursor_pos, to_read);
                        cursor_pos++;
                    }
                break;
    		}
        } while(true);

        return {}; 
    }

    void update_line() noexcept {
        io.write(csi_sequence::clear_line, sizeof(csi_sequence::clear_line));
        io.write(csi_sequence::beginning_of_line, sizeof(csi_sequence::beginning_of_line));

        io.write(prompt.raw_data(), prompt.raw_length());
        io.write(current_read.raw_data(), current_read.raw_length());

        io.write(csi_sequence::beginning_of_line, sizeof(csi_sequence::beginning_of_line));

        if(searching) {
            io.write((short *) &csi_sequence::arrow_down, 1);

            io.write(csi_sequence::clear_line, sizeof(csi_sequence::clear_line));
            io.write(csi_sequence::beginning_of_line, sizeof(csi_sequence::beginning_of_line));

            io.write(search_hint.c_str(), search_hint.size());
            io.write(search_input.raw_data(), search_input.raw_length());
//            io.write(current_read.raw_data(), current_read.raw_length());

            io.write((short *) &csi_sequence::arrow_up, 1);
        }

        io.write(csi_sequence::beginning_of_line, sizeof(csi_sequence::beginning_of_line));

        for(int i = 0; i < cursor_pos + (int) prompt.length(); i++) {
            io.write((short *) &csi_sequence::arrow_right, 1);
        }
    }

    void history_add(const utf8string s) noexcept {
        h.add(s);
    }
private:
    bool searching = false;
    int cursor_pos;
    int hist_index;
    int old_cursor_pos;
    int before_search_pos;

    utf8string old_read;
    utf8string current_read;
    utf8string prompt; 
    utf8string before_search;
    utf8string search_input;

    std::string search_hint;

    io_stream<T> io;
    history h;

    char crlf[2] = {ascii::CR, ascii::LF};
};

template<typename T>
bool whitespace_only(const T & string) {
    auto f = [](unsigned char const c) { return std::isspace(c); };
    return std::all_of(string.begin(), string.end(), f);
}

int main() {
    readline<stdio> rl(1024);
    auto prompt = std::experimental::optional<utf8string>(std::experimental::in_place_t {}, "λ ");
    auto line = rl.getline(prompt);
    while(line) {
        auto l = line->unpack();
        if(!whitespace_only(l)) {
            rl.history_add(*line);
        }
        
        line = rl.getline(prompt);
    }
}
