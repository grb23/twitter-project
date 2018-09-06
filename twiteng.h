#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <string>
#include <set>
#include <stack>
#include <vector>
#include "user.h"
#include "tweet.h"
#include "datetime.h"
#include "hash.h"
#include "heap.h"
#include "tagdata.h"

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();
  /**
   * Parses the Twitter database and populates internal structures
   * @param filename of the database file
   * @return true if there is an error, false if successful
   */
  bool parse(char* filename);

  std::set<std::string> parseOutSet(std::string tweet, char ch);
  std::set<std::string> parseOutSetCase(std::string tweet, char ch);


  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(std::string& username, DateTime& time, std::string& text);

  /**
   * Overloaded, the good one
   */
  void addTweet(std::string& username, DateTime& time, std::string& text, std::set<std::string>& hashtags);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);


  // for searching
  std::set<Tweet*> setIntersect(std::set<Tweet*>& hold, std::set<Tweet*> tweetSet);

  std::set<Tweet*> setUnion(std::set<Tweet*>& hold, std::set<Tweet*> tweetSet);

  // testing stuff
  bool checkDateFormatting(std::string dateLine);


  /**
   * Dump feeds of each user to their own file
   */
  void dumpFeeds();

  void makeSaveFile(std::string filename);
  void makeSCCFile(std::string filename);

  void addUser(std::string firstname);

  void addHashCode(std::string firstname, unsigned long long hashCode);

  void login(std::string username);

  void logout();

  void trending(int num_tags);


  // personal use
  void displayUserSet();
  void hashCodeView();


  // only used like once
  std::map<std::string, User*> const getCollection() const;

  const std::map<std::string, User*>& getCo() const;
  const std::map<std::string, unsigned long long>& getHashCo() const;
  const Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher>& getHeap() const;
  const bool& loginState() const;
  const User* getCurrentUser() const;

private:
  std::map<std::string, User*> userCollection;
  std::map<std::string, unsigned long long> userHashCodeMap;
  std::map<std::string, std::set<Tweet*> > hashtagMap;

  // the hashtag, to the number of occurences, updated all the time
  std::map<std::string, int> tagToHeapMap;

  Heap<TagData, TagStringEqual, TagIntGreater, TagStringHasher> trending_;


  bool isLoggedIn;
  User* current_user;


};


#endif
