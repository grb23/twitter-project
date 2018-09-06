#include "cmdhandler.h"
#include "util.h"
#include "hash.h"
#include <vector>
using namespace std;


// TRENDING
TrendingHandler::TrendingHandler()
{

}

TrendingHandler::TrendingHandler(Handler* next) : Handler(next)
{

}

bool TrendingHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TRENDING";
}

Handler::HANDLER_STATUS_T TrendingHandler::process(TwitEng* eng, std::istream& instr) const
{
	int num_tags = 0;
	instr >> num_tags;

	eng->trending(num_tags);

	return HANDLER_OK;
}


// LOGIN
LoginHandler::LoginHandler()
{

}

LoginHandler::LoginHandler(Handler* next) : Handler(next)
{

}

bool LoginHandler::canHandle(const std::string& cmd) const
{
	return cmd == "LOGIN";
}

Handler::HANDLER_STATUS_T LoginHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::string username = "";
	std::string password = "";

	instr >> username;
	instr >> password;

	std::map<std::string, User*> userMap = eng->getCo();
	std::map<std::string, unsigned long long> hashCodeMap = eng->getHashCo();

	if(userMap.find(username) == userMap.end())
	{
		std::cout << "Invalid username/password" << std::endl;
		return HANDLER_ERROR;
	}

	if(hashCodeMap.find(username) == hashCodeMap.end())
	{
		std::cout << "Invalid username/password" << std::endl;
		return HANDLER_ERROR;
	}

	unsigned long long hashedNum = 0;
	hashedNum = calculateHash(password);
	unsigned long long hashFromMap = hashCodeMap[username];

	if(hashedNum != hashFromMap)
	{
		std::cout << "Invalid username/password" << std::endl;
		return HANDLER_ERROR;
	}
	else if(hashedNum == hashFromMap)
	{
		std::cout << "Login successful" << std::endl;
		eng->login(username);

		return HANDLER_OK;
	}
	

	return HANDLER_OK;
}


// LOGOUT
LogoutHandler::LogoutHandler()
{

}

LogoutHandler::LogoutHandler(Handler* next) : Handler(next)
{

}

bool LogoutHandler::canHandle(const std::string& cmd) const
{
	return cmd == "LOGOUT";
}

Handler::HANDLER_STATUS_T LogoutHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->logout();
	std::cout << "Logout successful" << std::endl;

	return HANDLER_OK;
}


// SCC
SCCHandler::SCCHandler()
{

}

SCCHandler::SCCHandler(Handler* next) : Handler(next)
{

}

bool SCCHandler::canHandle(const std::string& cmd) const
{
	return cmd == "SCC";
}

Handler::HANDLER_STATUS_T SCCHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::string filename;
	instr >> filename;

	if(filename == "")
	{
		std::cout << "error: no filename given" << std:: endl;
		return HANDLER_ERROR;
	}
	
	eng->makeSCCFile(filename);
	return HANDLER_OK;
}


// FOLLOW
FollowHandler::FollowHandler()
{

}

FollowHandler::FollowHandler(Handler* next) : Handler(next)
{

}

bool FollowHandler::canHandle(const std::string& cmd) const
{
	return cmd == "FOLLOW";
}

Handler::HANDLER_STATUS_T FollowHandler::process(TwitEng* eng, std::istream& instr) const
{
	if(eng->loginState() == false)
	{
		std::cout << "error: no user logged in" << std::endl;
		return HANDLER_ERROR;
	}

	const User* currentLoggedIn = eng->getCurrentUser();

	if(currentLoggedIn == NULL)
	{
		std::cout << "error: no user logged in" << std::endl;
		return HANDLER_ERROR;
	}
	
	std::string name_of_follower = currentLoggedIn->name();
	std::string name_to_follow;
	
	instr >> name_to_follow;
	
	std::map<std::string, User*> hold = eng->getCo();

	if(hold.find(name_of_follower) == hold.end())
	{
		std::cout << "error: could not find one or both of those users" << std::endl;
		return HANDLER_ERROR;
	}

	if(hold.find(name_to_follow) == hold.end())
	{
		std::cout << "error: could not find one or both of those users" << std::endl;
		return HANDLER_ERROR;
	}

	User* user1 = (hold.find(name_of_follower))->second;
	User* user2 = (hold.find(name_to_follow))->second;
	
	// kept this so if anyone wants to see what happens they can
	//std::cout << "you want " << user1->name() << " to follow " << user2->name() << std::endl;
	
	(hold[name_of_follower])->User::addFollowing(user2);
	
	(hold[name_to_follow])->User::addFollower(user1);

	return HANDLER_OK;
}


// SAVE
SaveHandler::SaveHandler()
{

}

SaveHandler::SaveHandler(Handler* next) : Handler(next)
{

}

bool SaveHandler::canHandle(const std::string& cmd) const
{
	return cmd == "SAVE";
}

Handler::HANDLER_STATUS_T SaveHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::string filename;
	instr >> filename;

	if(filename == "")
	{
		std::cout << "error: no filename given" << std:: endl;
		return HANDLER_ERROR;
	}
	
	eng->makeSaveFile(filename);
	return HANDLER_OK;
}


// TWEET
TweetHandler::TweetHandler()
{

}

TweetHandler::TweetHandler(Handler* next) : Handler(next)
{

}

bool TweetHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TWEET";
}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, std::istream& instr) const
{
	if(eng->loginState() == false)
	{
		std::cout << "error: no user logged in" << std::endl;
		return HANDLER_ERROR;
	}

	const User* currentLoggedIn = eng->getCurrentUser();

	if(currentLoggedIn == NULL)
	{
		std::cout << "error: no user logged in" << std::endl;
		return HANDLER_ERROR;
	}

	std::string creator = currentLoggedIn->name();

	std::map<std::string, User*> hold = eng->getCo();

	if(hold.find(creator) == hold.end())
	{
		std::cout << "error: this user was not in the original database" << std::endl;
		return HANDLER_ERROR;
	}
	else
	{	
		std::string text;
		getline(instr, text);
		text = trim(text);
		if(text == "")
		{
			std::cout << "error: empty tweets are not allowed" << std::endl;
			return HANDLER_ERROR;
		}
		std::set<std::string> hashtags = eng->parseOutSet(text, '#');
		DateTime currDate;

		eng->addTweet(creator, currDate, text, hashtags);
		return HANDLER_OK;
	}

}


// OR
OrHandler::OrHandler()
{

}

OrHandler::OrHandler(Handler* next) : Handler(next)
{

}

bool OrHandler::canHandle(const std::string& cmd) const
{
	return cmd == "OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, std::istream& instr) const
{
	vector<string> terms;
	while(!instr.eof())
	{
		string hold;
		instr >> hold;

		// terms need to be uppercase
		convUpper(hold);
		terms.push_back(hold);
	}


	vector<Tweet*> result = eng->search(terms, 1);
	displayHits(result);

	return HANDLER_OK;
}


// AND
AndHandler::AndHandler()
{

}

AndHandler::AndHandler(Handler* next) : Handler(next)
{

}

bool AndHandler::canHandle(const std::string& cmd) const
{
	return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, std::istream& instr) const
{
	vector<string> terms;
	while(!instr.eof())
	{
		string hold;
		instr >> hold;

		// terms need to be uppercase
		convUpper(hold);
		terms.push_back(hold);
	}


	vector<Tweet*> result = eng->search(terms, 0);
	displayHits(result);

	return HANDLER_OK;
}


// QUIT
QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next) : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
	return cmd == "QUIT";
}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}

