twitter: tagdata.h heap.h hash.h hash.cpp graph.h graph.cpp msort.h cmdhandler.cpp cmdhandler.h datetime.cpp datetime.h handler.cpp handler.h tweet.cpp tweet.h twiteng.cpp twiteng.h twitter.cpp user.cpp user.h util.cpp util.h
	g++ -g -std=c++11 -Wall hash.cpp graph.cpp twitter.cpp datetime.cpp user.cpp tweet.cpp twiteng.cpp util.cpp cmdhandler.cpp handler.cpp -o twitter

splaytest: splaytest.cpp splay_tree.h bst.h avlbst.h
	g++ -g -std=c++11 -Wall splaytest.cpp -o splaytest

hashtest: hashtest.cpp hash.cpp hash.h
	g++ -g -std=c++11 -Wall hashtest.cpp hash.cpp -o hashtest

heaptest: heaptest.cpp heap.h tagdata.h
	g++ -g -std=c++11 -Wall heaptest.cpp -o heaptest