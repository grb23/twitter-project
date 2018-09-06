#include <map>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stack>
#include "user.h"
#include "tweet.h"
#include "datetime.h"
#include "twiteng.h"
#include "util.h"
#include "msort.h"
#include "graph.h"
#include "heap.h"
#include "tagdata.h"


TwitEng::TwitEng() : trending_(2)
{
	isLoggedIn = false;
	current_user = NULL;
}

TwitEng::~TwitEng()
{
	std::map<std::string, User*>::iterator it;

	for (it = userCollection.begin(); it != userCollection.end(); ++it)
	{
		delete it->second;
	}

}


void TwitEng::trending(int num_tags)
{
	Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> holdTrend = getHeap();

	const std::vector<TagData> h = holdTrend.getStor();

	// fix bounds if needed
	if((unsigned)num_tags > (unsigned)h.size())
	{
		num_tags = h.size();
	}

	for(int i = 0; i < num_tags; i++)
	{
		string tag = h[i].tag;
		convLower(tag);
		std::cout << tag << " : " << h[i].num << std::endl;
	}
}

void TwitEng::makeSCCFile(std::string filename)
{
	std::ofstream ofile(filename.c_str());

	int numV = userCollection.size();

	std::map<User*, std::vector<User*> > adjMap;

	std::map<std::string, User*>::iterator it;
	std::set<User*>::iterator it2;

	// CREATION OF THE LARGE ADJACENCY
	for (it = userCollection.begin(); it != userCollection.end(); ++it)
	{
		User* hold = it->second;

		adjMap.insert(std::pair<User*,std::vector<User*> >(hold, std::vector<User*>()));

		std::set<User*> followingHold = it->second->following();
		for(it2 = followingHold.begin();
			it2 != followingHold.end();
			++it2)
		{
			User* hold2 = *it2;
			adjMap[hold].push_back(hold2);
		}
	}

	std::map<User*, std::vector<User*> >::iterator adjIt;



	int index = 0;
	std::map<std::string, int> indexMap;
	std::map<int, std::string> indexMap2;

	// EVERY USER IS INDEXED
	for (adjIt = adjMap.begin(); adjIt != adjMap.end(); ++adjIt)
	{
		indexMap.insert(std::pair<std::string, int>(adjIt->first->name(), index));
		indexMap2.insert(std::pair<int, std::string>(index, adjIt->first->name()));
		index++;
	}

	std::set<std::pair<int,int> > bigSet;

	// BIG SET HOLD V,W PAIRS WHICH REPRESENT EDGES
	for (adjIt = adjMap.begin(); adjIt != adjMap.end(); ++adjIt)
	{
		int v = indexMap[adjIt->first->name()];

		for(unsigned int i = 0; i < adjIt->second.size(); i++)
		{
			int w = indexMap[adjIt->second[i]->name()];
			bigSet.insert(std::pair<int, int>(v,w));
		}
	}


	std::set<std::pair<int,int> >::iterator setIt;
    Graph userGraph(numV);

    // ADD EDGES
	for (setIt = bigSet.begin(); setIt != bigSet.end(); ++setIt)
	{
		userGraph.addEdge(setIt->first, setIt->second);
	}

	// GET THE VECTOR OF COMPONENTS
	std::vector<std::string> resultVector = userGraph.SCC(indexMap2);

	// FORMATTING FOR OUTPUT
	std::set<std::string> tempSet;
	int tcount = 1;
    for(unsigned int i = 0; i < resultVector.size(); i++)
    {
    	if(resultVector.at(i) == "END")
    	{
    		ofile << "Component " << tcount << std::endl;
    		std::set<string>::iterator it;
    		for(it = tempSet.begin(); it != tempSet.end(); ++it)
    		{
    			ofile << *it << std::endl;
    		}
    		ofile << std::endl;
    		tempSet.clear();
    		tcount++;
    	}
    	else
    	{
    		tempSet.insert(resultVector.at(i));
    	}
    }

}


void TwitEng::makeSaveFile(std::string filename)
{
	std::ofstream ofile(filename.c_str());

	// OFILE THE NUM OF USERS
	ofile << userCollection.size() << std::endl;

	std::map<std::string, User*>::iterator it;
	std::set<User*>::iterator it2;

	// OFILE THE USERS
	for (it = userCollection.begin(); it != userCollection.end(); ++it)
	{
		ofile << it->first;
		ofile << " ";

		ofile << userHashCodeMap[it->first] << " ";

		std::set<User*> followingHold = it->second->following();
		for(it2 = followingHold.begin();
			it2 != followingHold.end();
			++it2)
		{
			ofile << (*it2)->name() << " ";
		}
		ofile << std::endl;
	}

	// OFILE THE TWEETS
	std::map<std::string, User*>::iterator mapIt;
	std::list<Tweet*>::iterator listIt;
	for(mapIt = userCollection.begin(); mapIt != userCollection.end(); ++mapIt)
	{
		std::list<Tweet*> tweetList = (userCollection[mapIt->first])->tweets();

		for(listIt = tweetList.begin();
			listIt != tweetList.end();
			++listIt)
		{
			ofile << *(*listIt) << std::endl;
		}
	}
}

bool TwitEng::checkDateFormatting(std::string dateLine)
{
	bool fail = false;
	for(unsigned int i = 0; i < dateLine.length(); i++)
	{
		if((i >= 0 && i < 4) ||
			(i >= 5 && i < 7) ||
			(i >= 8 && i < 10) ||
			(i >= 11 && i < 13) ||
			(i >= 14 && i < 16) ||
			(i >= 17 && i < 19))
		{
			if(!isdigit(dateLine.at(i)))
			{
				fail = true;
				break;
			}

		}
		else if(i == 4 || i == 7)
		{
			if(dateLine.at(i) != '-')
			{
				fail = true;
				break;
			}
		}
		else if(i == 13 || i == 16)
		{
			if(dateLine.at(i) != ':')
			{
				fail = true;
				break;
			}
		}
		else if(i == 10)
		{
			if(!isspace(dateLine.at(i)))
			{
				fail = true;
				break;
			}
		}
	}

	return fail;
}

std::map<std::string, User*> const TwitEng::getCollection() const
{
	return userCollection;
}

const std::map<std::string, User*>& TwitEng::getCo() const
{
	return userCollection;
}

const std::map<std::string, unsigned long long>& TwitEng::getHashCo() const
{
	return userHashCodeMap;
}

const Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher>& TwitEng::getHeap() const
{
	return trending_;
}

const bool& TwitEng::loginState() const
{
	return isLoggedIn;
}

const User* TwitEng::getCurrentUser() const
{
	return current_user;
}

std::set<std::string> TwitEng::parseOutSet(std::string tweet, char ch)
{
	std::set<std::string> localSet;
	std::stringstream ss;
	std::string garbage;

	ss << tweet;

		while(getline(ss, garbage, ch))
		{
			std::string content;
			ss >> content;
			convUpper(content);
			// check if it's already in, not unique
			if(content != "")
			{
				localSet.insert(content);
			}
			content.clear();
		}
		ss.clear();
		garbage.clear();

	return localSet;
}

std::set<std::string> TwitEng::parseOutSetCase(std::string tweet, char ch)
{
	std::set<std::string> localSet;
	std::stringstream ss;
	std::string garbage;

	ss << tweet;

		while(getline(ss, garbage, ch))
		{
			std::string content;
			ss >> content;
			// check if it's already in, not unique
			if(content != "")
			{
				localSet.insert(content);
			}
			content.clear();
		}
		ss.clear();
		garbage.clear();

	return localSet;
}



bool TwitEng::parse(char* filename)
{
	std::ifstream ifile;
	std::stringstream ss;
	ifile.open(filename);

	// for some reason you gave bad file
	if(ifile.fail())
	{
		return true;
	}
	int numOfUsers;
	std::string content;

	getline(ifile, content);

	numOfUsers = atoi(content.c_str());

	std::string hold;
	std::string firstName;

	// FOLLOWING FOLLOWER SETUP
	for(int i = 0; i < numOfUsers; i++)
	{
		getline(ifile, content);
		if(content != "")
		{
			unsigned long long hashCode = 0;
			ss << content;
			ss >> firstName;
			ss >> hashCode;

			this->addUser(firstName);
			this->addHashCode(firstName, hashCode);

			while(!ss.eof())
			{
				ss >> hold;

				if(userCollection.find(hold) == userCollection.end())
				{
					this->addUser(hold);
				}

				userCollection[firstName]->addFollowing(userCollection[hold]);
				userCollection[hold]->addFollower(userCollection[firstName]);
			}
		}
		ss.clear();
	}
	// ---------


	// ACTUAL TWEETS
	while(!ifile.eof())
	{
		getline(ifile, content);

		if(content != "")
		{
			std::string dateLine;
			std::string creator;

			ss << content;

			// DATE CONSTRUCTION -------
			for(int i = 0; i < 2; i++)
			{
				ss >> hold;
				if(dateLine == "")
				{
					dateLine = hold;
				}
				else
				{
					dateLine += " " + hold;
				}
			}

			//bool isCorrectFormat = checkDateFormatting(dateLine);


			for(unsigned int i = 0; i < dateLine.length(); i++)
			{
				if(dateLine.at(i) == ':')
				{
					dateLine[i] = ' ';
				}
				if(dateLine.at(i) == '-')
				{
					dateLine[i] = ' ';
				}
			}

			int dateInt;
			int dateHold[6] = {};

			std::stringstream ssDate;
			ssDate << dateLine;
			for(int i = 0; i < 6; i++)
			{
				ssDate >> dateInt;
				dateHold[i] = dateInt;
			}
			ssDate.clear();

			DateTime newdate(dateHold[3],
				dateHold[4],
				dateHold[5],
				dateHold[0],
				dateHold[1],
				dateHold[2]);

			// END OF DATE CONSTRUCTION -------

			// ASSIGN TWEET TO THIS USER ------
			ss >> creator;

			std::string textOfTweet;
			getline(ss, textOfTweet);

			// THE TWEET CAN'T BE NOTHING AND THE CREATOR MUST HAVE ALREADY BEEN INPUTTED
			if(textOfTweet != "" && (userCollection.find(creator) != userCollection.end()))
			{
				textOfTweet.erase(textOfTweet.begin());

				// HASHTAG COLLECTION
				std::set<std::string> hashtagSetFromTweet = parseOutSet(textOfTweet, '#');

				this->addTweet(creator, newdate, textOfTweet, hashtagSetFromTweet);
				// END OF TWEET CREATION -----------
			}
		}

		ss.clear();
	}

	return false;
}

// unused but left for unit testing
void TwitEng::addTweet(std::string& username, DateTime& time, std::string& text)
{
	if(userCollection.find(username) == userCollection.end())
	{
		return;
	}

	Tweet* tweetToAdd = new Tweet(userCollection[username], time, text);
	(userCollection[username])->addTweet(tweetToAdd);
}

// overload with hashtags
void TwitEng::addTweet(std::string& username, DateTime& time, std::string& text, std::set<std::string>& hashtags)
{
	if(userCollection.find(username) == userCollection.end())
	{
		return;
	}

	// TWEET CONSTRUCTOR
	Tweet* tweetToAdd = new Tweet(userCollection[username], time, text, hashtags);

	bool firstExists = false;
	bool atMentionedIsReal = false;

	std::string temptext = text;
	ltrim(temptext);

	// IF THE FIRST THING IS A VALID @, GET IT
	std::string firstMention;
	if(temptext.at(0) == '@')
	{
		firstExists = true;
		std::stringstream ss2;
		ss2 << temptext;

		ss2 >> firstMention;
		firstMention[0] = ' ';
		ltrim(firstMention);

		if(userCollection.find(firstMention) != userCollection.end())
		{
			atMentionedIsReal = true;
		}
	}

	(userCollection[username])->addTweet(tweetToAdd);

	// IF IT DOESN'T START WITH AN @
	if(firstExists == false)
	{
		//(userCollection[username])->addTweet(tweetToAdd);
		std::set<std::string> mentionedUsersTweet = parseOutSetCase(text, '@');

		std::set<std::string>::iterator it;
		for(it = mentionedUsersTweet.begin(); it != mentionedUsersTweet.end(); ++it)
		{
			// whoever is mentioned needs to be real
			if(userCollection.find(*it) != userCollection.end())
			{
				(userCollection[*it])->addMention(tweetToAdd);
			}
		}
	}
	else if(atMentionedIsReal == true && firstExists == true)
	{
		// IT DOES START WITH @
		// whoever is mentioned needs to be real
		if(userCollection.find(firstMention) != userCollection.end())
		{
			(userCollection[firstMention])->addMention(tweetToAdd);
		}
	}

	// HASHTAG MAP ASSIGNMENT
	std::set<std::string>::iterator it;

	for(it = hashtags.begin();
	it != hashtags.end();
	++it)
	{
		std::string holdtag;
		holdtag = *it;

		hashtagMap[holdtag].insert(tweetToAdd);

		// new tag not in heap, has 1 occ so far
		if(tagToHeapMap.find(holdtag) == tagToHeapMap.end())
		{
			TagData temp(holdtag, 1);
			trending_.push(temp);
			tagToHeapMap.insert(std::make_pair(holdtag, 1));
		}
		else if(tagToHeapMap.find(holdtag) != tagToHeapMap.end())
		{
			int numOcc = tagToHeapMap[holdtag];
			tagToHeapMap[holdtag]++;

			TagData oldTag(holdtag, numOcc);
			TagData newTag(holdtag, numOcc+1);
			trending_.decreaseKey(oldTag, newTag);

		}
	}
}

std::set<Tweet*> TwitEng::setUnion(std::set<Tweet*>& hold, std::set<Tweet*> tweetSet)
{
	std::set<Tweet*>::iterator it;
	std::set<Tweet*> sendBackSet;

	sendBackSet = hold;

	for(it = tweetSet.begin(); it != tweetSet.end(); ++it)
	{
		sendBackSet.insert(*it);
	}

	return sendBackSet;
}


std::set<Tweet*> TwitEng::setIntersect(std::set<Tweet*>& hold, std::set<Tweet*> tweetSet)
{
	std::set<Tweet*>::iterator it;
	std::set<Tweet*>::iterator it2;
	std::set<Tweet*> sendBackSet;

	for(it = hold.begin(); it != hold.end(); ++it)
	{
		for(it2 = tweetSet.begin();
			it2 != tweetSet.end();
			++it2)
		{

			if((*it)->hashTags() == (*it2)->hashTags())
			{
				sendBackSet.insert(*it);
			}
		}
	}

	return sendBackSet;

}

std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy)
{
	std::vector<Tweet*> resultVector;
	std::set<Tweet*> resultSet;

	if(strategy != 0 && strategy != 1)
	{
		return resultVector;
	}
	else if(terms.size() == 0)
	{
		return resultVector;
	}
	else if(strategy == 0)
	{
		// AND
		// must be in all
		if(terms.size() == 1)
		{
			resultSet = hashtagMap[terms.at(0)];
		}
		else
		{
			resultSet = hashtagMap[terms.at(0)];
			for(unsigned int i = 0; i < terms.size(); i++)
			{
				resultSet = setIntersect(resultSet, hashtagMap[terms.at(i)]);
			}
		}

		// conversion
		std::set<Tweet*>::iterator it;
		for(it = resultSet.begin(); it != resultSet.end(); ++it)
		{
			resultVector.push_back(*it);
		}

		return resultVector;
	}
	else if(strategy == 1)
	{
		// OR
		// any
		if(terms.size() == 1)
		{
			resultSet = hashtagMap[terms.at(0)];
		}
		else
		{
			resultSet = hashtagMap[terms.at(0)];
			for(unsigned int i = 0; i < terms.size(); i++)
			{
				resultSet = setUnion(resultSet, hashtagMap[terms.at(i)]);
			}
		}

		// conversion
		std::set<Tweet*>::iterator it;
		for(it = resultSet.begin(); it != resultSet.end(); ++it)
		{
			resultVector.push_back(*it);
		}

		return resultVector;
	}

	return resultVector;
}

void TwitEng::dumpFeeds()
{
	std::map<std::string, User*>::iterator it;
	std::set<Tweet*> feedSet;

	// FEED DUMP
	for(it = userCollection.begin(); it != userCollection.end(); ++it)
	{
		std::string currUser = it->first;
		std::string nameOfFile = it->first;

		nameOfFile += ".feed";

		std::ofstream ofile(nameOfFile.c_str());
		ofile << currUser << std::endl;

		std::vector<Tweet*> feedHold = userCollection[currUser]->getFeed();
		std::vector<Tweet*> finalResult;

		// GIANT UNFILTERD COLLECTION OF POTENTIAL FEED TWEETS
		for(unsigned int i = 0; i < feedHold.size(); i++)
		{
			Tweet* tweetOn = feedHold.at(i);

			if(tweetOn->text().at(0) == '@')
			{
				// START WITH @ CASE
				std::stringstream ss2;
				ss2 << tweetOn->text();
				std::string firstMention;
				ss2 >> firstMention;
				firstMention[0] = ' ';
				ltrim(firstMention);

				User* usernameU = userCollection[currUser];
				User* mentionU = userCollection[firstMention];

				std::set<User*> userHold = usernameU->following();
				std::set<User*> mentionHold = mentionU->following();

				// MUST FOLLOW EACH OTHER
				if(tweetOn->user() == usernameU)
				{
					finalResult.push_back(tweetOn);
				}
				else if(firstMention == currUser)
				{
					if(userHold.find(tweetOn->user()) != userHold.end())
					{
						std::set<User*> authorFollowing = tweetOn->user()->following();

						if(authorFollowing.find(usernameU) != authorFollowing.end())
						{
							finalResult.push_back(tweetOn);
						}
					}
				}
			}
			else
			{
				// NORMAL GOOD TWEET
				finalResult.push_back(tweetOn);
			}
		}

		// MERGESORT BEFORE FINAL OUTPUT
		mergeSort(finalResult, TweetComp());

		for(unsigned int i = 0; i < finalResult.size(); i++)
		{
			ofile << *finalResult.at(i) << std::endl;
		}

	}

	// MENTIONS DUMP
	for(it = userCollection.begin(); it != userCollection.end(); ++it)
	{
		std::string currUser = it->first;
		std::string nameOfMentionFile = it->first;

		nameOfMentionFile += ".mentions";

		std::ofstream ofile(nameOfMentionFile.c_str());
		ofile << currUser << std::endl;

		std::vector<Tweet*> mentionHold = userCollection[currUser]->getMentions();

		// MERGESORT BEFORE FINAL OUTPUT
		mergeSort(mentionHold, TweetComp());

		for(unsigned int i = 0; i < mentionHold.size(); i++)
		{
			ofile << *mentionHold.at(i) << std::endl;
		}
	}

}

void TwitEng::addUser(std::string firstname)
{
	if(userCollection.find(firstname) == userCollection.end())
	{
		User *newUser = new User(firstname);
		userCollection.insert(std::pair<std::string, User*> (firstname, newUser));
	}
}

void TwitEng::hashCodeView()
{
	std::map<std::string, unsigned long long>::iterator it;

	for(it = userHashCodeMap.begin(); it != userHashCodeMap.end(); ++it)
	{
		std::cout << "user: " << it->first << " & code: " << it->second << std::endl;
	}
}

void TwitEng::addHashCode(std::string firstname, unsigned long long hashCode)
{
	if(userHashCodeMap.find(firstname) == userHashCodeMap.end())
	{
		userHashCodeMap.insert(std::pair<std::string, unsigned long long> (firstname, hashCode));
	}
}


void TwitEng::login(std::string username)
{
	// just to be explicit
	if(isLoggedIn == true)
	{
		this->logout();
	}

	isLoggedIn = true;

	current_user = userCollection[username];
}

void TwitEng::logout()
{
	isLoggedIn = false;

	current_user = NULL;
}

void TwitEng::displayUserSet()
{
	std::map<std::string, User*>::iterator it;

	std::cout << "USER SET: " << std::endl;
	for (it = userCollection.begin(); it != userCollection.end(); ++it)
	{
		std::cout << it->second->name() << std::endl;
	}
	std::cout << "END SET---" << std::endl;

}
