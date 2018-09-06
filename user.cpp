#include <string>
#include <set>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include "user.h"
#include "tweet.h"
#include "util.h"

User::User(std::string name)
{
	userName = name;
}

User::~User()
{
	
	std::list<Tweet*>::iterator it;
	for (it = userTweets.begin(); it != userTweets.end(); ++it)
	{
		delete *it;
	}
	
}

std::string User::name() const
{
	return this->userName;
}

std::set<User*> User::followers() const
{
	return this->userFollowers;
}

std::set<User*> User::following() const
{
	return this->userFollowing;
}
  
std::list<Tweet*> User::tweets() const 
{
	return this->userTweets;
}

std::list<Tweet*> User::mentions() const 
{
	return this->userMentions;
}

void User::addFollower(User* u)
{
	userFollowers.insert(u);
}

void User::addFollowing(User* u)
{
	userFollowing.insert(u);
}

void User::addTweet(Tweet* t)
{
	userTweets.push_back(t);
}

void User::addMention(Tweet* t)
{
	userMentions.push_back(t);
}

// mentions
std::vector<Tweet*> User::getMentions()
{
	std::list<Tweet*> userMentions = this->mentions();
	std::vector<Tweet*> resultMentions;

	std::list<Tweet*>::iterator listIt;
	for(listIt = userMentions.begin(); listIt != userMentions.end(); ++listIt)
	{
		resultMentions.push_back(*listIt);
	}

	return resultMentions;
}

std::vector<Tweet*> User::getFeed()
{
	// get following users
	std::set<User*> followingSetHold = this->following();
	std::list<Tweet*> tweetFeedHold = this->tweets();

	std::vector<Tweet*> resultFeed;

	std::set<User*>::iterator it;
	std::list<Tweet*>::iterator innerIt;
	
	// get everyone's tweets into tweetFeedHold
	for(it = followingSetHold.begin(); it != followingSetHold.end(); ++it)
	{
		std::list<Tweet*> listHold = (*it)->tweets();
		
		// I do this to get everything
		for(innerIt = listHold.begin(); innerIt != listHold.end(); ++innerIt)
		{
			tweetFeedHold.push_back(*innerIt);
		}

	}

	// create result
	std::list<Tweet*>::iterator listIt;
	for(listIt = tweetFeedHold.begin(); listIt != tweetFeedHold.end(); ++listIt)
	{
		resultFeed.push_back(*listIt);
	}

	return resultFeed;
}
