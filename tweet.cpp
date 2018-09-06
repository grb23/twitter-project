#include <iostream>
#include <string>
#include <string>
#include <vector>
#include <ctime>
#include <set>
#include "datetime.h"
#include "user.h"
#include "tweet.h"

Tweet::Tweet() : tweetUser(NULL), tweetText("")
{
	DateTime currTime;
	tweetTime = currTime;
}

// kept for unit testing but not used
Tweet::Tweet(User* user, DateTime& time, std::string& text)
{
	tweetUser = user;
	tweetTime = time;
	tweetText = text;
}

// use this constructor
Tweet::Tweet(User* user, DateTime& time, std::string& text, std::set<std::string>& hashtagSet)
{
	tweetUser = user;
	tweetTime = time;
	tweetText = text;
	tweetHashTags = hashtagSet;
}

DateTime const & Tweet::time() const
{
	return tweetTime;
}

std::string const & Tweet::text() const
{
	return tweetText;
}

// for use in twiteng and other places
User* Tweet::user() const
{
	return tweetUser;
}

std::set<std::string> Tweet::hashTags() const
{
	return tweetHashTags;
}

bool Tweet::operator<(const Tweet& other) const 
{
	if(this->time() < other.time())
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::ostream& operator<<(std::ostream& os, const Tweet& t)
{
	os << t.time() << t.tweetUser->name() << " " << t.text();
	return os;
}