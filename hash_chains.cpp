#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list> 
#include <map>

using std::string;
using std::vector;
using std::cin;

struct Query {
	string type, s;
	size_t ind;
};

class QueryProcessor 
{
	int bucket_count;
	// store all strings in one vector
	vector<string> elems;
	std::map <long long, std::list <string> > mapChain;

	size_t hash_func(const string& s) const 
	{
		static const size_t multiplier = 263;
		static const size_t prime = 1000000007;
		unsigned long long hash = 0;
		for (int i = static_cast<int> (s.size()) - 1; i >= 0; --i)
			hash = (hash * multiplier + s[i]) % prime;
		return hash % bucket_count;
	}

public:
	explicit QueryProcessor(int bucket_count) : bucket_count(bucket_count) {}

	Query readQuery() const 
	{
		Query query;
		cin >> query.type;
		if (query.type != "check")
			cin >> query.s;
		else
			cin >> query.ind;
		return query;
	}

	void writeSearchResult(bool was_found) const 
	{
		std::cout << (was_found ? "yes\n" : "no\n");
	}

	void processQuery(const Query& query) 
	{
		if (query.type == "check") 
		{
			// use reverse order, because we append strings to the end
			for (int i = static_cast<int>(elems.size()) - 1; i >= 0; --i)	// took revrse to print backward
				if (hash_func(elems[i]) == query.ind)
					std::cout << elems[i] << " ";
			std::cout << "\n";

			auto check = mapChain.find(query.ind);
			if (check != mapChain.end())
			{
				std::list<string> tempList;
				tempList = check->second;
				for (auto it = tempList.begin(); it != tempList.end(); ++it)
					std::cout << *it << ' ';
			}

		}
		else 
		{
			vector<string>::iterator it = std::find(elems.begin(), elems.end(), query.s);
			//std::map <long long, std::list <string> > :: iterator it2 = std::find(mapChain.begin(), mapChain.end(), query.s);
			if (query.type == "find")
			{
				writeSearchResult(it != elems.end());
				//writeSearchResult(it2 != mapChain.end());
				long long key = hash_func(query.s);
				auto fnd = mapChain.find(key);
				if(fnd == mapChain.end())
					std::cout << ("no\n");
				else
				{
					std::list<string> tempList;
					tempList = fnd->second;
					for (auto it = tempList.begin(); it != tempList.end(); ++it)
						if (*it ==  query.s)
						{
							std::cout << ("yes\n");
						} 
				}

			}
			else if (query.type == "add") 
			{
				if (it == elems.end())
					elems.push_back(query.s);
				// start insert
				long long key = hash_func(query.s);
				auto fnd= mapChain.find(key);
				if (fnd != mapChain.end())	// if already present 
				{
					std::list<string> *tempList;
					tempList = &(fnd->second);
					tempList->push_front(query.s); 
				}
				else
				{
					std::list<string> tempList;
					tempList.push_back(query.s);
					mapChain.insert(std::pair<long long, std::list <string> >(key, tempList));
				}

			}
			else if (query.type == "del") 
			{
				if (it != elems.end())
					elems.erase(it);

				long long key = hash_func(query.s);
				auto fnd = mapChain.find(key);
				if (fnd == mapChain.end())
				{}
				else
				{
					std::list<string> tempList;
					tempList = fnd->second;
					for (auto it = tempList.begin(); it != tempList.end(); ++it)
						if (*it == query.s)
						{
							//std::cout << ("yes\n");
							tempList.erase(it);
							return;
						}
					
				}
			}
		}
	}

	void processQueries() 
	{
		int n;
		cin >> n;
		for (int i = 0; i < n; ++i)
			processQuery(readQuery());
	}
};

int main() {
	std::ios_base::sync_with_stdio(false);
	int bucket_count;
	cin >> bucket_count;
	QueryProcessor proc(bucket_count);
	proc.processQueries();
	return 0;
}
