# Twitter Clone

Jay Doshi

jaydoshi@usc.edu

*This code is updated on https://github.com/jaydoshi/twitter-project, other project code is available on my profile*

Download all the files, open in Linux command line:

**make twitter**

then

run ./twitter [twitter input file]

in this case, run **./twitter twitter.dat**

The following demo users and passwords for login purposes are: 

Tommy - fighton

Jill - password

Mark - abc123

Sam - Cs4

The twitter.dat file is required for use, the first line indicates how many users are in the system, followed by the follower/following pairs, then a list of tweets to start the program. If you choose to write your own input file, please follow the twitter.dat provided in terms of format. The date/time has to be in the same format I've given. You can use the hash function to come up with more passwords for more users if you choose to, but then you must update the input file with the hashed password integer, as well as the user name, and who they are following.

Example:

[User] [Hashed password] [User they are following] [User they are following]

Mark 28114 Tommy Jill

-end-
