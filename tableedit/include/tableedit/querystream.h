#ifndef _querystream_h_
#define _querystream_h_

// helper for forming queries
class QueryStream {
private:
	std::stringstream out;
	bool First;
public:
	QueryStream() { First = true; };
	template <typename T> void Add(T Value) { First==true ? First=false : out << ", "; out << Value; };
	template <typename T> void Add(T Value, std::string Separator) { First==true ? First=false : out << " " << Separator << " "; out << Value; };
	void New() { First = true; };
	template <typename T> QueryStream& operator<<(T Value) { out << Value; return *this;};
	std::string str() { return out.str(); };
};

#endif
